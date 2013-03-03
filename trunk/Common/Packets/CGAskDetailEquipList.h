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
		ObjID_t					m_ObjID;	//�Է���ObjID

		ASK_EQUIP_MODE		m_Mode;
		/*
		|   m_ItemList [HEQUIP_NUMBER]
		| xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
		|				  | |||||||| ||||||||___  ����  HEQUIP_MAINHAND
		|				  | |||||||| |||||||____  ����  HEQUIP_ASSIHAND
		|				  | |||||||| ||||||_____  ͷ    HEQUIP_HEAD
		|				  | |||||||| |||||______  ��    HEQUIP_NECK
		|             	  |	|||||||| ||||_______  ��    HEQUIP_SHOULDER
		|				  |	|||||||| |||_________ ����  HEQUIP_BACK
		|                 | |||||||| ||_________  ��    HEQUIP_WRIST
		|                 | |||||||| |_________   ��    HEQUIP_HAND

		|                 | ||||||||___________   ��    HEQUIP_WAIST
		|             	  |	|||||||______________ �� 	HEQUIP_FEET
		|             	  |	||||||_______________ ��ָ1 HEQUIP_RING1
		|             	  |	|||||________________ ��ָ2 HEQUIP_RING2
		|             	  |	||||_________________ ��Ʒ1 HEQUIP_ADORN1
		|             	  |	|||__________________ ��Ʒ2	HEQUIP_ADORN2
		|             	  |	||___________________ ��װ	HEQUIP_SUIT
		|             	  |	|____________________ ����	HEQUIP_RIDER
		|				  |______________________ ����  HEQUIP_SOUXIA
		*/
		UINT			m_wPartFlags;		// ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� HUMAN_EQUIP
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

