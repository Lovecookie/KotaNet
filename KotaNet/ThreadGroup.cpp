#include "stdafx.h"
#include "ThreadGroup.h"

namespace Kota
{
	void ThreadGroup::FuncAll( FuncInside&& func )
	{
		func();
	}

	void ThreadGroup::JoinAll()
	{
		for( auto& thd : _threads )
		{
			if( thd.joinable() )
				thd.join();
		}
	}
}