#pragma once

#include "SampleDefine.h"


namespace Kota
{	
	__interface ILogicTask;

	__interface IService
    {   
        bool Initialize();
        void Finalize();
    };
    
    struct MessageHeader;
    struct MessageBase;

    class PacketLogicService final : public IService
    {
    public:
        using OnPacketFunc = std::function<void( const MessageBase* const )>;
        using ValueType = std::tuple<const MessageBase* const, OnPacketFunc>;

    public:
        bool Initialize() override;
        void Finalize() override;

		void AddLogicTask( const ILogicTask* const pTask );
		void AddPacket( const MessageBase* const pBase );

        MessageBase* Clone( const MessageHeader* const pBase );		

    private:
        std::unordered_map<UINT16, ValueType> _messageMap;
		std::queue<ILogicTask*> _taskQueue;
    };

}
