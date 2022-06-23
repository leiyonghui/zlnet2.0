#pragma once
#include "Configs.h"

namespace net
{
    class IBuffer
    {
    public:
        virtual const char* data() const = 0;

        virtual uint32 size() const = 0;

        virtual void write(uint32 pos, const char* buff, uint32 size) = 0;

        virtual char* write(uint32 pos, uint32 size) = 0;

        virtual void read(uint32 pos, char *buff, uint32 size) const = 0;

        virtual const char* read(uint32 pos, uint32 size) const = 0;
    };
}