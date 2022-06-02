#pragma once
#include "IOEvent.h"
#include "Address.h"
#include "IOProtocol.h"

namespace net
{
	class IOListen : public IOEvent
	{
	public:
		IOListen(uint16 port, const IOProtocolPtr& protocol) : IOEvent(IO_EVENT_LISTEN, protocol->getKey()), _port(port), _protocol(protocol)
		{
		}

		IOProtocolPtr& getProtocol() { return _protocol; }
		uint16 getPort() const { return _port; };
	private:
		uint16 _port;
		IOProtocolPtr _protocol;
	};

	class IOEConnect : public IOEvent
	{
	public:
		IOEConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol) : IOEvent(IO_EVENT_CONNECT, protocol->getKey()), _ip(ip), _port(port), _protocol(protocol)
		{

		}

		const std::string& getIp() const { return _ip; };
		int16 getPort() const { return _port; }
		IOProtocolPtr& getProtocol() { return _protocol; }
	private:
		std::string _ip;
		int16 _port;
		IOProtocolPtr _protocol;
	};

	class IOClose : public IOEvent
	{
	public:
		IOClose(uint32 key, int32 delaySec) :IOEvent(IO_EVENT_CLOSE, key), _delaySec(delaySec)
		{

		}

		int32 getDelay() const { return _delaySec; }
	private:
		int32 _delaySec;
	};

	template<class UserData>
	class IOEventData : public IOEvent
	{
	public:
		IOEventData(uint32 key) :IOEvent(IO_EVENT_DATA, key)
		{

		}

		UserData getData() { return _data; }
	private:
		UserData _data;
	};
}