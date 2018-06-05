#include "NetAPI.h"
#include <iostream>
#include "Console.h"

namespace Kota
{
    LPFN_DISCONNECTEX NetAPI::_fnDisconnectEx = nullptr;
    LPFN_CONNECTEX NetAPI::_fnConnectEx = nullptr;

    NetAPI::NetAPI()
    {
        _CreateAPI();
    }

    NetAPI::~NetAPI()
    {
    }

    bool NetAPI::Disconnect( SOCKET socket, LPOVERLAPPED overlapped )
    {   
        const auto result = _fnDisconnectEx( socket, overlapped, TF_REUSE_SOCKET, 0 );
        if( FALSE == result )
        {
            if( WSAGetLastError() != WSA_IO_PENDING )
            {
                Console::Output( L"NetAPI::Disconnect() is failed.." );
                return false;
            }
        }

        return true;
    }

    bool NetAPI::NonBlocking( SOCKET socket, BOOL enable )
    {
        ULONG enableNonBlocking = enable;
        return SOCKET_ERROR != ioctlsocket( socket, FIONBIO, &enableNonBlocking ); 
    }
    
    bool NetAPI::NoDelay( SOCKET socket, BOOL enable )
    {
        INT32 noDelay = enable;
        return SOCKET_ERROR != ::setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&noDelay), sizeof( noDelay ) );
    }

    bool NetAPI::ReuseAddr( SOCKET socket, BOOL enable )
    {
        INT32 reuseAddr = enable;
        return SOCKET_ERROR != ::setsockopt( socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddr), sizeof( reuseAddr ) );
    }

    void NetAPI::_CreateAPI()
    {
        const SOCKET socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED );
        if( INVALID_SOCKET == socket )
        {   
            Console::Output( L"SocketAPI::_CreateAPI() socket create error" );
            return;
        }

        DWORD bytes = 0;

        if( nullptr == _fnDisconnectEx )
        {
            GUID guidDisconnect = WSAID_DISCONNECTEX;
            if( SOCKET_ERROR == WSAIoctl( socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidDisconnect, sizeof(GUID),
                                            &_fnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, nullptr, nullptr ) )
            {
                Console::Output( L"NetAPI::_CreateAPI() is failed. _fnDisconnectEx" );
                return;
            }
        }

        if( nullptr == _fnConnectEx )
        {
            GUID guidConnect = WSAID_CONNECTEX;

            if( SOCKET_ERROR == WSAIoctl( socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidConnect, sizeof(GUID),
                                          &_fnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, nullptr, nullptr) )
            {
                Console::Output( L"NetAPI::_CreateAPI() is failed. _fnConnectEx" );
                return;
            }
        }
    }



}
