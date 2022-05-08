#pragma once
#include "Configs.h"
#include "Address.h"

namespace network
{
	typedef std::shared_ptr<class CEndPoint> CEndPointPtr;
	typedef std::unique_ptr<class CEndPoint, std::function<void(CEndPoint*)>> CEndPointUnPtr;
	class CEndPoint : public CNoncopyable , public CPoolObject
	{
	public:
		CEndPoint();

		//CEndPoint(SOCKET s, const CAddress& address); 

		~CEndPoint();

		void onAwake(SOCKET s, const CAddress& address);

		void onRecycle();

		void reset();

		inline void setSocket(SOCKET s) { _socket = s; }

		inline SOCKET getSocket() const { return _socket; }

		inline void setAddress(const CAddress& address) { _address = address; }

		inline void setAddress(uint32 ip, uint16 port) {
			_address.ip(ip);
			_address.port(port);
		}

		CAddress getAddress() const { return _address; };

		inline bool good() { return _socket != INVALID_SOCKET; }

		void close();

		int32 setNonblock();

		void setReuseAddr(bool on = true);

		int32 bind();

		int32 listen();

		int32 connect();

		CEndPointUnPtr accept();

		int32 read(char* buff, int32 len);

		int32 shutdownWrite();

#ifdef __linux
		int32 readv(const struct iovec* iov, int32 iovcnt);
#endif // __linux

		int32 write(char* buff, int32 len);

		int32 getSocketError();

	private:
		SOCKET _socket;
		CAddress _address;
	};
}