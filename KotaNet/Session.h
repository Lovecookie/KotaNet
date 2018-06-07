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
    private:
        /*
         * 임시 상수
         */
        static const INT32 ReceiveSize = 2048;
        static const INT32 SendSize = 1024;

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
        bool Disconnect();
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
        std::array<char, 64> _addrBuff;
        std::array<char, ReceiveSize> _recvBuff;
        IPEndPoint _remoteEndPoint;
        UINT16 _remainedBytes = 0;
        SOCKET _socket = INVALID_SOCKET;
        std::queue<std::tuple<char*, ULONG, ULONG>> _sendQueue;
        bool _isZeroByte = true;

    private:
        OverlappedCallback _accept;
        OverlappedCallback _connect;
        OverlappedCallback _send;
        OverlappedCallback _recv;
        OverlappedCallback _disconnect;
    };

}
