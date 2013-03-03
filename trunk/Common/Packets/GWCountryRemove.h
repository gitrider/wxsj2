/********************************************************************************
*	文件名：	GWCountryRemove.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryRemove.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 15 日	11:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYREMOVE_H__
#define __GWCOUNTRYREMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryRemove : public Packet 
    {
    public:
        GWCountryRemove( ){} ;
        virtual ~GWCountryRemove( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_REMOVE ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_COUNTRY_REMOVE*     GetCountryRemoveData() { return &m_Data; };


        VOID					        SetCountryRemoveData( COUNTRY_CGW_COUNTRY_REMOVE* pRemove )
        {
            memcpy((void*)&m_Data, pRemove, sizeof(m_Data));
        }

    public:
        //数据
        COUNTRY_CGW_COUNTRY_REMOVE     m_Data;
    };


    class GWCountryRemoveFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryRemove() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_REMOVE ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_REMOVE); }
    };


    class GWCountryRemoveHandler 
    {
    public:
        static UINT Execute( GWCountryRemove* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
