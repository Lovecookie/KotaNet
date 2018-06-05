#include "IPEndPoint.h"


namespace Kota
{
    IPEndPoint::IPEndPoint()
        :   _ip(""),
            _port(0)
    {   
        _Create( AF_INET, _ip, _port );
    }

    IPEndPoint::IPEndPoint( const IPEndPoint& ep )
        :   _ip(ep._ip),
            _port(ep._port)
    {
        _Create( AF_INET, _ip, _port );
    }

    IPEndPoint::IPEndPoint( const std::string& ip, const UINT16 port )
        :   _ip(ip),
            _port(port)
    {   
        _Create( AF_INET, _ip, _port );
    }

    void IPEndPoint::_Create( const UINT16 family, const std::string& ip, const UINT16 port )
    {
        memset( &_sockAddr, 0, sizeof( SOCKADDR_IN ) );

        _sockAddr.sin_family = family;
        inet_pton( family, ip.c_str(), reinterpret_cast<void*>(&_sockAddr.sin_addr.s_addr) );
        _sockAddr.sin_port = htons( port );
    }
}
