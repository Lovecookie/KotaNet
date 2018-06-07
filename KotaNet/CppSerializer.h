#pragma once
#include "ISerializer.h"

namespace Kota
{
    class CppSerializer : public ISerializer
    {
    public:
        virtual ~CppSerializer() = default;

        bool Serialize(const char* data) override;
        bool Deserialize(const char* data) override;
    };
}
