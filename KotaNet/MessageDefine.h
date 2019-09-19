#pragma once

#include "MessageHeader.h"
#include "CppSerializer.h"

namespace Kota
{
	enum class EMsgID : UINT16
	{
		MsgChat = 1,
	};

    struct MsgChat final : MessageBase
    {
		MsgChat()
			: MessageBase(static_cast<UINT16>(EMsgID::MsgChat))
			, Msg { 0 }
		{	
		}

        MessageBase* Clone() const override
        {
		      return new MsgChat;
        }

        void Deserialize( const char* pBody ) override
        {
            CppSerializer serializer;
            auto pMsg = serializer.Deserialize<MsgChat>( pBody );
            
            memcpy( Msg.data(), pMsg->Msg.data(), sizeof( Msg ) );
        }

        void Serialize( OUT char* pBody ) override
        {
            CppSerializer serializer;
            serializer.Serialize<MsgChat>( this, pBody );
        }

        std::array<char, 128> Msg;
    };
}
