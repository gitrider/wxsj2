/********************************************************************************
 *	文件名：	GWLevelUp.h
 *	全路径：	d:\Prj\Common\Packets\GWLevelUp.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 9 日	14:25
 *
 *	功能说明：	给 World 发送玩家升级信息，以备查
 *	修改记录：
*********************************************************************************/
#ifndef _GW_LEVEL_UP_H_
#define _GW_LEVEL_UP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWLevelUp:public Packet
	{
	public:
		GWLevelUp(){};
		virtual					~GWLevelUp(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_LEVEUP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(GUID_t) + sizeof(INT) + sizeof(UINT);
		}

	public:

		VOID			SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t			GetGUID() { return m_GUID; }

		VOID			SetLevel(INT lvl) { m_Level = lvl; }
		INT				GetLevel() { return m_Level; }

		VOID			SetExp(UINT val) { m_m_Exp = val; }
		UINT			GetExp()		 { return m_m_Exp ; }

	private:
		GUID_t			m_GUID;			
		INT				m_Level;			
		UINT			m_m_Exp;			//玩家经验

	};


	class GWLevelUpFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWLevelUp(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_LEVEUP; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t) + sizeof(INT) + sizeof(UINT); }
	};


	class GWLevelUpHandler
	{
	public:
		static UINT Execute( GWLevelUp* pPacket, Player* pPlayer );
	};

}

#endif // _GW_LEVEL_UP_H_
