/********************************************************************************
*	文件名：	GWCountryVoteOn.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryVoteOn.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 29 日	10:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYVOTEON_H__
#define __GWCOUNTRYVOTEON_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryVoteOn : public Packet 
    {
    public:
        GWCountryVoteOn( ){} ;
        virtual ~GWCountryVoteOn( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_VOTEON ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t) + sizeof(BOOL); }

        VOID                            SetResult (BOOL bResult) { m_bResult = bResult; }
        BOOL                            GetResult () { return m_bResult; }

        VOID                            SetGuid (GUID_t guid) { m_Guid = guid; }
        GUID_t                          GetGuid () { return m_Guid; }

    public:

        BOOL                            m_bResult;
        GUID_t                          m_Guid;
    };


    class GWCountryVoteOnFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryVoteOn() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_VOTEON ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(BOOL); }
    };


    class GWCountryVoteOnHandler 
    {
    public:
        static UINT Execute( GWCountryVoteOn* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
