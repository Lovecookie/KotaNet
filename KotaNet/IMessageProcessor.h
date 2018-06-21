#pragma once

namespace Kota
{
	struct MessageBase;

	__interface IMessageProcessor
	{
		bool DoWork( const MessageBase* const pMsg ) const;
	};
}
