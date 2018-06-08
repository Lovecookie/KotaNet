#pragma once
 
#include "SampleDefine.h"

namespace Kota
{
    using OverlappedFunc = std::function<bool( const DWORD )>;

    class OverlappedCallback final : public OVERLAPPED
    {
    public:
        OverlappedCallback();        
        explicit OverlappedCallback( OverlappedFunc&& func ) : _callback( std::move(func) )
        {
            memset( static_cast<LPOVERLAPPED>(this), 0, sizeof( OVERLAPPED ) );
        }

        ~OverlappedCallback() = default;

	template<typename Fn>
	void Bind( Fn&& func )
	{
            _callback = func;
	}
        
        bool DoWork( const DWORD bytesTransferred );

    private:
        OverlappedFunc _callback;
    };
}