#pragma once

#include "SampleDefine.h"
#include "ISerializer.h"

namespace Kota
{
    #pragma pack(push, 1)
    struct MessageHeader final
    {    
        static const UINT32 headerSize = sizeof( UINT16 ) + sizeof( UINT16 );

        UINT16 BodyLength()
        {
            return size - headerSize;
        }
    
        UINT16 size;
        UINT16 id;
    };

    #pragma pack(pop)

    
    struct MessageBase abstract
    {   
        virtual ~MessageBase() = default;

        virtual MessageBase* Clone() const = 0;
        virtual bool Deserialize( const ISerializer* const pSerializer, const char* pBody ) = 0;
        virtual bool Serialize( const ISerializer* const pSerializer, const char* pBody ) = 0;

        UINT16 size;
        UINT16 id;
    };
}