#include "ClientMessageProcessor.h"
#include "MessageDefine.h"

namespace Kota
{
	bool ChatMsgProcessor::DoWork( MessageBase* const pMsg ) const
	{
		const auto pMessage = static_cast<MsgChat*>( pMsg );

		// logic...

		return true;
	}
}