/********************************************************************************
*	文件名：	GWCountryDonative.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryDonative.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYDONATIVE_H__
#define __GWCOUNTRYDONATIVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryDonative : public Packet 
    {
    public:
        GWCountryDonative( ){} ;
        virtual ~GWCountryDonative( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_DONATIVE ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT)+sizeof(GUID_t); }

        INT                             GetDonativeGold() { return m_nGold; };
        VOID					        SetDonativeGold( INT nGold ) { m_nGold = nGold; }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

    public:
        //数据
        GUID_t                          m_Guid;
        INT                             m_nGold;
    };


    class GWCountryDonativeFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryDonative() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_DONATIVE ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT)+sizeof(GUID_t); }
    };


    class GWCountryDonativeHandler 
    {
    public:
        static UINT Execute( GWCountryDonative* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
