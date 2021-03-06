﻿#pragma once

#include "SampleDefine.h"

namespace Kota
{
    using OverlappedFunc = std::function<bool( const DWORD )>;

    class OverlappedCallback final : public OVERLAPPED
    {
    public:
        OverlappedCallback();
		OverlappedCallback( OverlappedFunc&& func );

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