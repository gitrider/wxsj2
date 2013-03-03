#ifndef _CG_ASKMYEQUIP_LIST_H_
#define _CG_ASKMYEQUIP_LIST_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum ASK_EQUIP_MODE
	{
		ASK_EQUIP_ALL,
		ASK_EQUIP_SET
	};

	class CGAskDetailEquipList : public Packet
	{
	public:
		CGAskDetailEquipList()	{ m_wPartFlags=0; };
		virtual	~CGAskDetailEquipList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKDETAILEQUIPLIST ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE)+
																sizeof(UINT); }

	public:
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idObj) { m_ObjID = idObj; }

		VOID					SetAskMode(ASK_EQUIP_MODE mode){ m_Mode	= mode; }
		ASK_EQUIP_MODE			GetAskMode() {return m_Mode;}

		VOID					SetAskPart(UINT wFlags) { m_wPartFlags = wFlags; }
		UINT					GetAskPart(VOID) { return m_wPartFlags; }

	private:
		ObjID_t					m_ObjID;	//对方的ObjID

		ASK_EQUIP_MODE		m_Mode;
		/*
		|   m_ItemList [HEQUIP_NUMBER]
		| xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
		|				  | |||||||| ||||||||___  主手  HEQUIP_MAINHAND
		|				  | |||||||| |||||||____  副手  HEQUIP_ASSIHAND
		|				  | |||||||| ||||||_____  头    HEQUIP_HEAD
		|				  | |||||||| |||||______  颈    HEQUIP_NECK
		|             	  |	|||||||| ||||_______  肩    HEQUIP_SHOULDER
		|				  |	|||||||| |||_________ 躯干  HEQUIP_BACK
		|                 | |||||||| ||_________  腕    HEQUIP_WRIST
		|                 | |||||||| |_________   手    HEQUIP_HAND

		|                 | ||||||||___________   腰    HEQUIP_WAIST
		|             	  |	|||||||______________ 脚 	HEQUIP_FEET
		|             	  |	||||||_______________ 戒指1 HEQUIP_RING1
		|             	  |	|||||________________ 戒指2 HEQUIP_RING2
		|             	  |	||||_________________ 饰品1 HEQUIP_ADORN1
		|             	  |	|||__________________ 饰品2	HEQUIP_ADORN2
		|             	  |	||___________________ 外装	HEQUIP_SUIT
		|             	  |	|____________________ 坐骑	HEQUIP_RIDER
		|				  |______________________ 搜侠  HEQUIP_SOUXIA
		*/
		UINT			m_wPartFlags;		// 每个位表示一个属性是否要刷新 HUMAN_EQUIP
	};

	class CGAskDetailEquipListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskDetailEquipList() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKDETAILEQUIPLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE)+
														sizeof(UINT); }
	};

	class CGAskDetailEquipListHandler
	{
	public:
		static UINT	Execute( CGAskDetailEquipList* pPacket, Player* pPlayer );
	};
}

using namespace Packets;
#endif

