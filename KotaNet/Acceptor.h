#pragma once

#include "SampleDefine.h"
#include "Session.h"

namespace Kota
{   
    class Acceptor final
    {            
    public:
        Acceptor() = delete;
        Acceptor( Acceptor& ) = delete;
        Acceptor( const IPEndPoint& endPoint );

        Session* GetSession()
        {
            return &_session;
        }

        bool Accept( Session* const pSession );
        bool Bind();
        bool CreateTCP();
        bool Listen( INT32 backlog );

    private:
        Session _session;
        std::array<char, 64> _acceptBuff;
        IPEndPoint _endPoint;
    };
}
