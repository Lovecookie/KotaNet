#pragma once

namespace Kota
{

	class TupleBuffer final
	{
	public:
		using Type = std::tuple<char*, ULONG, ULONG>; // buff, len, remained

	private:
		Type _tupleBuffer;

	public:
		TupleBuffer( char* pBuff, ULONG len );
		~TupleBuffer();

		bool IsRemained()
		{
			return (std::get<2>( _tupleBuffer ) > 0);
		}

		Type& GetTuple()
		{
			return _tupleBuffer;
		}

		void DecrementLength( DWORD alterValue )
		{
			std::get<2>( _tupleBuffer ) -= alterValue;
		}
		
		void MakeBuff( WSABUF& outBuff );
		void MakeRemainedBuff( WSABUF& outBuff );
		void ReleaseBuff();
	};
}