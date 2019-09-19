#include "stdafx.h"
#include "ClientMessageProcessor.h"
#include "MessageDefine.h"

#include <iostream>

namespace Kota
{
	bool ChatMsgProcessor::DoWork( const MessageBase* const pMsg ) const
	{
		const auto pMessage = static_cast<const MsgChat*>( pMsg );
				
		std::cout << pMessage->Msg.data() << std::endl;

		return true;
	}
}