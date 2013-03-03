#ifndef _CG_EQUIPCHANGEPAITYPE_H_
#define _CG_EQUIPCHANGEPAITYPE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGEquipChangePaiType:	public Packet
	{
	public:
		CGEquipChangePaiType(){};
		virtual		~CGEquipChangePaiType(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPCHANGEPAITYPE; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT); }

		VOID					SetEquipPosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType;}
		BYTE					GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		BYTE					GetPos(){return m_nPos;}

		VOID					setItemID(_ITEM_GUID& id){m_ItemId =	id;}
		_ITEM_GUID				getItemID(){return m_ItemId;}


		VOID					setGemBagPos(UINT nPos){m_nGemBagPos	=	nPos;}
		UINT					getGemBagPos(){return m_nGemBagPos;}

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }
	private:
		BYTE			m_EquipPosType;
		BYTE			m_nPos;
		_ITEM_GUID		m_ItemId;			//装备的GUID
		BYTE			m_nGemBagPos;
		UINT			m_UniqueID;	
	};

	class CGEquipChangePaiTypeFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipChangePaiType() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPCHANGEPAITYPE; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT); }			
	};

	class CGEquipChangePaiTypeHandler
	{
	public:
		static UINT	Execute(CGEquipChangePaiType* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif