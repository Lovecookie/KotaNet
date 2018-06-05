#pragma once
 
#include "SampleDefine.h"
namespace Kota
{
    class IPEndPoint
    {
    public:
        static const DWORD AddrLength = sizeof( SOCKADDR_IN ) + 16;

    public:        
        IPEndPoint();
        explicit IPEndPoint( const IPEndPoint& ep );
        explicit IPEndPoint( const std::string& ip, const UINT16 port );

        const SOCKADDR_IN& GetSockAddress() const
        {
            return _sockAddr;
        }

        SOCKADDR* CastAddress()
        {
            return reinterpret_cast<SOCKADDR*>(&_sockAddr);
        }

        const std::string& GetIP() const
        {
            return _ip;
        }

        UINT16 GetPort() const
        {
            return _port;
        }

    private:
        void _Create( const UINT16 family, const std::string& ip, const UINT16 port );

    private:
        SOCKADDR_IN _sockAddr;
        std::string _ip;
        UINT16 _port;
    };

}
