// CGPlayerShopBuyItem.h
// 
// ֪ͨ������������Ʒ
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPBUYITEM_H__
#define __CGPLAYERSHOPBUYITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
namespace Packets
{
	class CGPlayerShopBuyItem : public Packet
	{
	public:
		CGPlayerShopBuyItem( )
		{
			m_StallIndex	=	0;	//��̨ID
			m_nSerial		=	0;	//��ǰ���к�
		};
		virtual ~CGPlayerShopBuyItem( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPBUYITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) + 
																sizeof(UINT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_StallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_StallIndex = nStallIndex;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;}
		VOID					SetPetGuid(PET_GUID_t nPetGuid) {m_PetGuid = nPetGuid;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_StallIndex;		//��̨ID
		_ITEM_GUID				m_ItemGuid;			//��ƷGUID
		PET_GUID_t				m_PetGuid;			//����GUID
		UINT					m_nSerial;			//��ǰ���к�

	};

	class CGPlayerShopBuyItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopBuyItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPBUYITEM; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) + 
														sizeof(UINT);}
	};

	class CGPlayerShopBuyItemHandler 
	{
	public:
		static UINT Execute( CGPlayerShopBuyItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
