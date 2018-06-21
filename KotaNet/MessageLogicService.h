#pragma once

#include "IService.h"

namespace Kota
{	
    __interface ILogicTask;
	__interface IMessageProcessor;
    struct MessageHeader;
    struct MessageBase;


    class MessageLogicService final : public IService
    {
    public:
        using OnPacketFunc = std::function<void( MessageBase* const )>;		
        using ValueType = std::tuple<const MessageBase* const, const IMessageProcessor* const>;
        using TaskQueue = std::queue<ILogicTask*>;

    public:
        bool Initialize() override;
        void Finalize() override;

        void AddPacket( const MessageBase* const pBase, const IMessageProcessor* const pProcessor );
        MessageBase* Clone( const MessageHeader* const pBase );
        void PushTask( ILogicTask* pTask );
		const IMessageProcessor* FindMessageProcessor( UINT16 id );
        
        void Run();

    private:
        std::unordered_map<UINT16, ValueType> _messageMap;
        TaskQueue _logicQueue;
        std::mutex _mutex;
        
        std::thread _thread;
        bool _isProgress = false;
    };

}
