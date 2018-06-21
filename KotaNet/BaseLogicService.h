#pragma once

#include "IService.h"

namespace Kota
{
	class BaseLogicService : public IService
	{
		bool Initialize() override
		{
			return true;
		}
		void Finalize() override
		{			
		}
	};
}
