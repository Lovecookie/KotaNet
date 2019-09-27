#include "stdafx.h"
#include "Session.h"
#include "NetAPI.h"
#include "Console.h"
#include "MessageHeader.h"
#include "MessageLogicService.h"
#include "CppSerializer.h"

namespace Kota
{
	Session::Session( MessageLogicService* pService )
		: _pLogicService( pService )
		, _addrBuff{ 0 }
		, _recvBuff{ 0 }
		, _remainedBuff{ 0 }
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
		
		DWORD outByteReceived = 0;
		const auto result = AcceptEx( listenSession.GetSocket(), _socket, _addrBuff.data(), 0, IPEndPoint::AddrLength, IPEndPoint::AddrLength, &outByteReceived, &_accept );
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

	bool Session::Send( TupleBuffer& buffer )
	{
		if( _sendQueue.empty() )
		{
			WSABUF wsa;
			buffer.MakeBuff( wsa );			

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

		_sendQueue.push( buffer );

		return true;
	}

	bool Session::PostReceive()
	{
		WSABUF wsa;
		wsa.buf = nullptr;
		wsa.len = 0;

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

		NetAPI::NonBlocking( socket, true );
		NetAPI::NoDelay( socket, true );
		NetAPI::ReuseAddr( socket, true );

		return socket;
	}

	MessageBase* Session::_DeserializeMessage( const MessageHeader* pMsgBase, const char* pBody )
	{
		const auto pMessageBase = _pLogicService->Clone( pMsgBase );
		if( nullptr == pMessageBase )
		{
			return nullptr;
		}

		try
		{
			pMessageBase->Deserialize( pBody );
		}
		catch( std::exception e )
		{
			Console::Output( e.what() );
			return nullptr;
		}

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

		if( PostReceive() )
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
		auto buffer = _sendQueue.front();
		buffer.DecrementLength( bytesTransferred );

		if( buffer.IsRemained() )
		{
			WSABUF wsa;
			buffer.MakeRemainedBuff( wsa );

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
			buff.ReleaseBuff();
		}

		if( _sendQueue.empty() )
		{
			_sendQueue.pop();
			return false;
		}

		if( !_sendQueue.empty() )
		{
			auto nextBuffer = _sendQueue.front();

			WSABUF wsa;
			nextBuffer.MakeBuff( wsa );

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
		if( nullptr == _pLogicService )
		{
			Disconnect();
			return false;
		}

		INT16 remainedMarker = 0;

		do
		{
			const auto curRecvSize = ::recv( _socket, _remainedBuff.data() + _readBytes, ReceiveSize - _readBytes, 0 );
			if( SOCKET_ERROR == curRecvSize )
			{
				if( WSAEWOULDBLOCK != GetLastError() )
				{
					Console::Output( L"Session::_OnRecv() is error" );
					Disconnect();
					return false;
				}

				return PostReceive();
			}

			_readBytes += curRecvSize;
			if( InvalidHeader( _readBytes ) )
			{
				continue;
			}

			const auto pMsgHeader = reinterpret_cast<MessageHeader*>(_remainedBuff.data());
			if( pMsgHeader->IsInvalid( _readBytes ) )
			{
				continue;
			}

			remainedMarker = pMsgHeader->size;
			const auto pBody = _remainedBuff.data() + MessageHeader::headerSize;

			const auto pMessageBase = _DeserializeMessage( pMsgHeader, pBody );
			if( nullptr == pMessageBase )
			{
				Console::Output( L"Session::_OnRecv() messageBase is null" );
				Disconnect();
				return false;
			}

			_pLogicService->PushTask( pMessageBase );

			break;
		}
		while( true );

		_readBytes -= remainedMarker;
		if( _readBytes > 0 )
		{
			memmove_s( _remainedBuff.data(), ReceiveSize, _remainedBuff.data() + remainedMarker, _readBytes );
		}
		else
		{
			_readBytes = 0;
		}

		return PostReceive();
	}

	bool Session::_OnDisconnect( const DWORD bytesTransferred )
	{
		return Close();
	}


}
