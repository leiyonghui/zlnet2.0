#pragma once
#include "Configs.h"
#include "network/BufferReader.h"
#include "network/BufferWriter.h"

namespace engine
{
    using net::BufferReader;
    using net::BufferWriter;

    void Serialize(BufferWriter& writer, int8 value);
    void Serialize(BufferWriter& writer, int16 value);
    void Serialize(BufferWriter& writer, int32 value);
    void Serialize(BufferWriter& writer, int64 value);
    void Serialize(BufferWriter& writer, const std::string& value);

    template<typename T>
    inline void Serialize(BufferWriter& writer, const T& value)
    {
        value.serialize(writer);
    }

    template<typename T>
    inline void Serialize(BufferWriter& writer, const std::vector<T>& vec)
    {
        int32 len = (int32)vec.size();
        writer.writeInt32(len);
        for (auto& value : vec)
        {
            Serialize(writer, value);
        }
    }

    template<typename T1, typename T2>
    inline void Serialize(BufferWriter& writer, const std::map<T1, T2>& map)
    {
        int32 len = (int32)map.size();
        writer.writeInt32(len);
        for (auto& iter : map)
        {
            Serialize(writer, iter.first);
            Serialize(writer, iter.second);
        }
    }

    template<typename T>
    inline void Serialize(BufferWriter& writer, const std::set<T>& set)
    {
        int32 len = (int32)set.size();
        writer.writeInt32(len);
        for (auto& value : set)
        {
            Serialize(writer, value);
        }
    }

    void Serialize(BufferReader& reader, int8& value);
    void Serialize(BufferReader& reader, int16& value);
    void Serialize(BufferReader& reader, int32& value);
    void Serialize(BufferReader& reader, int64& value);
    void Serialize(BufferReader& reader, std::string& value);

    template<typename T>
    inline void Serialize(BufferReader& reader, T& value)
    {
        value.serialize(reader);
    }

    template<typename T>
    inline void Serialize(BufferReader& reader, std::vector<T>& vec)
    {
        int32 len = reader.readInt32();
        vec.clear();
        vec.resize(len);
        for (int32 i = 0;i < len; i++)
        {
            Serialize(reader, vec[i]);
        }
    }

    template<typename T1, typename T2>
    inline void Serialize(BufferReader& reader, std::map<T1, T2>& map)
    {
        map.clear();
        int32 len = reader.readInt32();
        for (;len;len--)
        {
            T1 key;
            Serialize(reader, key);
            Serialize(reader, map[key]);
        }
    }

    template<typename T>
    inline void Serialize(BufferReader& reader, std::set<T>& set)
    {
        set.clear();
        int32 len = reader.readInt32();
        for (; len; len--)
        {
            T value;
            Serialize(reader, value);
            set.insert(value);
        }
    }

    template<typename T>
    BufferWriter& operator &(BufferWriter& writer, const T& value)
    {
        Serialize(writer, value);
        return writer;
    }

    template<typename T>
    BufferReader& operator &(BufferReader& reader, T& value)
    {
        Serialize(reader, value);
        return reader;
    }

	template<class ...Args>
	void pack(BufferWriter& writer, Args ...args)
	{
		try
		{
			(writer & ... & args);
		}
		catch (std::exception& e)
		{
			core_log_error("pack", e.what());
		}
	}

	template<class ...Args>
	void unpack(BufferReader& reader, Args ...args)
	{
		try
		{
			(reader & ... & args);
		}
		catch (std::exception& e)
		{
			core_log_error("unpack", e.what());
		}
	}

}