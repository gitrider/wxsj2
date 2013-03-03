/********************************************************************************
*	文件名：	GWCountryInfoRequest.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryInfoRequest.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 22 日	19:34
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYINFOREQUESTT_H__
#define __GWCOUNTRYINFOREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryInfoRequest : public Packet 
    {
    public:
        GWCountryInfoRequest( ){} ;
        virtual ~GWCountryInfoRequest( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_INFO_REQUEST; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT) + sizeof(GUID_t); }

        INT                             GetCountryID() { return m_nCountryID; };
        VOID					        SetCountryID( INT nCountry ) { m_nCountryID = nCountry; }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

    public:
        //数据
        GUID_t                          m_Guid;
        INT                             m_nCountryID;
    };


    class GWCountryInfoRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryInfoRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_INFO_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT) + sizeof(GUID_t); }
    };


    class GWCountryInfoRequestHandler 
    {
    public:
        static UINT Execute( GWCountryInfoRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
