#pragma once
#include "engine/Messages.h"

namespace msgs
{
    class TestMessage : public engine::IMessage
    {
    public:

        enum{ IDENTITY = 9999 };

        TestMessage():value1(0), value2(0) {};

        virtual ~TestMessage() {};

        virtual void serialize(BufferReader& archive);

        virtual void serialize(BufferWriter& archive) const;

        virtual int32 identity() const { return IDENTITY; }

        static IMessage* Create();

        std::string tostirng();

        int32 value1;
        int64 value2;
        //string str;
        //std::vector<int32> values;
        //std::map<int32, int64> map;
        //std::set<int32> set;
        //std::vector<string> strValues;
    };
    USING_SHARED_PTR(TestMessage);
}