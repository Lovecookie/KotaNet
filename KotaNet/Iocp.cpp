#include "stdafx.h"
#include "Iocp.h"
#include "Session.h"
#include "OverlappedCallback.h"

namespace Kota
{
    Iocp::~Iocp()
    {
        Finalize();
    }

    bool Iocp::Initialize( const UINT32 numberOfWorker, const UINT32 timeout )
    {   
        _iocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, numberOfWorker );
        if( INVALID_HANDLE_VALUE == _iocp )
        {
            return false;
        }

        _timeout = timeout;
        
        for( UINT32 i = 0; numberOfWorker > i; ++i )
        {
            _workers.emplace_back( std::thread( std::bind( &Iocp::ProcessIO, this ) ) );
        }
        
        return true;
    }

    void Iocp::Finalize()
    {
        const auto begin = _workers.begin();
        const auto end = _workers.end();
        
        for( auto iter = begin; iter != end; ++iter )
        {
            PostQueuedCompletionStatus( _iocp, 0, NULL, nullptr );
        }

        for( auto iter = begin; iter != end; ++iter )
        {
            if( iter->joinable() )
                iter->join();
        }

        if( INVALID_HANDLE_VALUE != _iocp )
        {
            CloseHandle( _iocp );
            _iocp = INVALID_HANDLE_VALUE;
        }
    }

    bool Iocp::Associate( ISessionObject* const pObject )
    {
        const auto result = CreateIoCompletionPort( pObject->GetHandle(), _iocp, reinterpret_cast<ULONG_PTR>(pObject), 0 );
        return result == _iocp;
    }

    void Iocp::ProcessIO()
    {
        const auto currentWorkerId = ++_nextWorkerID;
        DWORD byteTransferred = 0;
        ULONG_PTR completionKey = 0;
        LPOVERLAPPED pOverlapped = nullptr;

        while( true )
        {
            byteTransferred = 0;
            completionKey = 0;
            pOverlapped = nullptr;

            const auto result = GetQueuedCompletionStatus( _iocp, &byteTransferred, &completionKey, &pOverlapped, _timeout );
            if( FALSE == result )
            {
                const auto lastError = ::GetLastError();

                if( WAIT_TIMEOUT != lastError )
                {
                    //logging...
                    continue;
                }

                // logging..
                _HandleTimeOut( currentWorkerId );
            }
            else
            {
                const auto pSession = reinterpret_cast<ISessionObject*>(completionKey);
                if( !reinterpret_cast<OverlappedCallback*>(pOverlapped)->DoWork( byteTransferred ) )
                {
                    pSession->OnError();
                }
            }
        }
    }

    void Iocp::_HandleTimeOut( const UINT32 workerID )
    {   
    }
}
