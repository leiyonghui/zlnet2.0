#include "Buffer.h"
#include "core/ResourceMonitor.h"

namespace net
{
    void __AddCounter(const std::string& name, int32 add)
    {
        ResourceMonitor::Instance()->addCounter(name, add);
    }

    void __DelCounter(const std::string& name, int32 add)
    {
        ResourceMonitor::Instance()->delCounter(name, add);
    }

}