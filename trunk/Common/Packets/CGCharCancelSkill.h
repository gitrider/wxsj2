// CGCharCancelSkill.h
// 
// 使用技能
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARCANCELSKILL_H__
#define __CGCHARCANCELSKILL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharCancelSkill : public Packet
	{
	public:
		CGCharCancelSkill(){}
		virtual ~CGCharCancelSkill(){}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const {return PACKET_CG_CHARCANCELSKILL;}
		virtual UINT			GetPacketSize()const {return sizeof(ObjID_t);}

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) {m_ObjID = id;}
		ObjID_t			getObjID(VOID)const {return m_ObjID;}
	private:
		ObjID_t			m_ObjID;			// ObjID
	};


	class CGCharCancelSkillFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new CGCharCancelSkill();}
		PacketID_t	GetPacketID()const {return PACKET_CG_CHARCANCELSKILL;}
		UINT		GetPacketMaxSize()const {return sizeof(ObjID_t);}
	};

	class CGCharCancelSkillHandler 
	{
	public:
		static UINT Execute(CGCharCancelSkill* pPacket, Player* pPlayer);
	};
}

using namespace Packets;



#endif