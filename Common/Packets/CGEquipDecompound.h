#ifndef _CG_EQUIPDECOMPOUND_H_
#define _CG_EQUIPDECOMPOUND_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGEquipDecompound:	public Packet
	{
	public:
		CGEquipDecompound(){};
		virtual		~CGEquipDecompound(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPDECOMPOUND; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2+sizeof(_ITEM_GUID)+sizeof(UINT); }

		VOID					SetEquipPosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType;}
		BYTE					GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		BYTE					GetPos(){return m_nPos;}

		VOID					setItemID(_ITEM_GUID& id){m_ItemId	=	id;}
		_ITEM_GUID				getItemID(){return m_ItemId;}
	
		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }
	private:
		BYTE			m_EquipPosType;
		BYTE			m_nPos;
		_ITEM_GUID		m_ItemId;			//装备的GUID
		UINT			m_UniqueID;	
	};

	class CGEquipDecompoundFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipDecompound(); }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPDECOMPOUND; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2+sizeof(_ITEM_GUID)+sizeof(UINT); }			
	};

	class CGEquipDecompoundHandler
	{
	public:
		static UINT	Execute(CGEquipDecompound* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif