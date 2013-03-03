/********************************************************************************
 *	文件名：	CGAskTeamFollow.h
 *	全路径：	d:\Prj\Common\Packets\CGAskTeamFollow.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 15 日	17:55
 *
 *	功能说明：	队长发出进入组队跟随模式请求
 *	修改记录：
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

		//公用接口
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
