#include "Provider.h"
#include "Acceptor.h"

namespace Kota
{
    Provider::~Provider()
    {
    }

    UINT32 Provider::GetConnectedCount()
    {
        return _connectedCount.load();
    }

    void Provider::IncreamentConnected()
    {
        _connectedCount.fetch_add( 1 );
    }

    void Provider::DecreamentConnected()
    {
        _connectedCount.fetch_sub( 1 );
    }

    bool Provider::NetworkStart()
    {   
        _iocp.Initialize( 1, INFINITE );

        IPEndPoint endPoint( "127.0.0.1", 50001 );
        endPoint.UseIP4();

        _pAcceptor = std::make_shared<Acceptor>( endPoint );
        _pAcceptor->CreateTCP();

        if( !_pAcceptor->Bind() )
        {
            return false;
        }

        if( !_pAcceptor->Listen( SOMAXCONN ) )
        {
            return false;
        }
        
        _iocp.Associate( _pAcceptor->GetSession() );

        ReadyAccept();

        _isRunning = true;

        return true;
    }

    void Provider::NetworkClose()
    {
        _isRunning = false;
    }

    void Provider::ReadyAccept()
    {
        Session* pSession = nullptr;
        OverlappedCallback* pCallback = nullptr;

        for( auto index=0; index < 20; ++index )
        {
            pSession = new Session();
            pSession->Create();
            _iocp.Associate( pSession );

            _pAcceptor->Accept( pSession );
        }
    }

    void Provider::Processing()
    {
        while( _isRunning )
        {   
            std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        }
    }


}
