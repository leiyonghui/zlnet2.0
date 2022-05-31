#include "Network.h"
#include "TCP_IOObjects.h"
#include "Buffer.h"

namespace network
{
	void CNetwork::handleTcpAccept(const IOObjectPtr& object)
	{
		auto listener = std::dynamic_pointer_cast<TcpListener>(object);
		assert(listener);
		auto endPoint = listener->accept();
		core_log_trace("io accept", endPoint->getAddress().toString());
		if (!endPoint) 
		{
			core_log_error("unaccept", listener->getKey());
			removeTcpListen(listener);
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
		prototcol->setType(IO_OBJECT_CONNECTION);
		CAddress localname;
		endPoint->getLocalName(localname);
		prototcol->setLoclAddress(localname);
		CAddress remotename;
		endPoint->getRemoteName(remotename);
		auto con = CObjectPool<Connection>::Instance()->create(prototcol, std::move(endPoint));
		con->setErrorCallback(std::bind(&CNetwork::handleTcpConError, this, _1));
		con->setReadCallback(std::bind(&CNetwork::handleTcpConRead, this, _1));
		con->setWriteCallback(std::bind(&CNetwork::handleTcpConWrite, this, _1));
		addObject(con);
		_poller->registerReadHandler(object);
		prototcol->onAccept(listener->getProtocol());
	}

	void CNetwork::handleTcpConError(const IOObjectPtr& object)
	{
		core_log_error("tcp connection error", object->getKey(), object->getSocket());
		auto con = std::dynamic_pointer_cast<Connection>(object);
		removeTcpCon(con);
	}

	void CNetwork::handleTcpConRead(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);
		if (!con->enable())
		{
			core_log_trace("con dis read", object->getKey(), con->getState());
			return;
		}
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
			if (cnt >= writeable)
			{
				inputBuffer->write_confirm(writeable);
				inputBuffer->write(extrabuf, cnt - writeable);
			}
			else
			{
				inputBuffer->write_confirm(cnt);
			}
			protocol->onUnserialize(inputBuffer);
		}
		else if (cnt == 0)
		{
			removeTcpCon(con);
		}
		else
		{
			auto er = endPoint->getSocketError();
			core_log_error("tcp connection read error", object->getKey(), er, strerror(er));
		}
	}

	void CNetwork::handleTcpConWrite(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);
		if (!con->enable())
		{
			core_log_error("con writeable unexpect state", con->getState());
			assert(false);
		}
		auto outBuffer = con->getOutBuffer();
		auto size = int32(outBuffer->size());
		if (!size)
		{
			core_log_warning("send null data", con->getKey());
			return;
		}
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
			if (con->getState() == DISCONNECTING)
			{
				tcpClose(con, 0);
			}
			else if(con->isWriting())
			{
				con->setWriting(false);
				_poller->deregisterWriteHandler(con);
			}
		}
		else if (count > 0 && count < size)
		{
			core_log_warning("con write messge size:", size, " remain:", size - count);
			outBuffer->read_confirm(count);
		}
		else
		{
			auto er = endPoint->getSocketError();
			core_log_error("write con", con->getKey(), count, er, strerror(er));
		}
	}

	void CNetwork::handleTcpConnectWrite(const IOObjectPtr& object)
	{
		auto connect = std::dynamic_pointer_cast<TcpConnector>(object);
		auto state = connect->getState();
		assert(state == CONNECTING);
		auto endPont = connect->getEndPoint();
		auto err = endPont->getSocketError();
		if (err)
		{
			core_log_warning("connect error", object->getSocket(), strerror(err));
			tcpConnectError(connect);
		}
		else if(endPont->isSelfConnect())
		{
			core_log_error("connect self connect", object->getSocket());
			tcpConnectError(connect);
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
			protocol->onConnect(false);
		}

	}

	void CNetwork::handleTcpConnectError(const IOObjectPtr& object)
	{
		tcpConnectError(std::dynamic_pointer_cast<TcpConnector>(object));
	}

	void CNetwork::removeTcpListen(const TcpListenerPtr& listener)
	{
		auto protocol = listener->getProtocol();
		_poller->deregisterObject(listener);
		removeObject(listener->getKey());
		protocol->onUnlisten();
	}

	void CNetwork::removeTcpCon(const ConnectionPtr& con)
	{
		auto protocol = con->getProtocol();
		con->setState(DISCONNECTED);
		_poller->deregisterObject(con);
		removeObject(con->getKey());
		if (con->getType() == IO_OBJECT_CONNECTION)
		{
			protocol->onClose();
		}
		else 
		{
			protocol->onDisConnect();
		}
	}

	void CNetwork::tcpListen(int16 port, const IOProtocolPtr& protocol)
	{
		auto socket = common::CreateSocket(EPROTO_TCP);
		auto address = CAddress(INADDR_ANY, port);
		auto listener = CObjectPool<TcpListener>::Instance()->create(protocol, CObjectPool<CEndPoint>::Instance()->createUnique(socket, address));
		listener->setErrorCallback(std::bind(&CNetwork::defaultErrorHandle, this, _1));
		listener->setReadCallback(std::bind(&CNetwork::handleTcpAccept, this, _1));
		protocol->setLoclAddress(address);
		auto endPoint = listener->getEndPoint();
		endPoint->setReuseAddr();
		endPoint->setReusePort();
		if (endPoint->bind() != 0)
		{
			core_log_error("listen bind", address.toString());
			protocol->onListen(false);
			return;
		}
		if (endPoint->listen() != 0)
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
		if (!con->enable())
		{
			core_log_trace("con dis send", object->getKey(), con->getState());
			return;
		}
		auto protocol = con->getProtocol();
		auto buffer = StackBuffer<2048>();
		protocol->onSerialize(event, &buffer);
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
		addObject(connect);
		auto ret = connect->getEndPoint()->connect();
		core_log_trace("connect", ret, strerror(ret));

		connect->setState(CONNECTING);
		_poller->registerWriteHandler(connect);
	}

	void CNetwork::tcpConnectError(const TcpConnectorPtr& connect)
	{
		_poller->deregisterObject(connect);
		connect->setState(DISCONNECTED);
		connect->setEndPoint(nullptr);
		//removeObject(connect->getKey());
		auto protocol = connect->getProtocol();
		protocol->onConnect(false);
		_timerHandler->addTimer(100ms * 2, 0ms, [this, connect]() {
			auto object = getObject(connect->getKey());
			if (object)
			{
				core_log_trace("reconnect", connect->getKey(), connect->getState());
				assert(connect->getState() == DISCONNECTED);

				auto socket = common::CreateSocket(EPROTO_TCP);
				connect->setEndPoint(CObjectPool<CEndPoint>::Instance()->createUnique(socket, connect->getEndPoint()->getAddress()));
				connect->setState(CONNECTING);
				auto ret = connect->getEndPoint()->connect();
				_poller->registerWriteHandler(connect);

				core_log_trace("reconnect", ret, strerror(ret));
			}
			else
			{
				core_log_warning("unknow", connect->getKey());
			}
		});
	}

	void CNetwork::tcpClose(uint32 key, uint32 second)
	{
		auto object = getObject(key);
		switch (object->getType())
		{
		case IO_OBJECT_LISTENER:
			break;
		case IO_OBJECT_CONNECTION:
		case IO_OBJECT_CONNECTOR:
			tcpClose(std::dynamic_pointer_cast<Connection>(object), second);
			break;
		default:
			break;
		}
	}

	void CNetwork::tcpClose(const ConnectionPtr& con, int32 second)
	{
		auto state = con->getState();
		assert(state != DISCONNECTED);
		if (state == DISCONNECTING)
		{
			core_log_warning("closing con", con->getType(), con->getSocket());
			return;
		}
		auto protocol = con->getProtocol();
		auto type = con->getType();
		if (type == IO_OBJECT_CONNECTOR)
		{
			if (state == CONNECTED)
			{
				protocol->onDisConnect();
			}
			else
			{
				core_log_trace("close connector state", con->getKey(), state);
				//protocol->onConnect(false);
			}
		}
		else
		{
			protocol->onClose();
		}
		if ((state == CONNECTED) && second > 0)
		{
			con->setState(DISCONNECTING);
			_timerHandler->addTimer(1ms * second, 0ms, [this, con]() {
				auto object = getObject(con->getKey());
				if (object)
				{
					tcpClose(con, 0);
				}
			});
		}
		else
		{
			con->setState(DISCONNECTED);
			_poller->deregisterObject(con);
			removeObject(con->getKey());
		}
	}

}