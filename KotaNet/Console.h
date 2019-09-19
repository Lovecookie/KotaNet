#pragma once


namespace Kota
{
    class Console final
    {
    public:        
        static void Output( const char* str );
        static void Output( const std::string& str );
        static void Output( const wchar_t* str );

		template<typename ...Args>
		static void _MakeLog( const std::string_view format,  const Args&... numeric )
		{
		}

		//template<typename TCategory, typename ...Args>
		//static void _MakeString( std::stringstream& outStream,const TCategory& category, const Args&... numeric)
		//{
		//	outStream << "[" << category << "] ";
		//}

    };
}
