#pragma once
 
#include "MessageHeader.h"

namespace Kota
{   
    struct MsgChat final : MessageBase
    {
        virtual MessageBase* Clone() const = 0;
        bool Deserialize( const ISerializer* const pSerializer, const char* pBody ) override
        {
            pSerializer->Deserialize( pBody );
        }

        bool Serialize( const ISerializer* const pSerializer, const char* pBody ) override
        {   
            pSerializer->Serialize( pBody );
        }

        std::array<char, 128> data;
    };

    
}