#pragma once

#include "MessageHeader.h"
#include "CppSerializer.h"

namespace Kota
{
    struct MsgChat final : MessageBase
    {
        MessageBase* Clone() const override
        {
            return new MsgChat;
        }

        void Deserialize( const char* pBody ) override
        {
            CppSerializer serializer;
            auto pMsg = serializer.Deserialize<MsgChat>( pBody );
            
            memcpy( message.data(), pMsg->message.data(), sizeof( message ) );
        }

        void Serialize( const char* pBody ) override
        {
            CppSerializer serializer;
            serializer.Serialize<MsgChat>( this, pBody );
        }

        std::array<char, 128> message;
    };
}
