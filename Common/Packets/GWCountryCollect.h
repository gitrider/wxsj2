/********************************************************************************
*	文件名：	GWCountryCollect.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryCollect.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYCOLLECT_H__
#define __GWCOUNTRYCOLLECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryCollect : public Packet 
    {
    public:
        GWCountryCollect( ){} ;
        virtual ~GWCountryCollect( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_COLLECT ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_COUNTRY_COLLECT*    GetCollectData() { return &m_Data; };


        VOID					        SetCollectData( COUNTRY_CGW_COUNTRY_COLLECT* pCollect )
        {
            memcpy((void*)&m_Data, pCollect, sizeof(m_Data));
        }

    public:
        //数据
        COUNTRY_CGW_COUNTRY_COLLECT     m_Data;
    };


    class GWCountryCollectFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryCollect() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_COLLECT ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_COLLECT); }
    };


    class GWCountryCollectHandler 
    {
    public:
        static UINT Execute( GWCountryCollect* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
