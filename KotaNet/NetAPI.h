#pragma once

#include "SampleDefine.h"

namespace Kota
{
    class NetAPI final
    {
    public:
        NetAPI();
        ~NetAPI();

        static bool Disconnect( SOCKET socket, LPOVERLAPPED overlapped );

        static bool NonBlocking( SOCKET socket, BOOL enable );
        static bool NoDelay( SOCKET socket, BOOL enable );
        static bool ReuseAddr( SOCKET socket, BOOL enable );

    private:
        void _CreateAPI();

    private:
        static LPFN_DISCONNECTEX _fnDisconnectEx;
        static LPFN_CONNECTEX _fnConnectEx;
    };
}
