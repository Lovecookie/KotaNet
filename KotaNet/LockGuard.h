#pragma once

#include <mutex>

namespace Kota
{
    namespace Utility
    {
        template< class Mutex, class Fn, class... Args >
        inline auto UniqueLock( Mutex& mt, Fn&& fn, Args&&... args )
        {
            const std::unique_lock<Mutex> lock( mt );
            return fn( args... );
        }
    }
}
