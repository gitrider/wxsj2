#ifndef _CG_EQUIPEXCHANGEPAITYPE_H_
#define _CG_EQUIPEXCHANGEPAITYPE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGEquipExChangePaiType:	public Packet
	{
	public:
		CGEquipExChangePaiType(){};
		virtual		~CGEquipExChangePaiType(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPEXCHANGEPAITYPE; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*5+sizeof(_ITEM_GUID)*2+sizeof(UINT); }

		VOID					SetEquip1PosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_Equip1PosType = nType;}
		BYTE					GetEquip1PosType(){;return m_Equip1PosType;}

		VOID					SetPos1(BYTE nPos){m_nPos1 = nPos;}
		BYTE					GetPos1(){return m_nPos1;}
		
		VOID					setItem1ID(_ITEM_GUID& id){m_Item1Id =	id;}
		_ITEM_GUID				getItem1ID(){return m_Item1Id;}

		VOID					SetEquip2PosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_Equip2PosType = nType;}
		BYTE					GetEquip2PosType(){;return m_Equip2PosType;}

		VOID					SetPos2(BYTE nPos){m_nPos2 = nPos;}
		BYTE					GetPos2(){return m_nPos2;}

		VOID					setItem2ID(_ITEM_GUID& id){m_Item2Id =	id;}
		_ITEM_GUID				getItem2ID(){return m_Item2Id;}

		VOID					setGemBagPos(UINT nPos){m_nGemBagPos	=	nPos;}
		UINT					getGemBagPos(){return m_nGemBagPos;}

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }
	private:
		BYTE			m_Equip1PosType;
		BYTE			m_nPos1;
		_ITEM_GUID		m_Item1Id;			//装备1的GUID
		BYTE			m_Equip2PosType;
		BYTE			m_nPos2;
		_ITEM_GUID		m_Item2Id;			//装备2的GUID
		BYTE			m_nGemBagPos;
		UINT			m_UniqueID;	
	};

	class CGEquipExChangePaiTypeFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipExChangePaiType() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPEXCHANGEPAITYPE; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*5+sizeof(_ITEM_GUID)*2+sizeof(UINT); }			
	};

	class CGEquipExChangePaiTypeHandler
	{
	public:
		static UINT	Execute(CGEquipExChangePaiType* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif