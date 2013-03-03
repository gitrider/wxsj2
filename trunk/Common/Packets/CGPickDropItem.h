#ifndef _CG_PICK_DROPITEM_H_
#define _CG_PICK_DROPITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGPickDropItem:public Packet
	{

	public:
		CGPickDropItem()
		{
			m_DropItemNameSize = 0;
			memset( (void*)m_DropItemName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		};
		virtual				~CGPickDropItem(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_PICKDROPITEM	; }

		virtual UINT			GetPacketSize() const
		{
			return	( sizeof(ObjID_t)
					+ sizeof(UCHAR)
					+ sizeof(CHAR)* m_DropItemNameSize );
			//return sizeof(ObjID_t);
		}

	public:
		
		VOID			setObj_ItemId(ObjID_t	id){m_ObjID = id;};
		ObjID_t			getObj_ItemId(){return m_ObjID;}
		VOID			setObj_ItemName( const CHAR* pName )
		{
			strncpy( m_DropItemName, pName, MAX_CHARACTER_NAME-1 );
			m_DropItemNameSize = (UCHAR)strlen(m_DropItemName);
		};
		const CHAR*		getObj_ItemName(){ return m_DropItemName; };
	private:
		ObjID_t			m_ObjID;
		UCHAR			m_DropItemNameSize;

		//在这里我没有找到掉落物品名称长度的限制，暂时先把它的长度定义成和玩家名称最长长度一样
		CHAR			m_DropItemName[MAX_CHARACTER_NAME];
	};

	class CGPickDropItemFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGPickDropItem(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_PICKDROPITEM; }
		//UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t);  }
		UINT		GetPacketMaxSize() const { return (sizeof(ObjID_t)
											+ sizeof(UCHAR)
											+ sizeof(CHAR)* MAX_CHARACTER_NAME); }
	};

	class CGPickDropItemHandler
	{
	public:
		static UINT	Execute( CGPickDropItem* pPacket, Player* pPlayer );
	};
}


#endif