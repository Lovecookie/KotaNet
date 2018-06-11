#pragma once
#include <string>

namespace Kota
{
    class Console final
    {
    public:        
        static void Output( const char* str );
        static void Output( std::string& str );
        static void Output( const wchar_t* str );
    };


}
