#pragma once

#include "IService.h"

namespace Kota
{	
    __interface ILogicTask;
    struct MessageHeader;
    struct MessageBase;


    class PacketLogicService final : public IService
    {
    public:
        using OnPacketFunc = std::function<void( MessageBase* const )>;
        using ValueType = std::tuple<const MessageBase* const, OnPacketFunc>;
        using TaskQueue = std::queue<ILogicTask*>;

    public:
        bool Initialize() override;
        void Finalize() override;

        void AddPacket( const MessageBase* const pBase, OnPacketFunc&& func );
        MessageBase* Clone( const MessageHeader* const pBase );
        void EmplaceTask( ILogicTask* pTask );
        
        void Run();

    private:
        std::unordered_map<UINT16, ValueType> _messageMap;
        TaskQueue _logicQueue;
        std::mutex _mutex;
        
        std::thread _thread;
        bool _isProgress = false;
    };

}
