/********************************************************************************
*	文件名：	CGCountryFetch.h
*	全路径：	d:\Prj\Server\Common\Packets\CGCountryFetch.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __CGCOUNTRYFETCH_H__
#define __CGCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryFetch : public Packet 
    {
    public:
        CGCountryFetch( ){} ;
        virtual ~CGCountryFetch( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetDonativeGold() { return m_nGold; };


        VOID					        SetDonativeGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //数据
        INT                             m_nGold;
    };


    class CGCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class CGCountryFetchHandler 
    {
    public:
        static UINT Execute( CGCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
