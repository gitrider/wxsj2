#ifndef _CG_EQUIPBIND_H_
#define _CG_EQUIPBIND_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGEquipBind:	public Packet
	{
	public:
		CGEquipBind(){};
		virtual		~CGEquipBind(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPBIND; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT); }

		VOID					SetEquipPosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType;}
		BYTE					GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		BYTE					GetPos(){return m_nPos;}

		VOID					setGemBagPos(UINT nPos){m_nGemBagPos	=	nPos;}
		UINT					getGemBagPos(){return m_nGemBagPos;}

		VOID					setItemID(_ITEM_GUID& id){m_ItemId	=	id;}
		_ITEM_GUID				getItemID(){return m_ItemId;}

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }
	private:
		BYTE			m_EquipPosType;
		BYTE			m_nPos;
		BYTE			m_nGemBagPos;
		_ITEM_GUID		m_ItemId;			//装备的GUID
		UINT			m_UniqueID;	
	};

	class CGEquipBindFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipBind() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPBIND; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT); }			
	};

	class CGEquipBindHandler
	{
	public:
		static UINT	Execute(CGEquipBind* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif