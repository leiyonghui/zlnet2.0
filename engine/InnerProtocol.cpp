#include "IOPacket.h"
#include "InnerProtocol.h"
#include "network/IOEvents.h"

namespace engine
{
	static const uint32 INIT_BUFFER_SIZE = 128 * 1024;
	static const uint32 MAX_BUFFER_SIZE = 1024 * 1024;

	thread_local static net::Buffer __InnerBuf(INIT_BUFFER_SIZE);

    void InnerProtocol::onAwake(net::EPROTOCOL pType)
    {
		Protocol::onAwake(pType);
    }

    void InnerProtocol::onRecycle()
    {
		Protocol::onRecycle();
    }

    net::IOProtocolPtr InnerProtocol::create() const
	{
		auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
		protocol->setQueue(_msgqueue);
		return protocol;
	}

	void InnerProtocol::onSerialize(net::IOEvent* event, net::IBuffer* buffer)
	{
		if (auto ev = static_cast<net::IOEventData<IOPacketPtr>*>(event))
		{
			auto packet = ev->getData();

			uint32 datasize = 0;

			if (auto message = packet->getMessage())
			{
				net::BufferWriter writer(&__InnerBuf);
				message->serialize(writer);
				datasize = writer.bytesWritten();
			}

			if (packet->getCommand())
			{
				if (packet->getCallbackId())
				{
					int32 totalsize = datasize + uint32(sizeof(HeaderCall));
					HeaderCall* head =  (HeaderCall*)(buffer->write(0, totalsize));
					head->type = HeaderTypeCall;
					head->cmd = packet->getCommand();
					head->callback = packet->getCallbackId();
					head->size = totalsize;
					__InnerBuf.read(0, (char*)(head + 1), datasize);
				}
				else
				{
					int32 totalsize = datasize + uint32(sizeof(HeaderMessage));
					HeaderMessage* head = (HeaderMessage*)(buffer->write(0, totalsize));
					head->type = HeaderTypeMessage;
					head->cmd = packet->getCommand();
					head->size = totalsize;
					__InnerBuf.read(0, (char*)(head + 1), datasize);
				}
			}
			else if(packet->getCallbackId())
			{
				int32 totalsize = datasize + uint32(sizeof(HeaderCallback));
				HeaderCallback* head = (HeaderCallback*)(buffer->write(0, totalsize));
				head->type = HeaderTypeCallback;
				head->callback = packet->getCallbackId();
				head->error = packet->getError();
				head->size = totalsize;
				__InnerBuf.read(0, (char*)(head + 1), datasize);
			}
		}
		else
		{
			assert(false);
		}

	}

	void InnerProtocol::onUnserialize(net::IIOBuffer* buffer)
	{
		if (buffer->size() < sizeof(Block))
			return;
		Block block;
		buffer->look((char*)(&block), sizeof(Block));
		block.size = net::networkToHost32(block.size);
		if (block.size > buffer->size())
			return;

	}

	
	
}
