/********************************************************************************
*	文件名：	GCCountryInfoConfirm.h
*	全路径：	d:\Prj\Server\Common\Packets\GCCountryInfoConfirm.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 22 日	19:34
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GCCOUNTRYINFOCONFIRM_H__
#define __GCCOUNTRYINFOCONFIRM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryInfoConfirm : public Packet 
    {
    public:
        GCCountryInfoConfirm( ){} ;
        virtual ~GCCountryInfoConfirm( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_INFO_CONFIRM; }
        virtual UINT			        GetPacketSize()const { return sizeof(COUNTRY_CGW_COUNTRY_INFO) + sizeof(PlayerID_t); }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        COUNTRY_CGW_COUNTRY_INFO*       GetCountryInfo() { return &m_Info; };
        VOID					        SetCountryInfo( COUNTRY_CGW_COUNTRY_INFO *pInfo )
        {
            memcpy(&m_Info, pInfo, sizeof(COUNTRY_CGW_COUNTRY_INFO));
        }

    public:
        //数据
        PlayerID_t				        m_PlayerID;	//连接者
        COUNTRY_CGW_COUNTRY_INFO        m_Info;
    };

    class GCCountryInfoConfirmFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryInfoConfirm() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_INFO_CONFIRM ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_INFO) + sizeof(PlayerID_t); }
    };

    class GCCountryInfoConfirmHandler 
    {
    public:
        static UINT Execute( GCCountryInfoConfirm* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
