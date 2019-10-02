#include "stdafx.h"
#include <iostream>
#include "Provider.h"
#include "NetworkInitialier.h"
#include "UnsafeByteArray.h"

int main()
{
    Kota::NetworkInitializer initializer;	

    Kota::Provider provider;
    provider.NetworkStart();

    provider.Processing();

    provider.NetworkClose();
    
    return 0;
}
