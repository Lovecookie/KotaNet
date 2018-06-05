#pragma once

#include "SampleDefine.h"
#include "OverlappedCallback.h"
#include "IPEndPoint.h"

namespace Kota
{
    __interface ISessionObject
    {
        HANDLE GetHandle();
        void OnError();
    };

    class Session : public ISessionObject
    {
    public:
        Session();
        virtual ~Session();

        SOCKET GetSocket() const
        {
            return _socket;
        }

        HANDLE GetHandle() override
        {
            return reinterpret_cast<HANDLE>(_socket);
        }

        void OnError() override
        {
        }
        
        bool Accept( const Session& listenSession );
        bool Create();
        void Close();
        bool Connect();
        bool Send( std::tuple<char*, ULONG, ULONG>& buff );
        bool Recv( char* buf, ULONG len );

    protected:
        virtual SOCKET _CreateSocket();
        bool _OnAccept( const DWORD bytesTransferred );
        bool _OnConnect( const DWORD bytesTransferred );
        bool _OnSend( const DWORD bytesTransferred );
        bool _OnRecv( const DWORD bytesTransferred );
        bool _OnDisconnect( const DWORD bytesTransferred );

    protected:
        SOCKET _socket = INVALID_SOCKET;
        IPEndPoint _endPoint;
        std::queue<std::tuple<char*, ULONG, ULONG>> _sendQueue;
        std::array<char, 64> _addrBuff;
        std::array<char, 1024> _recvBuff;
        bool _isZeroByte = true;

        OverlappedCallback _accept;
        OverlappedCallback _connect;
        OverlappedCallback _send;
        OverlappedCallback _recv;
        OverlappedCallback _disconnect;
    };

}
