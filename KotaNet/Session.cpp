#include "Session.h"
#include "NetAPI.h"
#include "Console.h"
#include "MessageBase.h"

namespace Kota
{   
    Session::Session()
    {
		_accept.Bind( std::bind( &Session::_OnAccept ) );
		_connect.Bind( std::bind( &Session::_OnConnect ) );
		_recv.Bind( std::bind( &Session::_OnConnect ) );
		_send.Bind( std::bind( &Session::_OnSend ) );
		_disconnect.Bind( std::bind( &Session::_OnDisconnect ) );		
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
            const auto result = ::recv( _socket, _recvBuff.data() + _remainedBytes, ReceiveSize, 0 );
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

            const auto msgBase = reinterpret_cast<MessageBase*>(_recvBuff.data());            
            if( msgBase->size > ReceiveSize )
            {
            }

            _remainedBytes = bytesTransferred;
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
