/********************************************************************************
 *	�ļ�����	CGAskTeamFollow.h
 *	ȫ·����	d:\Prj\Common\Packets\CGAskTeamFollow.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 15 ��	17:55
 *
 *	����˵����	�ӳ�����������Ӹ���ģʽ����
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _CG_ASKTEAMFOLLOW_H_
#define _CG_ASKTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskTeamFollow:	public Packet
	{
	public:
		CGAskTeamFollow(){}
		virtual		~CGAskTeamFollow(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASK_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return 0; }

	};

	class CGAskTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_CG_ASK_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return 0; }			
	};

	class CGAskTeamFollowHandler
	{
	public:
		static UINT Execute(CGAskTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _CG_ASKTEAMFOLLOW_H_
