#include "SocketAPI.h"
#include <iostream>
#include "Console.h"

namespace Kota
{       
    SocketAPI::SocketAPI()
    {
        _CreateAPI();
    }

    SocketAPI::~SocketAPI()
    {
    }

    void SocketAPI::_CreateAPI()
    {
        const SOCKET socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED );
        if( INVALID_SOCKET == socket )
        {   
            Console::Output( "SocketAPI::_CreateAPI() a socket create error" );
            return;
        }

        DWORD bytes = 0;

        if( nullptr == fnDisconnectEx )
        {
            GUID guidDisconnect = WSAID_DISCONNECTEX;
            if( SOCKET_ERROR == WSAIoctl( socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidDisconnect, sizeof(GUID),
                                            &fnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, nullptr, nullptr ) )
            {   
                return;
            }
        }
    }    
}
