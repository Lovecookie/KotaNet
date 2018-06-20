#pragma once

#include "SampleDefine.h"

namespace Kota
{
    class ISerializer abstract
    {
    public:
        virtual bool Serialize( const char* data ) const = 0;
        virtual bool Deserialize( const char* data ) const = 0;
    };
}
