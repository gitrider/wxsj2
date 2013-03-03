/********************************************************************************
*	文件名：	CGCountryVoteOn.h
*	全路径：	d:\Prj\Server\Common\Packets\CGCountryVoteOn.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 29 日	10:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __CGCOUNTRYVOTEON_H__
#define __CGCOUNTRYVOTEON_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryVoteOn : public Packet 
    {
    public:
        CGCountryVoteOn( ){} ;
        virtual ~CGCountryVoteOn( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_VOTEON ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t) + sizeof(BOOL); }

        VOID                            SetResult (BOOL bResult) { m_bResult = bResult; }
        BOOL                            GetResult () { return m_bResult; }

        VOID                            SetGuid (GUID_t guid) { m_Guid = guid; }
        GUID_t                          GetGuid () { return m_Guid; }

    public:

        BOOL                            m_bResult;
        GUID_t                          m_Guid;
    };


    class CGCountryVoteOnFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryVoteOn() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_VOTEON ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(BOOL); }
    };


    class CGCountryVoteOnHandler 
    {
    public:
        static UINT Execute( CGCountryVoteOn* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
