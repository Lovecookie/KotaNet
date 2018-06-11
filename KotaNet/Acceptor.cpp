#include "Acceptor.h"
#include "Session.h"

namespace Kota
{   
    Acceptor::Acceptor( const IPEndPoint& endPoint )
        :   _endPoint( endPoint ),
            _session( nullptr )
    {   
    }

    bool Acceptor::Accept( Session* const pSession )
    {
        return pSession->Accept( _session );
    }

    bool Acceptor::Bind()
    {
        const auto result = bind( _session.GetSocket(), _endPoint.CastAddress(), sizeof( SOCKADDR_IN ) );
        if( SOCKET_ERROR == result )
        {
            // error
            return false;
        }

        return true;
    }

    bool Acceptor::CreateTCP()
    {
        return _session.Create();
    }

    bool Acceptor::Listen( INT32 backlog )
    {
        const auto result = listen( _session.GetSocket(), backlog );
        if( SOCKET_ERROR == result )
        {
            // error
            return false;
        }

        return true;
    }
}
