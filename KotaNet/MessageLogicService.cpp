#include "stdafx.h"
#include "MessageLogicService.h"
#include "MessageHeader.h"
#include "LockGuard.h"
#include "IMessageProcessor.h"

namespace Kota
{
    bool MessageLogicService::Initialize()
    {
        _isProgress = true;

		_thread = std::thread( &MessageLogicService::Run, this );

        return true;
    }

    void MessageLogicService::Finalize()
    {
		_isProgress = false;
		_thread.join();

        for( auto iter : _messageMap )
        {
            const auto pBase = std::get<0>( iter.second );
			const auto pProcessor = std::get<1>( iter.second );

            if( nullptr != pBase )
            {
                delete pBase;
            }

			if( nullptr != pProcessor )
			{
				delete pProcessor;
			}
        }

        _messageMap.clear();
    }

    void MessageLogicService::AddPacket( const MessageBase* const pBase, const IMessageProcessor* const pProcessor )
    {   
        _messageMap.emplace( std::make_pair( pBase->id,std::make_tuple( pBase, pProcessor ) ) );
    }

    MessageBase* MessageLogicService::Clone( const MessageHeader* const pBase )
    {
        const auto iter = _messageMap.find( pBase->id );
        if( _messageMap.end() == iter )
        {
            return nullptr;
        }

        const auto pMessage = std::get<0>( iter->second )->Clone();

        pMessage->size = pBase->size;
        pMessage->id = pBase->id;

        return pMessage;
    }

	void MessageLogicService::PushTask( ILogicTask* pTask )
	{
		if( _isProgress )
		{
			delete pTask;
			return;
		}

		Utility::UniqueLock( _mutex, [this, pTask]()
		{
			_logicQueue.emplace( pTask );
		} );
	}

	const IMessageProcessor* MessageLogicService::FindMessageProcessor( UINT16 id )
	{
		const auto iter = _messageMap.find( id );
		if( _messageMap.end() == iter )
		{
			return nullptr;
		}

		return std::get<1>( iter->second );
	}

    void MessageLogicService::Run()
    {
        while( _isProgress )
        {   
            if( _logicQueue.empty() )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            }
            else
            {            
                TaskQueue taskQueue;
                Utility::UniqueLock( _mutex, std::bind( &TaskQueue::swap, std::ref( _logicQueue ), std::ref( taskQueue ) ) );

                while( !taskQueue.empty() )
                {
                    const auto pTask = taskQueue.front();
                    taskQueue.pop();

					pTask->Process( this );

                    delete pTask;
                }
            }
        }
    }

}
