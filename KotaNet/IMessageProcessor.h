#pragma once

namespace Kota
{
	struct MessageBase;

	__interface IMessageProcessor
	{
		bool DoWork( MessageBase* const pMsg ) const;
	};
}
