#pragma once

#include <mutex>

namespace Kota
{
    namespace Utility
    {
        /*template< class Fn, class... Args >
        inline auto SharedLock( Fn&& fn, Args&&... args )
        {
            const std::sha
            return fn( args... );
        }*/

        template< class Mutex, class Fn, class... Args >
        inline auto UniqueLock( Mutex& mt, Fn&& fn, Args&&... args )
        {
            const std::unique_lock<Mutex> lock( mt );
            return fn( args... );
        }
    }
}
