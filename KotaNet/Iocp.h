#pragma once

#include "SampleDefine.h"

namespace Kota
{
    __interface ISessionObject;
	class ThreadGroup;

    class Iocp
    {
    public:
        Iocp();
        virtual ~Iocp();

        bool Initialize( const UINT32 workerCount, const UINT32 timeout );
        void Finalize();
        bool Associate( ISessionObject* const pObject );
        void ProcessIO();
        virtual void _HandleTimeOut( const UINT32 workerID );        

    private:
        HANDLE _iocp = INVALID_HANDLE_VALUE;
        std::atomic_uint32_t _nextWorkerID = 0;
        DWORD _timeout = INFINITE;        
		std::unique_ptr<ThreadGroup> _pWorkerGroup;
    };
}