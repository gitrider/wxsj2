/********************************************************************************
*	文件名：	GWCountryFetch.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryFetch.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYFETCH_H__
#define __GWCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryFetch : public Packet 
    {
    public:
        GWCountryFetch( ){} ;
        virtual ~GWCountryFetch( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT)+sizeof(GUID_t); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

        INT                             GetGold() { return m_nGold; };
        VOID					        SetGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //数据
        GUID_t                          m_Guid;
        INT                             m_nGold;
    };


    class GWCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT)+sizeof(GUID_t); }
    };


    class GWCountryFetchHandler 
    {
    public:
        static UINT Execute( GWCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
