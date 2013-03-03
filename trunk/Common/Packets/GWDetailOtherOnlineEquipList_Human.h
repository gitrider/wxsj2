// GWDetailOtherOnlineEquipList_Human.h
// 
///////////////////////////////////////////////////////
#ifndef _GWDetailOtherOnlineEquipList_Human_H_
#define _GWDetailOtherOnlineEquipList_Human_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGAskDetailEquipList.h"

namespace Packets
{
	class GWDetailOtherOnlineEquipList_Human : public Packet
	{
	public:
		GWDetailOtherOnlineEquipList_Human(){m_wPartFlags = 0;}
		virtual	~GWDetailOtherOnlineEquipList_Human(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_DETAILOTHERONLINEEQUIPLIST_HUMAN ; }
		virtual UINT			GetPacketSize() const 
		{ 
			UINT	uSize = sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE) + sizeof(WORD) + sizeof(GUID_t);
			for(INT	i=0; i<HEQUIP_NUMBER; i++)
			{
				if( m_wPartFlags & (1<<i) ) uSize += GetItemVarSize(m_ItemList[i]);
			}

			return uSize;
		}

	public:
		ObjID_t					GetPlayerID(VOID) { return m_ObjID; }
		VOID					SetPlayerID(ObjID_t idObj) { m_ObjID = idObj; }

		VOID					SetAskMode(ASK_EQUIP_MODE mode){m_Mode	=	mode;}
		ASK_EQUIP_MODE			GetAskMode(){return m_Mode;}

		VOID					SetAskPart(UINT wFlags) { m_wPartFlags = wFlags; }
		UINT					GetAskPart(VOID)		{ return m_wPartFlags; }

		INT						GetItemCount(VOID) 
		{
			if(m_Mode == ASK_EQUIP_ALL) return HEQUIP_NUMBER;

			INT	nItemNum = 0;
			for(INT	i=0; i<HEQUIP_NUMBER; i++)
			{
				if( m_wPartFlags & (1<<i) ) nItemNum++;
			}
			return nItemNum;
		}

		VOID					SetEquipData(HUMAN_EQUIP Point,const	_ITEM* pEquip)
		{
			memcpy(&m_ItemList[Point],pEquip,sizeof(_ITEM));
			m_wPartFlags|=(1<<(INT)Point);
		}

		_ITEM*					GetEquipData(VOID)
		{
			return	m_ItemList;
		}
		_ITEM*					GetEquipData(HUMAN_EQUIP Point)
		{
			return  &m_ItemList[Point];
		}

		UINT					GetPartFlags(VOID) { return m_wPartFlags; }

		GUID_t			GetPlayerGUID() {return m_PlayerGUID;}
		VOID			SetPlayerGUID(GUID_t guid) {m_PlayerGUID = guid;}

	private:
		ObjID_t				m_ObjID;	//对方的ObjID

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
		_ITEM			m_ItemList[HEQUIP_NUMBER];
		GUID_t			m_PlayerGUID;
	};

	class GWDetailOtherOnlineEquipList_HumanFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWDetailOtherOnlineEquipList_Human() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_DETAILOTHERONLINEEQUIPLIST_HUMAN; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE)+
			sizeof(UINT)+sizeof(GUID_t) +
			sizeof(_ITEM)*HEQUIP_NUMBER; }
	};

	class GWDetailOtherOnlineEquipList_HumanHandler
	{
	public:
		static UINT	Execute( GWDetailOtherOnlineEquipList_Human* pPacket, Player* pPlayer );
	};

}


#endif // _GWDetailOtherOnlineEquipList_Human_H_
