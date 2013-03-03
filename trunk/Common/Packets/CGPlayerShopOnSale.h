// CGPlayerShopOnSale.h
// 
// ֪ͨ��������Ʒ�ϼ�
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPONSALE_H__
#define __CGPLAYERSHOPONSALE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopOnSale : public Packet
	{
	public:
		CGPlayerShopOnSale( )
		{
			m_StallIndex	=	0;	//��̨ID
			m_nSerial		=	0;	//��ǰ���к�
			m_nPrice		=	0;	//�۸�
			m_bIsOnSale		=	0;	//�Ƿ��ϼ�
		};
		virtual ~CGPlayerShopOnSale( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPONSALE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(_ITEM_GUID) + 
																sizeof(UINT) +
																sizeof(UINT) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(PET_GUID_t);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_StallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_StallIndex = nStallIndex;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		UINT					GetPrice(VOID) const {return m_nPrice;}
		VOID					SetPrice(UINT nPrice) {m_nPrice = nPrice;}

		BYTE					GetIsOnSale(VOID) const {return m_bIsOnSale;}
		VOID					SetIsOnSale(BYTE bIsOnSale) {m_bIsOnSale = bIsOnSale;}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;}
		VOID					SetPetGuid(PET_GUID_t nPetGuid) {m_PetGuid = nPetGuid;}

		BYTE					GetShopSerial(VOID) const {return m_nShopSerial;}
		VOID					SetShopSerial(BYTE nSerial) {m_nShopSerial = nSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_StallIndex;		//��̨ID
		_ITEM_GUID				m_ItemGuid;			//��ƷGUID
		PET_GUID_t				m_PetGuid;			//����GUID
		UINT					m_nSerial;			//��ǰ���к�
		UINT					m_nPrice;			//�۸�
		BYTE					m_bIsOnSale;		//�Ƿ��ϼ�
		BYTE					m_nShopSerial;			//�̵����к�
	};

	class CGPlayerShopOnSaleFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopOnSale() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPONSALE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(_ITEM_GUID) + 
														sizeof(UINT) +
														sizeof(UINT) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(PET_GUID_t);}
	};

	class CGPlayerShopOnSaleHandler 
	{
	public:
		static UINT Execute( CGPlayerShopOnSale* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
