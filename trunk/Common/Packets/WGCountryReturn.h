/********************************************************************************
*	文件名：	WGCountryReturn.h
*	全路径：	d:\Prj\Common\Packets\WGCountryReturn.h
*	创建人：	Amos
*	创建时间：	2008 年 4 月 15 日	15:51
*
*	功能说明：	国家操作的返回值
*	修改记录：
*********************************************************************************/

#ifndef __WG_COUNTRYRETURN_H__
#define __WG_COUNTRYRETURN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
    class WGCountryReturn:		public Packet
    {
    public:
        WGCountryReturn(){}
        virtual					~WGCountryReturn(){}

        //公用接口
        virtual BOOL			Read( SocketInputStream& iStream );
        virtual BOOL			Write( SocketOutputStream& oStream ) const;
        virtual UINT			Execute( Player* pPlayer );

        virtual PacketID_t		GetPacketID() const { return PACKET_WG_COUNTRYRETURN; }
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

    class WGCountryReturnFactory:	public PacketFactory
    {
    public:
        Packet*					CreatePacket() { return new WGCountryReturn(); }
        PacketID_t				GetPacketID() const { return PACKET_WG_COUNTRYRETURN; }
        UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(_COUNTRY_RETURN); }
    };

    class WGCountryReturnHandler
    {
    public:
        static UINT				Execute(WGCountryReturn* pPacket, Player* pPlayer);
    };

}

using namespace Packets;

#endif // 
