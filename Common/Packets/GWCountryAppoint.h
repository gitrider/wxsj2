/********************************************************************************
*	文件名：	GWCountryRemove.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryAppoint.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 15 日	10:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYAPPOINT_H__
#define __GWCOUNTRYAPPOINT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryAppoint : public Packet 
    {
    public:
        GWCountryAppoint( ){} ;
        virtual ~GWCountryAppoint( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_APPOINT ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_COUNTRY_APPOINT*    GetCountryAppointData() { return &m_Data; };


        VOID					        SetCountryAppointData( COUNTRY_CGW_COUNTRY_APPOINT* pAppoint )
        {
            memcpy((void*)&m_Data, pAppoint, sizeof(m_Data));
        }
    public:
        //数据
        COUNTRY_CGW_COUNTRY_APPOINT     m_Data;
    };


    class GWCountryAppointFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryAppoint() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_APPOINT ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_APPOINT); }
    };


    class GWCountryAppointHandler 
    {
    public:
        static UINT Execute( GWCountryAppoint* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
