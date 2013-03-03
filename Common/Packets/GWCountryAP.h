/********************************************************************************
*	文件名：	GWCountryAP.h
*	全路径：	d:\Prj\Server\Common\Packets\GWCountryAP.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 27 日	15:24
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GWCOUNTRYAP_H__
#define __GWCOUNTRYAP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryAP : public Packet 
    {
    public:
        GWCountryAP( ) {} ;
        virtual ~GWCountryAP( ) {} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_AP ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT)+sizeof(GUID_t)+sizeof(BYTE); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

        INT                             GetAP() { return m_nAP; };
        VOID					        SetAP( INT nAP ) { m_nAP = nAP; }

        BYTE                            GetType() { return m_cType; }
        VOID                            SetType(BYTE cType) { m_cType = cType; }

    public:
        //数据
        GUID_t                          m_Guid;
        BYTE                            m_cType;        //0 增加  1减少
        INT                             m_nAP;
    };


    class GWCountryAPFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryAP() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_AP ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT)+sizeof(GUID_t)+sizeof(BYTE); }
    };


    class GWCountryAPHandler 
    {
    public:
        static UINT Execute( GWCountryAP* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
