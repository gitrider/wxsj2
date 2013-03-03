#ifndef _CG_EQUIPREMOULD_H_
#define _CG_EQUIPREMOULD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


struct  EQUIPREMOULD_MATERIAL_INFO
{
	EQUIPREMOULD_MATERIAL_INFO()
	{
		m_nMaterialType  = -1;
		m_nMaterialLevel = 0;
	}
	BYTE m_nMaterialType;
	BYTE m_nMaterialLevel;
};

namespace Packets
{
	class CGEquipRemould:	public Packet
	{
	public:
		CGEquipRemould()
		{ 
			m_EquipPosType = INVALID_ITEM_POS_TYPE; 
			m_ValidNum = 0; 
			m_nPos = INVALID_ITEM_POS;
			m_ItemId.Reset();
		};
		virtual		~CGEquipRemould(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPREMOULD; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(EQUIPREMOULD_MATERIAL_INFO)*m_ValidNum; }

		VOID					SetEquipPosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType;}
		BYTE					GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		BYTE					GetPos(){return m_nPos;}

		VOID					setItemID(_ITEM_GUID& id){m_ItemId	=	id;}
		_ITEM_GUID				getItemID(){return m_ItemId;}

		VOID					SetValidNum(BYTE nValidNum){m_ValidNum = nValidNum;}
		BYTE					GetValidNum(){return m_ValidNum;}

		VOID						SetMaterialInfo(EQUIPREMOULD_MATERIAL_INFO& nInfo)
		{	Assert(m_ValidNum<MAX_REMOULD_MATERIAL_NUM);  m_MaterialInfo[m_ValidNum] = nInfo; ++m_ValidNum;}
		EQUIPREMOULD_MATERIAL_INFO&	GetMaterialReq(UINT nNum){Assert(nNum < m_ValidNum); return m_MaterialInfo[nNum];}

	private:
		BYTE			m_EquipPosType;
		BYTE			m_nPos;
		_ITEM_GUID		m_ItemId;			//装备的GUID
		BYTE			m_ValidNum;
		EQUIPREMOULD_MATERIAL_INFO m_MaterialInfo[MAX_REMOULD_MATERIAL_NUM];
	};

	class CGEquipRemouldFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipRemould(); }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPREMOULD; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+MAX_REMOULD_MATERIAL_NUM*sizeof(EQUIPREMOULD_MATERIAL_INFO); }			
	};

	class CGEquipRemouldHandler
	{
	public:
		static UINT	Execute(CGEquipRemould* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif