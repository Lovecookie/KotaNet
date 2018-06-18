#pragma once

#include "SampleDefine.h"

namespace Kota
{
    __interface IService
    {
        bool Initialize();
        void Finalize();
    };
}
