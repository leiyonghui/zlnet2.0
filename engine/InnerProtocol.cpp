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

			__InnerBuf.clear();
			if (auto message = packet->getMessage())
			{
				net::BufferWriter writer(&__InnerBuf);
				writer.writeInt32(message->identity());
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
					head->cmd = net::hostToNetwork32(packet->getCommand());
					head->callback = net::hostToNetwork32(packet->getCallbackId());
					head->size = net::hostToNetwork32(totalsize);
					__InnerBuf.read(0, (char*)(head + 1), datasize);
				}
				else
				{
					int32 totalsize = datasize + uint32(sizeof(HeaderMessage));
					HeaderMessage* head = (HeaderMessage*)(buffer->write(0, totalsize));
					head->type = HeaderTypeMessage;
					head->cmd = net::hostToNetwork32(packet->getCommand());
					head->size = net::hostToNetwork32(totalsize);
					__InnerBuf.read(0, (char*)(head + 1), datasize);
				}
			}
			else if(packet->getCallbackId())
			{
				int32 totalsize = datasize + uint32(sizeof(HeaderCallback));
				HeaderCallback* head = (HeaderCallback*)(buffer->write(0, totalsize));
				head->type = HeaderTypeCallback;
				head->callback = net::hostToNetwork32(packet->getCallbackId());
				head->error = net::hostToNetwork32(packet->getError());
				head->size = net::hostToNetwork32(totalsize);
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
		char str[50];
		buffer->look(str, sizeof(Block));
		core_log_debug("======", int16(block.type), block.size);
		for (int32 i = 0 ; i< sizeof(Block); i++)
		{
			printf("%c", str[i]);
		}
		core_log_debug("======");
		if (block.size > buffer->size())
			return;
		__InnerBuf.clear();
		__InnerBuf.ensure(block.size);
		buffer->read(__InnerBuf.write(0, block.size), block.size);
		if (block.type == HeaderTypeCall)
		{
			HeaderCall* head = (HeaderCall*)(__InnerBuf.read(0, sizeof(HeaderCall)));
			head->size = net::hostToNetwork32(head->size);
			head->cmd = net::networkToHost32(head->cmd);
			head->callback = net::networkToHost32(head->callback);

			int32* id = (int32*)__InnerBuf.read(sizeof(HeaderCall), sizeof(int32));
			auto msglen = head->size - sizeof(HeaderCall) - sizeof(int32);
			IMessagePtr message;
			if (msglen) 
				message = std::make_shared<SerializeMessage>(*id, __InnerBuf.read(sizeof(HeaderCall) + sizeof(int32), msglen), msglen);

			IOPacketPtr packet(new IOPacket(getKey(), head->cmd, head->callback, 0, std::static_pointer_cast<IMessage>(message)));
			dispatchPacket(packet);
		}
		else if (block.type == HeaderTypeCallback)
		{
			HeaderCallback* head = (HeaderCallback*)(__InnerBuf.read(0, sizeof(HeaderCallback)));
			head->size = net::hostToNetwork32(head->size);
			head->callback = net::networkToHost32(head->callback);
			head->error == net::hostToNetwork32(head->error);

			int32* id = (int32*)__InnerBuf.read(sizeof(HeaderCallback), sizeof(int32));
			auto msglen = head->size - sizeof(HeaderCallback) - sizeof(int32);
			IMessagePtr message;
			if (msglen > 0) 
				message = std::make_shared<SerializeMessage>(*id, __InnerBuf.read(sizeof(HeaderCallback) + sizeof(int32), msglen), msglen);

			IOPacketPtr packet(new IOPacket(getKey(), 0, head->callback, head->error, std::static_pointer_cast<IMessage>(message)));
			dispatchPacket(packet);
		}
		else if (block.type == HeaderTypeMessage)
		{
			HeaderMessage* head = (HeaderMessage*)(__InnerBuf.read(0, sizeof(HeaderMessage)));
			head->size = net::hostToNetwork32(head->size);
			head->cmd = net::networkToHost32(head->cmd);

			int32* id = (int32*)__InnerBuf.read(sizeof(HeaderMessage), sizeof(int32));
			auto msglen = head->size - sizeof(HeaderMessage) - sizeof(int32);
			core_log_debug("..", head->size, head->cmd, msglen, id);
			IMessagePtr message;
			if (msglen)
				message = std::make_shared<SerializeMessage>(*id, __InnerBuf.read(sizeof(HeaderMessage) + sizeof(int32), msglen), msglen);

			IOPacketPtr packet = std::make_shared<IOPacket>(getKey(), head->cmd, 0, 0, std::static_pointer_cast<IMessage>(message));
			dispatchPacket(packet);
		}
	}

	
	
}
