#pragma once

#include "SampleDefine.h"

namespace Kota
{   
    #pragma pack(1)
    class MessageBase abstract
    {
    public:
        UINT16 size;
        UINT16 id;
    };    

    #pragma pack(pop)
}
