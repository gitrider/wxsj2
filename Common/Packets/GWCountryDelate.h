/********************************************************************************
*	文件名：	GWCountryDelate.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryDelate.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 28 日	19:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYDELATE_H__
#define __GWCOUNTRYDELATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryDelate : public Packet 
    {
    public:
        GWCountryDelate( ){} ;
        virtual ~GWCountryDelate( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_DELATE ; }
        virtual UINT			        GetPacketSize()const { return sizeof(BOOL)+sizeof(GUID_t); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

        INT                             GetResult() { return m_bResult; };
        VOID					        SetResult( BOOL bResult )
        {
            m_bResult = bResult;
        }

    public:
        //数据
        GUID_t                          m_Guid;
        BOOL                            m_bResult;
    };


    class GWCountryDelateFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryDelate() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_DELATE ; }
        UINT		GetPacketMaxSize()const { return sizeof(BOOL)+sizeof(GUID_t); }
    };


    class GWCountryDelateHandler 
    {
    public:
        static UINT Execute( GWCountryDelate* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
