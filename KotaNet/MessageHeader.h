#pragma once

#include "SampleDefine.h"
#include "ISerializer.h"
#include "ILogicTask.h"

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

        bool IsInvalid( const INT32 readSize )
        {
            return size > readSize;
        }
    
        UINT16 size;
        UINT16 id;
    };

    #pragma pack(pop)

    inline bool InvalidHeader( const INT32 readSize )
    {
        return MessageHeader::headerSize >= readSize;
    }

    struct MessageBase abstract : public ILogicTask
    {   
        virtual ~MessageBase() = default;

        virtual MessageBase* Clone() const = 0;
        virtual void Deserialize( const char* pBody ) = 0;
        virtual void Serialize( const char* pBody ) = 0;

        UINT16 size;
        UINT16 id;
    };

}
