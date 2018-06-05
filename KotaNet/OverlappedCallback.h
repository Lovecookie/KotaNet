#pragma once
 
#include "SampleDefine.h"

namespace Kota
{
    using OverlappedFunc = std::function<bool( const DWORD )>;

    class OverlappedCallback final : public OVERLAPPED
    {
    public:
        OverlappedCallback();
        explicit OverlappedCallback( OverlappedFunc&& func );
        ~OverlappedCallback() = default;

        void Bind( OverlappedFunc&& func );
        bool DoWork( const DWORD bytesTransferred );

    private:
        OverlappedFunc _callback;
    };
}