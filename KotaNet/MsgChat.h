#pragma once
 
#include "MessageBase.h"

namespace Kota
{
    #pragma pack(1)
    class MsgChat : public MessageBase
    {
    public:
        char data[128];
    };

    #pragma pack(pop)
}