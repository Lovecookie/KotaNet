#include "stdafx.h"
#include "TupleBuffer.h"

namespace Kota
{

	TupleBuffer::TupleBuffer( char* pBuff, ULONG len )
		: _tupleBuffer( pBuff, len, len )
	{
	}

	TupleBuffer::~TupleBuffer()
	{
		ReleaseBuff();
	}

	void TupleBuffer::MakeBuff( WSABUF& outBuff )
	{
		outBuff.buf = std::get<0>( _tupleBuffer );
		outBuff.len = std::get<1>( _tupleBuffer );
	}

	void TupleBuffer::MakeRemainedBuff( WSABUF& outBuff )
	{
		outBuff.buf = std::get<0>( _tupleBuffer ) + (std::get<1>( _tupleBuffer ) - std::get<2>( _tupleBuffer ));
		outBuff.len = std::get<2>( _tupleBuffer );
	}

	void TupleBuffer::ReleaseBuff()
	{
		if( nullptr != std::get<0>( _tupleBuffer ) )
			delete std::get<0>( _tupleBuffer );
	}

}