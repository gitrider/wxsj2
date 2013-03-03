// CGStallBuy.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLBUY_H__
#define __CGSTALLBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallBuy : public Packet
	{
	public:
		CGStallBuy( )
		{
			m_objID = INVALID_ID;
			m_nSerial = 0;
		};
		virtual ~CGStallBuy( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLBUY; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(_ITEM_GUID) + sizeof(UINT);}

		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

	private:
		ObjID_t					m_objID;		//̯��id
		_ITEM_GUID				m_ItemGuid;
		UINT					m_nSerial;

	};

	class CGStallBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLBUY; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) + sizeof(_ITEM_GUID) + sizeof(UINT);}
	};

	class CGStallBuyHandler 
	{
	public:
		static UINT Execute( CGStallBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
