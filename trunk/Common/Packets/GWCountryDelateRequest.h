/********************************************************************************
*	文件名：	GWCountryDelateRequest.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryDelateRequest.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 28 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYDELATEREQUEST_H__
#define __GWCOUNTRYDELATEREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryDelateRequest : public Packet 
    {
    public:
        GWCountryDelateRequest( ){} ;
        virtual ~GWCountryDelateRequest( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_DELATE_REQUEST ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; }

    public:
        GUID_t                          m_Guid;
    };


    class GWCountryDelateRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryDelateRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_DELATE_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t); }
    };


    class GWCountryDelateRequestHandler 
    {
    public:
        static UINT Execute( GWCountryDelateRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
