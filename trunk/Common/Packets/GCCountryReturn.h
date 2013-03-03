/********************************************************************************
*	�ļ�����	GCCountryReturn.h
*	ȫ·����	d:\Prj\Common\Packets\GCCountryReturn.h
*	�����ˣ�	Amos
*	����ʱ�䣺	2008 �� 4 �� 15 ��	11:41
*
*	����˵����	������صķ���ֵ
*	�޸ļ�¼��
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

        //���ýӿ�
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
        PlayerID_t				m_PlayerID;	//������
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
