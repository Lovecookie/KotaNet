#include "stdafx.h"
#include "Provider.h"
#include "Acceptor.h"
#include "MessageLogicService.h"
#include "MessageDefine.h"
#include "ClientMessageProcessor.h"
#include "Console.h"

namespace Kota
{
	Provider::Provider()
		: _isRunning(false)
	{
	}

    Provider::~Provider()
    {
    }

    bool Provider::NetworkStart()
    {   
		_PacketInitialize();

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
            pSession = new Session( _pMessageLogicService.get() );
            pSession->Create();
            _iocp.Associate( pSession );

            _pAcceptor->Accept( pSession );
        }
    }

    void Provider::Processing()
    {

		Console::Output( L"Waiting Connection user.." );
		
        while( _isRunning )
        {   
            std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        }
    }
	
	void Provider::_PacketInitialize()
	{
		_pMessageLogicService = std::make_shared<MessageLogicService>();
		_pMessageLogicService->AddPacket( new MsgChat(), new ChatMsgProcessor() );
	}
}
