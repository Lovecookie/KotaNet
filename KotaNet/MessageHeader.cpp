#include "MessageHeader.h"
#include "MessageLogicService.h"
#include "IMessageProcessor.h"
#include "Console.h"

namespace Kota
{	
	bool MessageBase::Process( MessageLogicService* const pLogicService )
	{
		const auto pProcessor = pLogicService->FindMessageProcessor( id );
		if( nullptr == pProcessor )
		{
			return false;
		}

		const auto result = pProcessor->DoWork( this );
		if( !result )
		{
			Console::Output( L"MessageBase::Process() is failed.." );
			return false;
		}

		return true;
	}

}