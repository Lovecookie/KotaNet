#pragma once

#include "SampleDefine.h"

namespace Kota
{
    class SocketAPI final
    {
    public:
        SocketAPI();
        ~SocketAPI();            

    private:
        void _CreateAPI();

    private:
        LPFN_DISCONNECTEX fnDisconnectEx;
    };
}
