#include "stdafx.h"
#include "IPEndPoint.h"
#include "Console.h"

namespace Kota
{
    IPEndPoint::IPEndPoint()
        :   _ip("127.0.0.1"),
            _port(0)
    {
		ZeroMemory( &_sockAddr, sizeof( SOCKADDR_IN ) );
    }

    IPEndPoint::IPEndPoint( const std::string& ip, const UINT16 port )
        : _ip( ip ),
        _port( port )
    {
		ZeroMemory( &_sockAddr, sizeof( SOCKADDR_IN ) );
    }

    IPEndPoint::IPEndPoint( const IPEndPoint& ep )
        :   _ip(ep._ip),
            _port(ep._port)
    {
        ::memcpy( &_sockAddr, &ep._sockAddr, sizeof( SOCKADDR_IN ) );		
    }

    void IPEndPoint::ConvertAddress( const LPSOCKADDR sockAddr )
    {
        if( AF_INET == sockAddr->sa_family )
        {
            ::memcpy( &_sockAddr, sockAddr, sizeof( SOCKADDR ) );

            std::array<char, INET_ADDRSTRLEN> ip;
            inet_ntop( AF_INET, &_sockAddr.sin_addr, ip.data(), INET_ADDRSTRLEN );

            _ip = ip.data();
            _port = ntohs( _sockAddr.sin_port );
        }
        else if( AF_INET6 == sockAddr->sa_family )
        {
            Console::Output( L"not support.." );
        }
    }

    void IPEndPoint::CreateIP4( const std::string& ip, const UINT16 port )
    {
        _CreateIP4( port, ip.c_str() );
    }

    void IPEndPoint::CreateIP4( const UINT16 port )
    {
        _CreateIP4( port );
    }

    void IPEndPoint::UseIP4()
    {
        _CreateIP4( _port );
    }

    void IPEndPoint::_CreateIP4( const UINT16 port, const char* ip )
    {   
        _port = port;
        _sockAddr.sin_family = AF_INET;
        _sockAddr.sin_port = htons( port );

        if( nullptr == ip )
        {   
            _sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
        }
        else
        {
            inet_pton( AF_INET, ip, &_sockAddr.sin_addr );       
        }        
    }

}
