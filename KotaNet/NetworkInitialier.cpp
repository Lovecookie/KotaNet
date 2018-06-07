#include "NetworkInitialier.h"

#include "SampleDefine.h"

namespace Kota
{
    NetworkInitializer::NetworkInitializer()
    {
        WSADATA wsa;
        if( 0 != WSAStartup( MAKEWORD( 2, 2 ), &wsa ) )
        {
            //error
            _isIntializerError = false;
        }

        _netApi.CreateAPI();

        _isIntializerError = true;
    }

    NetworkInitializer::~NetworkInitializer()
    {
        WSACleanup();
    }

    bool NetworkInitializer::GetError()
    {   
        return _isIntializerError;
    }

}
