// GCStallRemoveItem.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLREMOVEITEM_H__
#define __GCSTALLREMOVEITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallRemoveItem : public Packet
	{
	public:
		GCStallRemoveItem( )
		{
			m_ToType = 0;
			m_Serial = 0;
		};
		virtual ~GCStallRemoveItem( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLREMOVEITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) +
																sizeof(UINT) +
																sizeof(BYTE);}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t Guid) {m_PetGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_Serial;}
		VOID					SetSerial(UINT Serial) {m_Serial = Serial;}

		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

	private:
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_Serial;
		BYTE					m_ToType;
	};

	class GCStallRemoveItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallRemoveItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLREMOVEITEM; };
		UINT		GetPacketMaxSize()const { return	sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) +
														sizeof(UINT) +
														sizeof(BYTE);}
	};

	class GCStallRemoveItemHandler 
	{
	public:
		static UINT Execute( GCStallRemoveItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
