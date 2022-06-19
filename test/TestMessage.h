#include "engine/IMessage.h"
#include "Configs.h"


namespace msgs
{
    

    class TestMessage : public engine::IMessage
    {
    public:

        enum{ IDENTITY = 9999 };

        TestMessage():value1(0), value2(0) {};

        virtual ~TestMessage() {};

        virtual void serialize(net::BufferReader& archive) override;

        virtual void serialize(net::BufferWriter& archive) const override;

        virtual int32 identity() const { return IDENTITY; }

        static IMessage* Create();

        std::string tostirng();

        int32 value1;
        int64 value2;
		std::string str;
		std::vector<int64> values;
		std::map<int32, int64> map;
		std::set<int32> set;
		std::vector<std::string> strValues;
    };
    USING_SHARED_PTR(TestMessage);
}