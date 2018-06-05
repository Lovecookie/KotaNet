#include "Session.h"


namespace Kota
{
    Session::Session()
        :   _socket( INVALID_SOCKET )
    {
        _accept.Bind( [this]( const DWORD bytes )
        {
            return _OnAccept( bytes );
        } );

        _connect.Bind( [this]( const DWORD bytes )
        {
            return _OnConnect( bytes );
        } );

        _recv.Bind( [this]( const DWORD bytes )
        {
            return _OnRecv( bytes );
        } );

        _send.Bind( [this]( const DWORD bytes )
        {
            return _OnSend( bytes );
        } );

        _disconnect.Bind( [this]( const DWORD bytes )
        {
            return _OnDisconnect( bytes );
        } );
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

        const auto result = AcceptEx( listenSession.GetSocket(), _socket, _addrBuff.data(), 0, IPEndPoint::AddrLength, IPEndPoint::AddrLength, nullptr, &_accept );
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

    void Session::Close()
    {
        if( INVALID_SOCKET != _socket )
        {
            shutdown( _socket, SD_SEND );
        }
    }

    bool Session::Connect()
    {
        // 구현 필요
        return false;
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

        u_long enableNonBlocking = 1;
        ioctlsocket( socket, FIONBIO, &enableNonBlocking );

        int noDelay = 1;
        ::setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, std::to_string( noDelay ).c_str(), sizeof( noDelay ) );

        return socket;
    }

    bool Session::_OnAccept( const DWORD bytesTransferred )
    {
        SOCKADDR_IN* remoteAddr = nullptr;
        SOCKADDR_IN* localAddr = nullptr;

        int localLength = sizeof( SOCKADDR_IN );
        int remoteLength = sizeof( SOCKADDR_IN );

        GetAcceptExSockaddrs( _addrBuff.data(), 0, IPEndPoint::AddrLength, IPEndPoint::AddrLength, reinterpret_cast<SOCKADDR**>(&localAddr), &localLength,
                              reinterpret_cast<SOCKADDR**>(&remoteAddr), &remoteLength );
        
        if( !Recv( nullptr, 0 ) )
        {
            return false;
        }

        _isZeroByte = true;

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
        DWORD readBytes = 0;

        do
        {   
            const auto result = ::recv( _socket, _recvBuff.data() + readBytes, 1024, 0 );

            if( SOCKET_ERROR == result )
            {
                if( WSAEWOULDBLOCK == result )
                {
                    break;
                }

                return false;
            }

            if( 0 == result )
            {
                return false;
            }

            readBytes = bytesTransferred;
        } while( bytesTransferred > readBytes );

        
        if( !Recv(nullptr, 0 ) )
        {
            return false;
        }

        return true;
    }

    bool Session::_OnDisconnect( const DWORD bytesTransferred )
    {
        return true;
    }
}
