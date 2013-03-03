// GCPlayerShopOnSale.h
// 
// ֪ͨ�ͻ�����Ʒ�ϼ�
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPONSALE_H__
#define __GCPLAYERSHOPONSALE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCPlayerShopOnSale : public Packet
	{
	public:
		GCPlayerShopOnSale( )
		{
			m_StallIndex=	0;		//��̨ID
			m_nSerial	=	0;		//��ǰ���к�
			m_bIsOnSale	=	0;		//�Ƿ��ϼ�
			m_uPrice	=	0;		//��Ʒ�۸�
			m_nShopSerial = 0;		//�̵����к�
		};
		virtual ~GCPlayerShopOnSale( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPONSALE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) + 
																sizeof(UINT) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(UINT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_StallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_StallIndex = nStallIndex;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		BYTE					GetIsOnSale(VOID) const {return m_bIsOnSale;}
		VOID					SetIsOnSale(BYTE bIsOnSale) {m_bIsOnSale = bIsOnSale;}

		UINT					GetPrice(VOID) const {return m_uPrice;}
		VOID					SetPrice(UINT uPrice) {m_uPrice = uPrice;}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;}
		VOID					SetPetGuid(PET_GUID_t nPetGuid) {m_PetGuid = nPetGuid;}

		BYTE					GetShopSerial(VOID) const {return m_nShopSerial;}
		VOID					SetShopSerial(BYTE nShopSerial) {m_nShopSerial = nShopSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_StallIndex;		//��̨ID
		_ITEM_GUID				m_ItemGuid;			//��ƷGUID
		PET_GUID_t				m_PetGuid;			//����GUID
		UINT					m_nSerial;			//��ǰ���к�
		BYTE					m_bIsOnSale;		//�Ƿ��ϼ�
		UINT					m_uPrice;			//��Ʒ�۸�
		BYTE					m_nShopSerial;		//�̵����к�

	};

	class GCPlayerShopOnSaleFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopOnSale() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPONSALE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) + 
														sizeof(UINT) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(UINT);}
	};

	class GCPlayerShopOnSaleHandler 
	{
	public:
		static UINT Execute( GCPlayerShopOnSale* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
