#ifndef _CG_EQUIPLEVELUP_H_
#define _CG_EQUIPLEVELUP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGEquipLevelup:	public Packet
	{
	public:
		CGEquipLevelup(){};
		virtual		~CGEquipLevelup(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EQUIPLEVELUP; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*3+sizeof(UINT)+sizeof(_ITEM_GUID); }

		VOID					SetEquipPosType(BYTE nType){Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType;}
		BYTE					GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		BYTE					GetPos(){return m_nPos;}

		VOID					SetGemNum(BYTE nGemNum){Assert(nGemNum<MAX_LEVELUP_GEMREQ_NUM+1);m_nGemNum = nGemNum;}
		BYTE					GetGemNum(){return m_nGemNum;}
		
		VOID					setGemID(UINT id){m_GemId	=	id;}
		UINT					getGemID(){return m_GemId;}

		VOID					setItemID(_ITEM_GUID& id){m_ItemId	=	id;}
		_ITEM_GUID				getItemID(){return m_ItemId;}
	private:
		BYTE			m_EquipPosType;
		BYTE			m_nPos;
		BYTE			m_nGemNum;
		UINT			m_GemId;
		_ITEM_GUID		m_ItemId;			//装备的GUID
	};

	class CGEquipLevelupFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipLevelup() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPLEVELUP; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3+sizeof(UINT)+sizeof(_ITEM_GUID); }			
	};

	class CGEquipLevelupHandler
	{
	public:
		static UINT	Execute(CGEquipLevelup* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif