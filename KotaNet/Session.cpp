﻿#include "Session.h"
#include "NetAPI.h"
#include "Console.h"
#include "MessageHeader.h"
#include "PacketMakeService.h"
#include "CppSerializer.h"

namespace Kota
{   
    Session::Session( PacketMakeService* pService )
        :   _pMakeService( pService )
    {
        _accept.Bind( std::bind( &Session::_OnAccept, this, std::placeholders::_1 ) );
        _connect.Bind( std::bind( &Session::_OnConnect, this, std::placeholders::_1 ) );
        _recv.Bind( std::bind( &Session::_OnRecv, this, std::placeholders::_1 ) );
        _send.Bind( std::bind( &Session::_OnSend, this, std::placeholders::_1 ) );
        _disconnect.Bind( std::bind( &Session::_OnDisconnect, this, std::placeholders::_1 ) );        
    }

    Session::~Session()
    {
        Close();
    }

    bool Session::Accept( const Session& listenSession )
    {
        if( INVALID_SOCKET == _socket )
        {
            return false;
        }

        const auto result = AcceptEx( listenSession.GetSocket(), _socket, _addrBuff.data(), 0, IPEndPoint::AddrLength,
									  IPEndPoint::AddrLength, nullptr, &_accept );
        if( result == FALSE )
        {
            const auto error = WSAGetLastError();
            if( WSA_IO_PENDING != error )
            {
                return false;
            }
        }

        return true;
    }    

    bool Session::Create()
    {
        _socket = _CreateSocket();
        if( INVALID_SOCKET == _socket )
        {
            return false;
        }

        return true;
    }

    bool Session::Close()
    {  
	if( _socket == INVALID_SOCKET )
	{
		return false;			
	}

	shutdown( _socket, SD_BOTH );
	closesocket( _socket );
	_socket = INVALID_SOCKET;

	return true;
    }

    bool Session::Connect()
    {
        // 구현 필요
        return true;
    }

    bool Session::Disconnect()
    {
        const auto result = NetAPI::Disconnect( _socket, &_disconnect );        
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

    bool Session::Send( std::tuple<char*, ULONG, ULONG>& buff )
    {
        if( _sendQueue.empty() )
        {
            WSABUF wsa;
            wsa.buf = std::get<0>( buff );
            wsa.len = std::get<1>( buff );

            const auto result = WSASend( _socket, &wsa, 1, nullptr, 0, &_send, nullptr );
            if( SOCKET_ERROR == result )
            {
                const auto lastError = WSAGetLastError();
                if( WSA_IO_PENDING != lastError )
                {
                    //  logging..
                    return false;
                }
            }
        }

        _sendQueue.push( buff );

        return true;
    }

    bool Session::Recv( char* buf, ULONG len  )
    {
        WSABUF wsa;
        wsa.buf = buf;
        wsa.len = len;

        DWORD readBytes = 0;
        DWORD flags = 0;
        const auto result = WSARecv( _socket, &wsa, 1, &readBytes, &flags, &_recv, nullptr );
        if( SOCKET_ERROR == result )
        {
            const auto lastError = WSAGetLastError();
            if( WSA_IO_PENDING != lastError )
            {
                // logging..
                return false;
            }
        }

        return true;
    }

    SOCKET Session::_CreateSocket()
    {
        auto socket = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED );
        if( INVALID_SOCKET == socket )
        {
            return INVALID_SOCKET;
        }

        NetAPI::NonBlocking( socket, TRUE );
        NetAPI::NoDelay( socket, TRUE );
        NetAPI::ReuseAddr( socket, TRUE );

        return socket;
    }
    
    MessageBase* Session::_DismantlePacket( const MessageHeader* pMsgBase, const char* pBody )
    {   
        const auto pMessageBase = _pMakeService->Clone( pMsgBase );
        if( nullptr == pMessageBase )
        {
            return nullptr;
        }
        
        pMessageBase->Deserialize( pBody );

        return pMessageBase;
    }

    bool Session::_OnAccept( const DWORD bytesTransferred )
    {
        PSOCKADDR remoteAddr = nullptr;
        PSOCKADDR localAddr = nullptr;

        int addrSize = sizeof( SOCKADDR );

        GetAcceptExSockaddrs( _addrBuff.data(), 0, IPEndPoint::AddrLength, IPEndPoint::AddrLength, &localAddr,
                              &addrSize, &remoteAddr, &addrSize );

        _remoteEndPoint.ConvertAddress( remoteAddr );        
        
        if( !Recv( nullptr, 0 ) )
        {
            return false;
        }       

        return true;
    }

    bool Session::_OnConnect( const DWORD bytesTransferred )
    {
        return true;
    }

    bool Session::_OnSend( const DWORD bytesTransferred )
    {
        auto& buff = _sendQueue.front();
        std::get<2>( buff ) -= bytesTransferred;

        if( 0 < std::get<2>( buff ) )
        {
            WSABUF wsa;
            wsa.buf = std::get<0>( buff ) + (std::get<1>( buff ) - std::get<2>( buff ));
            wsa.len = std::get<2>( buff );

            const auto result = WSASend( _socket, &wsa, 1, nullptr, 0, &_send, nullptr );
            if( SOCKET_ERROR == result )
            {
                const auto lastError = WSAGetLastError();
                if( WSA_IO_PENDING != lastError )
                {
                    //  logging..
                    return false;
                }
            }
            return true;
        }
        else
        {
            delete std::get<0>( buff );
        }

        if( _sendQueue.empty() )
        {
            _sendQueue.pop();
            return false;
        }

        if( !_sendQueue.empty() )
        {
            const auto nextBuff = _sendQueue.front();

            WSABUF wsa;
            wsa.buf = std::get<0>( nextBuff );
            wsa.len = std::get<1>( nextBuff );

            const auto result = WSASend( _socket, &wsa, 1, nullptr, 0, &_send, nullptr );
            if( SOCKET_ERROR == result )
            {
                const auto lastError = WSAGetLastError();
                if( WSA_IO_PENDING != lastError )
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool Session::_OnRecv( const DWORD bytesTransferred )
    {
        do
        {
            const auto result = ::recv( _socket, _remainedBuff.data() + _readBytes, ReceiveSize, 0 );
            if( SOCKET_ERROR == result )
            {
                if( WSAEWOULDBLOCK != result )
                {
                    Console::Output( L"Session::_OnRecv() is error" );
                    Disconnect();

                    return false;
                }
                
                //Recv( , 0 );
                return true;
            }

            const auto pMsgHeader = reinterpret_cast<MessageHeader*>(_remainedBuff.data());
            if( pMsgHeader->size > ReceiveSize )
            {
                Console::Output( L"Session::_OnRecv() overflow receive size" );
                Disconnect();
                return false;
            }

            const auto pBody = _remainedBuff.data() + MessageHeader::headerSize;
            _readBytes = pMsgHeader->size;
            
            const auto pMessageBase = _DismantlePacket( pMsgHeader, pBody );

            
        } while( bytesTransferred > 4 );

        
        if( !Recv(nullptr, 0 ) )
        {
            return false;
        }

        return true;
    }

    bool Session::_OnDisconnect( const DWORD bytesTransferred )
    {
    	return Close();        
    }


}
