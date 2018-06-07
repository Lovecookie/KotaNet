#pragma once

namespace Kota
{
    __interface ISerializer
    {
        bool Serialize( const char* data );
        bool Deserialize( const char* data );
    };
}
