#include "Network.h"
#include "TcpIOObjects.h"

namespace network
{
	void CNetwork::handleTcpAccept(const IOObjectPtr& object)
	{
		auto listener = std::dynamic_pointer_cast<TcpListener>(object);
		assert(listener);
		auto protocol = listener->getProtocol();
		auto endPoint = listener->accept();
		if (!endPoint) 
		{
			return;
		}
		auto key = popKey();
		if (!key) 
		{
			core_log_error("not key accept");
			return;
		}
		auto conPrototcol = protocol->create();
		conPrototcol->setKey(key);
		auto con = CObjectPool<TcpConnection>::Instance()->create(conPrototcol, std::move(endPoint));
		con->setErrorCallback(std::bind(&CNetwork::handleTcpConError, this, _1));
		con->setReadCallback(std::bind(&CNetwork::handleTcpConRead, this, _1));
		con->setWriteCallbac(std::bind(&CNetwork::handleTcpConWrite, this, _1));
		addObject(con);
		_poller->deregisterReadhandler(object);
		protocol->onAccept(conPrototcol);
	}

	void CNetwork::handleTcpConError(const IOObjectPtr& object)
	{
		core_log_error("tcp connection error", object->getKey(), object->getSocket());
		auto con = std::dynamic_pointer_cast<TcpConnection>(object);
		closeTcpCon(con);
	}

	void CNetwork::handleTcpConRead(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<TcpConnection>(object);
		if (con->getState() == DISCONNECTING)
		{
			core_log_trace("read disconnection", object->getKey());
			return;
		}
		auto endPoint = con->getEndPoint();
		auto inputBuff = con->getInputBuff();
		auto protocol = con->getProtocol();
		SBufferVec* writev = inputBuff->getBufferVec();
		int32 writeable = writev[0].len + writev[1].len;
		constexpr int32 extralen = 65536;
		char extrabuf[extralen];
		struct iovec vec[2];
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
				inputBuff->write(writeable);
				inputBuff->write(extrabuf, cnt - writeable);
			}
			else
			{
				inputBuff->write(cnt);
			}
			protocol->onSerialize(inputBuff);
		}
		else if (cnt == 0)
		{
			closeTcpCon(con);
		}
		else
		{
			core_log_error("tcp connection read error", endPoint->getSocketError());
		}
	}

	void CNetwork::handleTcpConWrite(const IOObjectPtr& object)
	{

	}

	void CNetwork::closeTcpCon(const TcpConnectionPtr& con)
	{
		auto protocol = con->getProtocol();
		con->setState(DISCONNECTED);
		_poller->deregisterObject(con);
		removeObject(con->getKey());
		protocol->onClose();
	}

	void CNetwork::processTcpObjectEvent(const IOObjectPtr& object, IOEvent* event)
	{
		switch (event->getType())
		{
		case IO_EVENT_DATA:
		{
			break;
		}
		case IO_EVENT_CLOSE:
		{
			closeTcpCon(std::dynamic_pointer_cast<TcpConnection>(object));
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::tcpListen(int16 port, const IOProtocolPtr& protocol)
	{
		auto socket = common::CreateSocket(EPROTO_TCP);
		auto address = CAddress(INADDR_ANY, port);
		auto endPoint = CObjectPool<CEndPoint>::Instance()->createUnique(socket, address);
		auto listener = CObjectPool<TcpListener>::Instance()->create(protocol, std::move(endPoint));
		listener->setErrorCallback(std::bind(&CNetwork::defaultErrorHandle, this, _1));
		listener->setReadCallback(std::bind(&CNetwork::handleTcpAccept, this, _1));
		if (!listener->listen())
		{
			protocol->onUnlisten();
			return;
		}
		addObject(listener);
		_poller->deregisterReadhandler(listener);
	}

	void CNetwork::tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol)
	{

	}
	
}