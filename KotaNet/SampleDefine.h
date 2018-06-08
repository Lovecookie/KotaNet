#pragma once

#include <stdint.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>
#include <functional>
#include <queue>
#include <MSWSock.h>
#include <string>
#include <array>
#include <atomic>
#include <thread>
#include <chrono>
#include <list>
#include <unordered_map>
#include <tuple>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")


namespace Kota
{
    //  {{CONSTANT}}

    //  {{CONSTANT}}


    template< typename T >
    inline void SafeDelete( T*& type )
    {
        if( nullptr != type )
        {
            delete type;
            type = nullptr;
        }
    }

    template< typename T >
    inline void SafeDeleteArray( T*& type )
    {
        if( nullptr != type )
        {
            delete[] type;
            type = nullptr;
        }
    }

}