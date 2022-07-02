#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h> 
#include <netinet/in.h>
#endif // __linux

#include "Network.h"
#include "Buffer.h"
#include "IOObject.h"
#include "IOObjects.h"
#include "IOProtocol.h"
#include "TCP_IOObjects.h"
#include "EPoller.h"

#include "Common.h"

namespace net
{
	const int32 FORCE_CLOSE_SECOND = 8;

	void CNetwork::handleTcpAccept(const IOObjectPtr& object)
	{
		auto listener = std::dynamic_pointer_cast<TcpListener>(object);
		assert(listener);
		auto endPoint = listener->accept();
		if (!endPoint) 
		{
			core_log_error("unaccept", listener->getKey());
			tcpCloseListen(listener);
			return;
		}
		auto key = popKey();
		if (!key) 
		{
			core_log_error("not key accept");
			return;
		}
		auto prototcol = listener->getProtocol()->create();
		if (!prototcol)
		{
			core_log_error("unexpect create protocol");
			return;
		}
		prototcol->setKey(key);
		prototcol->setType(IO_OBJECT_SESSION);
		prototcol->setLoclAddress(endPoint->getLocalName().first);
		prototcol->setRemoteAddress(endPoint->getRemoteName().first);
		prototcol->setNetwork(this);

		auto con = CObjectPool<TcpSession>::Instance()->create(prototcol, std::move(endPoint));
		con->setErrorCallback(std::bind(&CNetwork::handleTcpConError, this, _1));
		con->setReadCallback(std::bind(&CNetwork::handleTcpConRead, this, _1));
		con->setWriteCallback(std::bind(&CNetwork::handleTcpConWrite, this, _1));
		con->setScheduler(_shceduler);
		con->setState(CONNECTED);

		addObject(con);
		_poller->registerReadHandler(con);

		prototcol->onAccept(listener->getProtocol());

		onNewConnection(std::static_pointer_cast<Connection>(con));
	}

	void CNetwork::handleTcpConError(const IOObjectPtr& object)
	{
		core_log_error("tcp connection error", object->getKey(), object->getSocket(), strerror(object->getEndPoint()->getSocketError()));
		tcpCloseCon(std::dynamic_pointer_cast<Connection>(object), true);
	}

	void CNetwork::handleTcpConRead(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);

		assert(con->enableRead());

		auto endPoint = con->getEndPoint();
		auto inputBuffer = con->getInputBuffer();
		auto protocol = con->getProtocol();
		SBufferVec* writev = inputBuffer->getWriteableVec();
		int32 writeable = writev[0].len + writev[1].len;
		constexpr int32 extralen = 65536;
		char extrabuf[extralen];
		struct iovec vec[3];
		vec[0].iov_base = writev[0].buff;
		vec[0].iov_len = writev[0].len;
		vec[1].iov_base = writev[1].buff;
		vec[1].iov_len = writev[1].len;
		vec[2].iov_base = extrabuf;
		vec[2].iov_len = extralen;
		int32 iovcnt = 3;
		if (writev[0].len >= extralen)
			iovcnt = 1;
		else if (writeable >= extralen)
			iovcnt = 2;
		int32 cnt = endPoint->readv(vec, iovcnt);
		if (cnt > 0)
		{
			if (cnt > writeable)
			{
				if (writeable)
					inputBuffer->write_confirm(writeable);
				inputBuffer->write(extrabuf, cnt - writeable);
			}
			else
			{
				inputBuffer->write_confirm(cnt);
			}
			if (!protocol->onUnserialize(inputBuffer))
			{
				core_log_error("unserialize error", con->getKey(), con->getType());
				tcpCloseCon(con, true);
			}
		}
		else if (cnt == 0)
		{
			tcpCloseCon(con, true);
		}
		else
		{
			//auto er = endPoint->getSocketError();
			core_log_error("tcp connection read error", object->getKey()/*, er, strerror(er)*/);
		}
	}

	void CNetwork::handleTcpConWrite(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);

		assert(con->enableWrite());
		assert(con->isWriting());

		auto outBuffer = con->getOutBuffer();
		auto size = int32(outBuffer->size());

		assert(size);

		SBufferVec* writev = outBuffer->getReadableVec();
		struct iovec vec[2];
		vec[0].iov_base = writev[0].buff;
		vec[0].iov_len = writev[0].len;
		vec[1].iov_base = writev[1].buff;
		vec[1].iov_len = writev[1].len;
		auto endPoint = con->getEndPoint();
		auto count = endPoint->writev(vec, size);
		if (count == size)
		{
			outBuffer->read_confirm(size);

			con->setWriting(false);
			_poller->deregisterWriteHandler(con);

			if (con->getState() == DISCONNECTING)
				con->getEndPoint()->shutdownWrite();//发送fin
		}
		else if (count > 0 && count < size)
		{
			core_log_warning("con write messge size:", size, " remain:", size - count);
			outBuffer->read_confirm(count);
		}
		else
		{
			//auto er = endPoint->getSocketError();
			core_log_error("write con", con->getKey(), count/*, er, strerror(er)*/);
		}
	}

	void CNetwork::handleTcpConnectWrite(const IOObjectPtr& object)
	{
		auto connect = std::dynamic_pointer_cast<TcpConnector>(object);
		auto state = connect->getState();
		assert(state == CONNECTING);
		auto endPont = connect->getEndPoint();
		if(auto ipair =  endPont->getRemoteName(); !ipair.second)
		{
			auto err = object->getEndPoint()->getSocketError();		//geterror会把error清理
			core_log_warning("connect remote error", object->getSocket(), err, strerror(err));
			onTcpConnectError(connect);
		}
		else if(endPont->isSelfConnect())
		{
			core_log_error("connect self connect", object->getSocket());
			onTcpConnectError(connect);
		}
		else
		{
			_poller->deregisterObject(object);
			connect->setState(CONNECTED);
			connect->setErrorCallback(std::bind(&CNetwork::handleTcpConError, this, _1));
			connect->setReadCallback(std::bind(&CNetwork::handleTcpConRead, this, _1));
			connect->setWriteCallback(std::bind(&CNetwork::handleTcpConWrite, this, _1));

			_poller->registerReadHandler(object);
			auto protocol = connect->getProtocol();
			protocol->setLoclAddress(endPont->getLocalName().first);
			protocol->setRemoteAddress(endPont->getRemoteName().first);

			protocol->onConnect(true);

			onNewConnection(std::static_pointer_cast<Connection>(connect));
		}
	}

	void CNetwork::handleTcpConnectError(const IOObjectPtr& object)
	{
		auto err = object->getEndPoint()->getSocketError();
		core_log_warning("connect error", object->getSocket(), strerror(err));

		onTcpConnectError(std::dynamic_pointer_cast<TcpConnector>(object));
	}

	void CNetwork::tcpListen(int16 port, const IOProtocolPtr& protocol)
	{
		auto socket = common::CreateSocket(EPROTO_TCP);
		if (socket == INVALID_SOCKET)
		{
			core_log_error("listen create socket");
			protocol->onListen(false);
			return;
		}
		auto address = CAddress(INADDR_ANY, port);
		auto listener = CObjectPool<TcpListener>::Instance()->create(protocol, CObjectPool<CEndPoint>::Instance()->createUnique(socket, address));
		listener->setErrorCallback(std::bind(&CNetwork::defaultErrorHandle, this, _1));
		listener->setReadCallback(std::bind(&CNetwork::handleTcpAccept, this, _1));
		protocol->setLoclAddress(address);
		auto endPoint = listener->getEndPoint();
		endPoint->setReuseAddr();
		endPoint->setReusePort();
		if (endPoint->bind() < 0)
		{
			core_log_error("listen bind", address.toString());
			protocol->onListen(false);
			return;
		}
		if (endPoint->listen() < 0)
		{
			protocol->onListen(false);
			core_log_error("listen", address.toString());
			return;
		}
		_poller->registerReadHandler(listener);
		addObject(listener);
		protocol->onListen(true);
	}

	void CNetwork::tcpSend(const IOObjectPtr& object, IOEvent* event)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);
		assert(con);
		if (!con->enableSend())
		{
			core_log_warning("con dis send", object->getKey(), con->getState());
			return;
		}
		auto protocol = con->getProtocol();
		auto buffer = StackBuffer<2048>();
		if (!protocol->onSerialize(event, &buffer))
		{
			core_log_error("serialize error", con->getKey(), con->getType());
			tcpCloseCon(con, true);
			return;
		}
		auto size = int32(buffer.size());
		if (size == 0)
		{
			core_log_warning("send null data", con->getKey());
			return;
		}
		auto data = buffer.data();
		auto endPoint = con->getEndPoint();
		auto count = endPoint->write(data, size);
		if (count > 0 && count != size)
		{
			auto outBuffer = con->getOutBuffer();
			outBuffer->write(data + count, size - count);
			if (!con->isWriting())
			{
				con->setWriting(true);
				_poller->deregisterWriteHandler(con);
			}
			core_log_warning("con write bigger", con->getSocket(), size, size - count);
		}
		else if(count < 0)
		{
			core_log_error("write con", con->getKey(), size, count);
		}
	}

	void CNetwork::tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol)
	{
		auto socket = common::CreateSocket(EPROTO_TCP);
		auto address = CAddress(ip, port);
		auto endPoint = CObjectPool<CEndPoint>::Instance()->createUnique(socket, address);
		auto connect = CObjectPool<TcpConnector>::Instance()->create(protocol, std::move(endPoint));
		connect->setErrorCallback(std::bind(&CNetwork::handleTcpConnectError, this, _1));
		connect->setWriteCallback(std::bind(&CNetwork::handleTcpConnectWrite, this, _1));
		connect->setScheduler(_shceduler);

		auto ret = connect->getEndPoint()->connect();
		core_log_trace("connect", ret, strerror(ret));

		addObject(connect);
		connect->setState(CONNECTING);
		_poller->registerWriteHandler(connect);
	}

	void CNetwork::onTcpConnectError(const TcpConnectorPtr& connect)
	{
		CAddress address = connect->getEndPoint()->getAddress();

		_poller->deregisterObject(connect);
		connect->setState(DISCONNECTED);
		connect->setEndPoint(nullptr);
		auto protocol = connect->getProtocol();
		protocol->onConnect(false);

		connect->startTimer(1000ms * 4, 0ms, [this, address](ConnectionPtr con) {
			if (getObject(con->getKey()))
			{
				auto connect = std::dynamic_pointer_cast<TcpConnector>(con);
				core_log_trace("reconnect", connect->getKey(), connect->getState());
				assert(connect->getState() == DISCONNECTED);

				auto socket = common::CreateSocket(EPROTO_TCP);
				connect->setEndPoint(CObjectPool<CEndPoint>::Instance()->createUnique(socket, address));
				connect->setState(CONNECTING);
				connect->getEndPoint()->setNonblock();
				auto ret = connect->getEndPoint()->connect();
				_poller->registerWriteHandler(connect);

				core_log_trace("reconnect", ret, strerror(ret));
			}
			else
			{
				core_log_warning("unknow", con->getKey());
			}
		});
	}

	void CNetwork::tcpClose(uint32 key, bool force)
	{
		auto object = getObject(key);
		switch (object->getType())
		{
		case IO_OBJECT_LISTENER:
			tcpCloseListen(std::dynamic_pointer_cast<TcpListener>(object));
			break;
		case IO_OBJECT_SESSION:
			tcpCloseConnection(std::dynamic_pointer_cast<TcpSession>(object), force);
			break;
		case IO_OBJECT_CONNECTOR:
			tcpCloseConnector(std::dynamic_pointer_cast<TcpConnector>(object), force);
			break;
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::tcpCloseCon(const ConnectionPtr& con, bool force)
	{
		if (con->getType() == IO_OBJECT_SESSION)
			tcpCloseConnection(std::dynamic_pointer_cast<TcpSession>(con), force);
		else
			tcpCloseConnector(std::dynamic_pointer_cast<TcpConnector>(con), force);
	}

	void CNetwork::tcpCloseConnection(const TcpSessionPtr& con, bool force)
	{
		assert(con->getType() == IO_OBJECT_SESSION);
		assert(con->getState() != DISCONNECTED);

		if (con->getState() == CONNECTED)
		{
			con->getProtocol()->onClose();
		}

		if (force)
		{
			removeTcpCon(con);
		}
		else
		{
			if (con->getState() == DISCONNECTING)
			{
				core_log_warning("close con distonnecting", con->getKey());
				return;
			}
			con->setState(DISCONNECTING);

			if (!con->isWriting())
				con->getEndPoint()->shutdownWrite();//发送fin				

			con->startTimer(1000ms * FORCE_CLOSE_SECOND, 0ms, [this](ConnectionPtr con) {
				if (!getObject(con->getKey()))
				{
					core_log_warning("close connection exist", con->getKey());
					return;
				}
				auto state = con->getState();
				assert(state == DISCONNECTING);
				removeTcpCon(con);
			});
		}
	}

	void CNetwork::tcpCloseConnector(const TcpConnectorPtr& con, bool force)
	{
		assert(con->getType() == IO_OBJECT_CONNECTOR);

		auto state = con->getState();
		auto protocol = con->getProtocol();
		core_log_trace("close connector", con->getKey(), state);
		if (state == DISCONNECTED || state == CONNECTING)
		{
			protocol->onConnect(false);
			removeTcpCon(con);
			return;
		}

		if (con->getState() == CONNECTED)
		{
			con->getProtocol()->onDisConnect();
		}

		if (force)
		{
			removeTcpCon(con);
		}
		else
		{
			if (state == DISCONNECTING)
			{
				core_log_trace("close con distonnecting", con->getKey());
				return;
			}
			con->setState(DISCONNECTING);

			if (!con->isWriting())
				con->getEndPoint()->shutdownWrite();

			con->startTimer(1000ms * FORCE_CLOSE_SECOND, 0ms, [this](ConnectionPtr con) {
				if (!getObject(con->getKey()))
				{
					core_log_warning("close connector exist", con->getKey());
					return;
				}
				auto state = con->getState();
				assert(state == DISCONNECTING);
				removeTcpCon(con);
			});
		}
	}

	void CNetwork::tcpCloseListen(const TcpListenerPtr& listener)
	{
		listener->getProtocol()->onUnlisten();
		removeTcpListen(listener);
	}

	void CNetwork::removeTcpListen(const TcpListenerPtr& listener)
	{
		_poller->deregisterObject(listener);
		removeObject(listener->getKey());
	}

	void CNetwork::removeTcpCon(const ConnectionPtr& con)
	{
		if (con->isWriting())
			core_log_warning("remove con is writing", con->getKey());

		con->setState(DISCONNECTED);
		_poller->deregisterObject(con);
		removeObject(con->getKey());
	}

}