﻿#pragma once

#include "SampleDefine.h"
#include "Iocp.h"

namespace Kota
{
    class Acceptor;
    class MessageLogicService;

    /*
     * Provider의 역할을 변경할 예정
     */
    class Provider final 
    {
    public:
        Provider();
        ~Provider();

        bool IsRunning()
        {
            return _isRunning;
        }

        bool NetworkStart();
        void NetworkClose();
        void ReadyAccept();

        void Processing();

	private:

		void _PacketInitialize();

    private:
        std::shared_ptr<Acceptor> _pAcceptor;
        std::shared_ptr<MessageLogicService> _pMessageLogicService;
        Iocp _iocp;
        std::atomic_uint32_t _connectedCount;
        bool _isRunning;
    };
}