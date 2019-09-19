#include "stdafx.h"
#include "Console.h"
#include <iostream>

namespace Kota
{
    void Console::Output( const char* str)
    {   
        std::cout << str << std::endl;
    }

    void Console::Output( const std::string& str )
    {
        std::cout << str << std::endl;
    }

    void Console::Output( const wchar_t* str )
    {
        std::wcout << str << std::endl;
    }
}
