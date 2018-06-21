#pragma once

namespace Kota
{
	class MessageLogicService;

	__interface ILogicTask
	{
		bool Process( MessageLogicService* const pLogicService );
	};
}
