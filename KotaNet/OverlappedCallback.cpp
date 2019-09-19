#include "stdafx.h"
#include "OverlappedCallback.h"

namespace Kota
{
    OverlappedCallback::OverlappedCallback()
    {
        memset( static_cast<LPOVERLAPPED>(this), 0, sizeof( OVERLAPPED ) );
    }

    /*OverlappedCallback::OverlappedCallback( OverlappedFunc&& func ) : _callback( func )
    {   
    }*/

    bool OverlappedCallback::DoWork( const DWORD bytesTransferred )
    {
        return nullptr != _callback ? _callback( bytesTransferred ) : false;
    }
}
