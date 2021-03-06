#pragma once
#include "IOEvent.h"
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
		IOClose(uint32 key) :IOEvent(IO_EVENT_CLOSE, key)
		{

		}
	};

	template<class UserData>
	class IOEventData : public IOEvent
	{
	public:
		IOEventData(uint32 key, const UserData& data) :IOEvent(IO_EVENT_DATA, key), _data(data)
		{

		}

		UserData getData() { return _data; }
	private:
		UserData _data;
	};

	class IOEventClear : public IOEvent
	{
	public:
		IOEventClear(int32 group) :IOEvent(IO_EVENT_CLEAR, 0), _group(group)
		{

		}

		int32 getGroup() const { return _group; }
	private:
		int32 _group;
	};
}