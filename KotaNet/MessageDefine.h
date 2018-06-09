#pragma once
 
#include "MessageHeader.h"
#include "CppSerializer.h"

namespace Kota
{   
    struct MsgChat final : MessageBase
	{
		virtual MessageBase* Clone() const override
		{
			return new MsgChat;
		}

        void Deserialize( const char* pBody ) override
        {
			CppSerializer serializer;
            auto pMsg = serializer.Deserialize<MsgChat>( pBody );

			_Move( pMsg );
        }

        void Serialize( const char* pBody ) override
        {   
			CppSerializer serializer;
			serializer.Serialize<MsgChat>( this, pBody );
        }

        std::array<char, 128> message;

	private:
		void _Move( const MsgChat* msg )
		{
			memcpy( message.data(), msg->message.data(), sizeof( message ) );
		}
    };

    
}