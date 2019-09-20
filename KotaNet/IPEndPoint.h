#pragma once
 
#include "SampleDefine.h"
namespace Kota
{
    class IPEndPoint final
    {
    public:
        static const DWORD AddrLength = sizeof( SOCKADDR_IN ) + 16;        

    public:        
        IPEndPoint();
        explicit IPEndPoint( const std::string& ip, const UINT16 port );
        explicit IPEndPoint( const IPEndPoint& ep );

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
        
        void ConvertAddress( const LPSOCKADDR sockAddr );
        void CreateIP4( const std::string& ip, const UINT16 port );
        void CreateIP4( const UINT16 port );
        void UseIP4();

    private:
        void _CreateIP4( const UINT16 port, const char* ip = nullptr );

    private:
        SOCKADDR_IN _sockAddr;
        std::string _ip;
        UINT16 _port;
    };    

}
