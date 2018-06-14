#include "PacketLogicService.h"
#include "MessageHeader.h"
#include "LockGuard.h"

namespace Kota
{
    bool PacketLogicService::Initialize()
    {
        _isProgress = true;

        return true;
    }

    void PacketLogicService::Finalize()
    {
        for( auto iter : _messageMap )
        {
            const auto pBase = std::get<0>( iter.second );

            if( nullptr != pBase )
            {
                delete pBase;
            }
        }

        _messageMap.clear();
    }

    void PacketLogicService::AddPacket( const MessageBase* const pBase, OnPacketFunc&& func )
    {   
        _messageMap.emplace( std::make_pair( pBase->id,std::make_tuple( pBase, func ) ) );
    }

    void PacketLogicService::EmplaceTask( ILogicTask* pTask )
    {
        if ( _isProgress )
        {
            delete pTask;
            return;
        }

        Utility::UniqueLock( _mutex, [this, pTask]()
        {
            _logicQueue.emplace( pTask );
        } );        
    }

    MessageBase* PacketLogicService::Clone( const MessageHeader* const pBase )
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

    void PacketLogicService::Run()
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
                    
                    delete pTask;
                }
            }
        }
    }

}
