/********************************************************************************
*	�ļ�����	WGCountryReturn.h
*	ȫ·����	d:\Prj\Common\Packets\WGCountryReturn.h
*	�����ˣ�	Amos
*	����ʱ�䣺	2008 �� 4 �� 15 ��	15:51
*
*	����˵����	���Ҳ����ķ���ֵ
*	�޸ļ�¼��
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

        //���ýӿ�
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
        PlayerID_t				m_PlayerID;	//������
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
