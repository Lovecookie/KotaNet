#pragma once

#include "IMessageProcessor.h"


namespace Kota
{
	class ChatMsgProcessor final : public IMessageProcessor
	{
		bool DoWork( MessageBase* const pMsg ) const override;
	};
}