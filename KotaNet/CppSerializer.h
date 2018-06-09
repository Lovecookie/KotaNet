#pragma once
#include "ISerializer.h"

namespace Kota
{
    class CppSerializer final
    {
    public:
        ~CppSerializer() = default;        
        
		template< typename T >
        void Serialize( T* type, const char* data ) const override
        {   
			::memcpy( data, type, sizeof( T ) );
        }

		template< typename T >
        T* Deserialize( const char* data ) const override
        {
			return reinterpret_cast<T*>( data );
        }

    private:
        
    };
}
