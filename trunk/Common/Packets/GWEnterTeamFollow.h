/********************************************************************************
 *	�ļ�����	GWEnterTeamFollow.h
 *	ȫ·����	d:\Prj\Common\Packets\GWEnterTeamFollow.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	15:07
 *
 *	����˵����	Server �� World ֪��һ����ҽ�����Ӹ���״̬
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GW_ENTERTEAMFOLLOW_H_
#define _GW_ENTERTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWEnterTeamFollow:	public Packet
	{
	public:
		GWEnterTeamFollow(){}
		virtual		~GWEnterTeamFollow(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ENTER_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t); }

	public :
		VOID					SetGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					GetGUID( ) { return m_GUID; }

	private :
		GUID_t					m_GUID;	// �������״̬����� GUID

	};

	class GWEnterTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWEnterTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GW_ENTER_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t); }			
	};

	class GWEnterTeamFollowHandler
	{
	public:
		static UINT Execute(GWEnterTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GW_ENTERTEAMFOLLOW_H_
