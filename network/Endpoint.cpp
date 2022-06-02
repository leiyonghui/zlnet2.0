#include <string.h>

#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h> 
//#include <netdb.h>
#endif // __linux

#include "Endpoint.h"


namespace net
{
	CEndPoint::CEndPoint(): _socket(INVALID_SOCKET)
	{

	}

	//CEndPoint::CEndPoint(SOCKET s, const CAddress& address): _socket(s), _address(address)
	//{
	//}

	CEndPoint::~CEndPoint()
	{
		close();
	}

	void CEndPoint::onAwake(SOCKET s, const CAddress& address)
	{
		_socket = s;
		_address = address;
	}

	void CEndPoint::onRecycle()
	{
		close();
		reset();
	}

	void CEndPoint::reset()
	{
		_address.ip(0);
		_address.port(0);
		_socket = INVALID_SOCKET;
	}

	void CEndPoint::close()
	{
		if (_socket == INVALID_SOCKET)
		{
			core_log_warning("close invalid socket");
			return;
		}
#ifdef __linux
		::close(_socket);
#else // __liunx

#endif
		core_log_info("close socket", _socket);
	}

#ifdef __linux
	int32 CEndPoint::setNonblock()
	{
		int32 flags = ::fcntl(_socket, F_GETFL, 0);
		flags |= O_NONBLOCK;
		if (::fcntl(_socket, F_SETFL, flags)) {
			core_log_error("SocketOpt::setNonBlock", _socket);
			return errno;
		}
		return 0;
	}

	int32 CEndPoint::bind()
	{
		sockaddr_in addr = _address.get_sockaddr_in();
		socklen_t addrlen = static_cast<socklen_t>(sizeof(addr));
		if (::bind(_socket, (struct sockaddr*)&addr, addrlen))
		{
			core_log_error("SocketOpt::bind", errno, strerror(errno));
			return errno;
		}
		return 0;
	}

	int32 CEndPoint::listen()
	{
		if (::listen(_socket, SOMAXCONN))
		{
			core_log_error("SocketOpt::listen", errno, strerror(errno));
			return errno;
		}
		return 0;
	}

	int32 CEndPoint::connect()
	{
		sockaddr_in addr = _address.get_sockaddr_in();
		socklen_t addrlen = static_cast<socklen_t>(sizeof(addr));
		if (::connect(_socket, (struct sockaddr*)&addr, addrlen))
		{
			return errno;
		}
		return 0;
	}

	void CEndPoint::setReuseAddr(bool on)
	{
		int32 optval = on ? 1 : 0;
		::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
	}

	void CEndPoint::setReusePort(bool on)
	{
		int32 optval = on ? 1 : 0;
		::setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	}

	CEndPointUnPtr CEndPoint::accept()
	{
		sockaddr_in addr;
		socklen_t addrlen = static_cast<socklen_t>(sizeof(addr));
		int32 fd = ::accept(_socket, (struct sockaddr*)&addr, &addrlen);
		if (fd < 0)
		{
			core_log_error("SocketOpt::accpet", errno, strerror(errno));
			return nullptr;
		}
		CEndPointUnPtr newEndPoint = CObjectPool<CEndPoint>::Instance()->createUnique(fd, CAddress(addr));
		return newEndPoint;
	}

	int32 CEndPoint::read(char* buff, int32 len)
	{
		int32 count = (int32)::read(_socket, buff, (size_t)len);
		if (count < 0)
		{
			core_log_warning("SocketOpt::read", errno, strerror(errno));
		}
		return count;
	}

	int32 CEndPoint::write(const char* buff, int32 len)
	{
		uint32 count = (uint32)::write(_socket, buff, (size_t)len);
		if (count < 0)
		{
			core_log_warning("SocketOpt::write", errno, strerror(errno));
		}
		return count;
	}

	int32 CEndPoint::readv(const iovec* iov, int32 iovcnt)
	{
		int32 count = (int32)::readv(_socket, iov, iovcnt);
		if (count < 0)
		{
			core_log_warning("SocketOpt::readv", errno, strerror(errno));
		}
		return count;
	}

	int32 CEndPoint::writev(const iovec* iov, int32 iovcnt)
	{
		int32 count = (int32)::writev(_socket, iov, iovcnt);
		if (count < 0)
		{
			core_log_warning("SocketOpt::writev", errno, strerror(errno));
		}
		return count;
	}

	int32 CEndPoint::shutdownWrite()
	{
		if (::shutdown(_socket, SHUT_WR) < 0)
		{
			core_log_error("sockets::shutdownWrite", errno, strerror(errno));
			return -1;
		}
		return 0;
	}

	int32 CEndPoint::getSocketError()
	{
		int optval;
		socklen_t optlen = static_cast<socklen_t>(sizeof optval);
		if (::getsockopt(_socket, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
		{
			return errno;
		}
		else
		{
			return optval;
		}
	}

	std::pair<CAddress, bool> CEndPoint::getRemoteName()
	{
		struct sockaddr_in remoteaddr;
		socklen_t addrlen = static_cast<socklen_t>(sizeof remoteaddr);
		if (::getpeername(_socket, (struct sockaddr*)&remoteaddr, &addrlen) < 0)
		{
			core_log_error("get remote name");
			return { CAddress(remoteaddr), false };
		}
		return { CAddress(remoteaddr) , true};
	}

	std::pair<CAddress, bool> CEndPoint::getLocalName()
	{
		struct sockaddr_in localname;
		socklen_t addrlen = static_cast<socklen_t>(sizeof localname);
		if (::getsockname(_socket, (struct sockaddr*)&localname, &addrlen) < 0)
		{
			core_log_error("get local name");
			return { CAddress(localname), false };
		}
		return { CAddress(localname), true };
	}

	bool CEndPoint::isSelfConnect()
	{
		auto pair1 = getLocalName();
		auto pair2 = getRemoteName();
		assert(pair1.second && pair2.second);
		if(pair1.first == pair2.first)
		{
			return true;
		}
		return false;
	}

#else // __liunx
	int32 CEndPoint::setNonblock()
	{
		return 0;
	}

	int32 CEndPoint::bind()
	{
		return 0;
	}

	int32 CEndPoint::listen()
	{
		return 0;
	}

	int32 CEndPoint::connect()
	{
		return 0;
	}

	CEndPointPtr CEndPoint::accept()
	{
		return 0;
	}

	int32 CEndPoint::read(char* buff)
	{
		return 0;
	}

	int32 CEndPoint::write(char* buff, int32 len)
	{
		return 0;
	}
#endif
}