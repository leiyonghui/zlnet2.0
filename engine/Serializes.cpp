#include "Serializes.h"

namespace engine
{
    void Serialize(BufferWriter& writer, uint8 value)
    {
        writer.writeUint8(value);
    }

    void Serialize(BufferWriter& writer, uint16 value)
    {
        writer.writeUint16(value);
    }

    void Serialize(BufferWriter& writer, uint32 value)
    {
        writer.writeUint32(value);
    }

    void Serialize(BufferWriter& writer, uint64 value)
    {
        writer.writeUint64(value);
    }

    void Serialize(BufferWriter& writer, int8 value)
    {
		writer.writeInt8(value);
    }

    void Serialize(BufferWriter& writer, int16 value)
    {
		writer.writeInt16(value);
    }

    void Serialize(BufferWriter& writer, int32 value)
    {
		writer.writeInt32(value);
    }

    void Serialize(BufferWriter& writer, int64 value)
    {
		writer.writeInt64(value);
    }

    void Serialize(BufferWriter& writer, const std::string& value)
    {
        int32 len = (int32)value.size();
        writer.writeInt32(len);
        writer.writeBytes(value.c_str(), len);
    }
    
    void Serialize(BufferReader& reader, uint8& value)
    {
        value = reader.readUint8();
    }

    void Serialize(BufferReader& reader, uint16& value)
    {
		value = reader.readUint16();
    }

    void Serialize(BufferReader& reader, uint32& value)
    {
		value = reader.readUint32();
    }

    void Serialize(BufferReader& reader, uint64& value)
    {
		value = reader.readUint64();
    }

    void Serialize(BufferReader& reader, int8& value)
    {
        value = reader.readInt8();
    }

    void Serialize(BufferReader& reader, int16& value)
    {
        value = reader.readInt16();
    }

    void Serialize(BufferReader& reader, int32& value)
    {
        value = reader.readInt32();
    }

    void Serialize(BufferReader& reader, int64& value)
    {
        value = reader.readInt64();
    }

    void Serialize(BufferReader& reader, std::string& value)
    {
        int32 len = reader.readInt32();
        value.clear();
        auto buff = reader.read(len);
        value.append(buff, len);
    }
}
