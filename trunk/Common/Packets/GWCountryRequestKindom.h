

#ifndef __GWCOUNTRYREQUESTKINDOM_H__
#define __GWCOUNTRYREQUESTKINDOM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryRequestKindom : public Packet 
    {
    public:
        GWCountryRequestKindom( ){} ;
        virtual ~GWCountryRequestKindom( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_REQUEST_KINDOM ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_KINDOM_REQUEST*     GetKindomRequestData() { return &m_Data; };


        VOID					        SetGetKindomRequestData( COUNTRY_CGW_KINDOM_REQUEST* pRequest )
        {
            memcpy((void*)&m_Data, pRequest, sizeof(m_Data));
        }

    public:
        //数据
        COUNTRY_CGW_KINDOM_REQUEST      m_Data;
    };


    class GWCountryRequestKindomFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryRequestKindom() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_REQUEST_KINDOM ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_KINDOM_REQUEST); }
    };


    class GWCountryRequestKindomHandler 
    {
    public:
        static UINT Execute( GWCountryRequestKindom* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
