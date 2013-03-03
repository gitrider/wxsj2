/********************************************************************************
*	文件名：	GWCountryVoteRequest.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryVoteRequest.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 30 日	13:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYVOTEREQUEST_H__
#define __GWCOUNTRYVOTEREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryVoteRequest : public Packet 
    {
    public:
        GWCountryVoteRequest( ){} ;
        virtual ~GWCountryVoteRequest( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_VOTE_REQUEST ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; }

    public:
        GUID_t                          m_Guid;
    };


    class GWCountryVoteRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryVoteRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_VOTE_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t); }
    };


    class GWCountryVoteRequestHandler 
    {
    public:
        static UINT Execute( GWCountryVoteRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
