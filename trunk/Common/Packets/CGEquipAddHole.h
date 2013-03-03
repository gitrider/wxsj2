#ifndef _CG_EUIP_ADDHOLE_H_
#define _CG_EUIP_ADDHOLE_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGEquipAddHole:public Packet
	{

	public:
		CGEquipAddHole(){};
		virtual				~CGEquipAddHole(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_EQUIPADDHOLE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2+sizeof(_ITEM_GUID)+sizeof(UINT);
		}

	public:
		VOID			setEquipFromType(BYTE nType){m_EquipFromType = nType;}
		BYTE			getEquipFromType(){return m_EquipFromType;}

		VOID			setEquipFromPos(BYTE nPos){m_EquipFromPos = nPos;}
		BYTE			getEquipFromPos(){return m_EquipFromPos;}

		VOID			setItemID(_ITEM_GUID id){m_ItemId	=	id;}
		_ITEM_GUID		getItemID(){return m_ItemId;}

		VOID			SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT			GetUniqueID(void) { return m_UniqueID; }
	private:
		BYTE				m_EquipFromType;	//来源
		BYTE				m_EquipFromPos;		//装备点，或背包位置
		_ITEM_GUID			m_ItemId;			//装备的GUID
		UINT				m_UniqueID;	
	};


	class CGEquipAddHoleFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipAddHole(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_EQUIPADDHOLE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*2+sizeof(_ITEM_GUID)+sizeof(UINT); }
	};


	class CGEquipAddHoleHandler
	{
	public:
		static UINT	Execute( CGEquipAddHole* pPacket, Player* pPlayer );
	};

}


#endif