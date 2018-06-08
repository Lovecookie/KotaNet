#pragma once
#include "ISerializer.h"

namespace Kota
{
    class CppSerializer final : public ISerializer
    {
    public:
        ~CppSerializer() = default;        
        
        bool Serialize( const char* data ) const override
        {   
        }

        bool Deserialize( const char* data ) const override
        {
        }

    private:
        
    };
}
