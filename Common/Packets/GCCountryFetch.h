/********************************************************************************
*	文件名：	GCCountryFetch.h
*	全路径：	d:\Prj\Server\Common\Packets\GCCountryFetch.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GCCOUNTRYFETCH_H__
#define __GCCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryFetch : public Packet 
    {
    public:
        GCCountryFetch( ){} ;
        virtual ~GCCountryFetch( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetGold() { return m_nGold; };


        VOID					        SetGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //数据
        INT                             m_nGold;
    };


    class GCCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class GCCountryFetchHandler 
    {
    public:
        static UINT Execute( GCCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
