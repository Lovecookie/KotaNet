#include "PacketLogicService.h"
#include "MessageHeader.h"


namespace Kota
{
    bool PacketLogicService::Initialize()
    {
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

	void PacketLogicService::AddLogicTask( const ILogicTask* const pTask )
	{

	}

    void PacketLogicService::AddPacket( const MessageBase* const pBase )
    {
        // юс╫ц
        _messageMap.emplace( std::make_pair( pBase->id, std::make_tuple( pBase, []( const MessageBase* const p ) {} ) ) );
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


}