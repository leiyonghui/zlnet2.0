#include "TestMessage.h"
#include "engine/Serializes.h"
#include <iostream>

using namespace std;

namespace msgs
{
    void TestMessage::serialize(net::BufferReader& archive)
    {
        archive& value1& value2 & str& values& map& set& strValues;
    }

    void TestMessage::serialize(net::BufferWriter& archive) const
    {
        archive& value1& value2 & str & values & map& set & strValues;
    }

    IMessage* TestMessage::Create()
    {
        return new TestMessage();
    }

    std::string TestMessage::tostirng()
    {
        std::stringstream ss;
        ss << "value1:(" << value1<<")";
        ss << ",value2: " << value2 << ")";
        ss << ",str: " << str << endl;
        ss << ",values:(";
        for (auto value : values)
        {
            ss << "_" << value;
        }
        ss << ")" << endl;

        ss << ",map:{";
        for (auto& iter : map)
        {
            ss << "(" << iter.first << "," << iter.second << ")";
        }
        ss << "}," << endl;

        ss << "set:(";
        for (auto value : set)
        {
            ss << "_" << value;
        }
        ss << ")," << endl;

        ss << "strValues:{";
        for (auto value : strValues)
        {
            ss << "_" << value;
        }
        ss << "}," << endl;

        /*ss << member.tostirng(); */

        return ss.str();
    }
};
