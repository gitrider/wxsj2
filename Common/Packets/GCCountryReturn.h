/********************************************************************************
*	文件名：	GCCountryReturn.h
*	全路径：	d:\Prj\Common\Packets\GCCountryReturn.h
*	创建人：	Amos
*	创建时间：	2008 年 4 月 15 日	11:41
*
*	功能说明：	国家相关的返回值
*	修改记录：
*********************************************************************************/

#ifndef __GC_COUNTRYRETURN_H__
#define __GC_COUNTRYRETURN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
    class GCCountryReturn:		public Packet
    {
    public:
        GCCountryReturn(){}
        virtual					~GCCountryReturn(){}

        //公用接口
        virtual BOOL			Read( SocketInputStream& iStream );
        virtual BOOL			Write( SocketOutputStream& oStream ) const;
        virtual UINT			Execute( Player* pPlayer );

        virtual PacketID_t		GetPacketID() const { return PACKET_GC_COUNTRYRETURN; }
        virtual UINT			GetPacketSize() const { return sizeof(m_PlayerID) + m_CountryReturn.GetPacketSize(); }

    public :
        VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

        VOID					SetCountryReturn( _COUNTRY_RETURN* pReturn )
        {
            memcpy((void*)&m_CountryReturn, pReturn, sizeof(m_CountryReturn));
        }

        _COUNTRY_RETURN*		GetCountryReturn() { return &m_CountryReturn; }

    private :
        PlayerID_t				m_PlayerID;	//连接者
        _COUNTRY_RETURN			m_CountryReturn;

    };

    class GCCountryReturnFactory:	public PacketFactory
    {
    public:
        Packet*					CreatePacket() { return new GCCountryReturn(); }
        PacketID_t				GetPacketID() const { return PACKET_GC_COUNTRYRETURN; }
        UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(_COUNTRY_RETURN); }
    };

    class GCCountryReturnHandler
    {
    public:
        static UINT				Execute(GCCountryReturn* pPacket, Player* pPlayer);
    };

}

using namespace Packets;

#endif // __WG_GUILDRETURN_H__
