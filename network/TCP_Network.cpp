#include "Network.h"
#include "TCP_IOObjects.h"

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
		auto con = CObjectPool<Connection>::Instance()->create(conPrototcol, std::move(endPoint));
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
				inputBuffer->write_confirm(writeable);
				inputBuffer->write(extrabuf, cnt - writeable);
			}
			else
			{
				inputBuffer->write_confirm(cnt);
			}
			protocol->onSerialize(inputBuffer);
		}
		else if (cnt == 0)
		{
			removeTcpCon(con);
		}
		else
		{
			core_log_error("tcp connection read error", endPoint->getSocketError());
		}
	}

	void CNetwork::handleTcpConWrite(const IOObjectPtr& object)
	{
		auto con = std::dynamic_pointer_cast<Connection>(object);
		if (!con->enable())
		{
			core_log_trace("con dis write", object->getKey(), con->getState());
			return;
		}
		tcpWrite(con);
	}

	void CNetwork::removeTcpCon(const ConnectionPtr& con)
	{
		auto protocol = con->getProtocol();
		con->setState(DISCONNECTED);
		_poller->deregisterObject(con);
		removeObject(con->getKey());
		protocol->onClose();
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
		auto outBuffer = con->getOutBuffer();
		protocol->onUnserialize(event, outBuffer);
		if (con->isWriting())
		{
			core_log_warning("con is writing", con->getKey());
			return;
		}
		tcpWrite(con);
	}

	void CNetwork::tcpWrite(const ConnectionPtr& con)
	{
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
			if (con->isWriting())
				_poller->deregisterWritehandler(con);
		}
		else if (count > 0 && count < size)
		{
			core_log_warning("con write messge size:", size, " remain:", size - count);
			outBuffer->read_confirm(count);
			if (!con->isWriting())
				_poller->deregisterWritehandler(con);
		}
		else
		{
			core_log_error("write con", con->getKey(), count);
		}	
	}

	void CNetwork::tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol)
	{

	}

	void CNetwork::tcpClose(uint32 key, uint32 second)
	{

	}	
}