#pragma once
#include "NetAPI.h"

namespace Kota
{
    class NetworkInitializer final
    {
    public:
        NetworkInitializer();
        ~NetworkInitializer();

        bool GetError();

    private:
        bool _isIntializerError = false;
        NetAPI _netApi;
    };
}
