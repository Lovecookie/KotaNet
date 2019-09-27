#pragma once

#include "SampleDefine.h"

namespace Kota
{
    class NetAPI final
    {
    public:
        NetAPI();
        ~NetAPI();

        static BOOL Disconnect( SOCKET socket, LPOVERLAPPED overlapped );

        static bool Linger( SOCKET socket, bool enable, UINT16 time );
        static bool NonBlocking( SOCKET socket, bool enable );
        static bool NoDelay( SOCKET socket, bool enable );
        static bool ReuseAddr( SOCKET socket, bool enable );
    
        void CreateAPI();

    private:
        static LPFN_DISCONNECTEX _fnDisconnectEx;
        static LPFN_CONNECTEX _fnConnectEx;
    };
}
