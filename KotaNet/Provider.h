#pragma once

#include "SampleDefine.h"
#include "Iocp.h"

namespace Kota
{
    class Acceptor;

    class Provider final 
    {
    public:
        Provider() = default;
        ~Provider();

        bool IsRunning()
        {
            return _isRunning;
        }       

        UINT32 GetConnectedCount();
        void IncreamentConnected();
        void DecreamentConnected();

        bool NetworkStart();
        void NetworkClose();
        void ReadyAccept();

        void Processing();

    private:
        std::shared_ptr<Acceptor> _pAcceptor;
        Iocp _iocp;
        std::atomic_uint32_t _connectedCount;
        bool _isRunning;
    };
}