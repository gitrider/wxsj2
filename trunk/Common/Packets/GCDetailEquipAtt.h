// GCDetailEquipAtt.h
// 
///////////////////////////////////////////////////////
#ifndef _GCDETAILEQUIPATT_H_
#define _GCDETAILEQUIPATT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

struct EQUIP_ATT_CONTENT
{
	BYTE		 m_nEquipPoint;
	_ITEM_ATTR	 m_nEquipAtt;

	UINT	GetSize() const 
	{
		return	sizeof(BYTE)+sizeof(INT)+sizeof(SHORT);
	}
};

namespace Packets
{
	class GCDetailEquipAtt : public Packet
	{
	public:
		GCDetailEquipAtt(){m_wPartFlags = 0;}
		virtual	~GCDetailEquipAtt(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_DETAILEQUIPATT ; }
		virtual UINT			GetPacketSize() const 
		{ 
			UINT	uSize = sizeof(ObjID_t) + sizeof(BYTE);
			for(INT	i=0; i<HEQUIP_NUMBER; i++)
			{
				if( m_wPartFlags & (1<<i) ) uSize += m_ItemList[i].GetSize();
			}

			return uSize;
		}

	public:
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idObj) { m_ObjID = idObj; }


		VOID					SetAskPart(UINT wFlags) { m_wPartFlags = wFlags; }
		UINT					GetAskPart(VOID)		 { return m_wPartFlags; }

		VOID					SetEquipAttData(HUMAN_EQUIP Point,const	SHORT  nValue)
		{
			m_ItemList[Point].m_nEquipPoint       = Point;
			//m_ItemList[Point].m_nEquipAtt.m_Value = nValue;
			m_wPartFlags|=(1<<(INT)Point);
		}

		UINT					GetPartFlags(VOID) { return m_wPartFlags; }

	private:
		ObjID_t				m_ObjID;	//对方的ObjID
		BYTE				m_wPartFlags;		// 每个位表示一个属性是否要刷新 HUMAN_EQUIP
		EQUIP_ATT_CONTENT	m_ItemList[HEQUIP_NUMBER];
	};

	class GCDetailEquipAttFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCDetailEquipAtt() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_DETAILEQUIPATT; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof(UINT)+
			sizeof(EQUIP_ATT_CONTENT)*HEQUIP_NUMBER; }
	};

	class GCDetailEquipAttHandler
	{
	public:
		static UINT	Execute( GCDetailEquipAtt* pPacket, Player* pPlayer );
	};

}


#endif // _GCDETAILEQUIPATT_H_