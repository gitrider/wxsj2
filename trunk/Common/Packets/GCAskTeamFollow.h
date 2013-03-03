/********************************************************************************
 *	�ļ�����	GCAskTeamFollow.h
 *	ȫ·����	d:\Prj\Common\Packets\GCAskTeamFollow.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	16:09
 *
 *	����˵����	�����Ա������Ӹ���
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GC_ASKTEAMFOLLOW_H_
#define _GC_ASKTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAskTeamFollow:	public Packet
	{
	public:
		GCAskTeamFollow(){}
		virtual		~GCAskTeamFollow(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ASK_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return 0; }

	};

	class GCAskTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAskTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GC_ASK_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return 0; }			
	};

	class GCAskTeamFollowHandler
	{
	public:
		static UINT Execute(GCAskTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GC_ASKTEAMFOLLOW_H_
