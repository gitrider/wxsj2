// GCPlayerShopUpdateFavorite.h
// 
// ���¿ͻ����̵��б��е��ղ��̵�
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPUPDATEFAVORITE_H__
#define __GCPLAYERSHOPUPDATEFAVORITE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCPlayerShopUpdateFavorite: public Packet
	{
	public:
		GCPlayerShopUpdateFavorite( )
		{
			m_bIsInFavorite = FALSE;
		};
		virtual ~GCPlayerShopUpdateFavorite( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPUPDATEFAVORITE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetIsInFavorite(VOID) const {return m_bIsInFavorite;}
		VOID					SetIsInFavorite(BYTE bIsInFavorite) {m_bIsInFavorite = bIsInFavorite;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_bIsInFavorite;	//��̨ID

	};

	class GCPlayerShopUpdateFavoriteFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopUpdateFavorite() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPUPDATEFAVORITE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE);}
	};

	class GCPlayerShopUpdateFavoriteHandler 
	{
	public:
		static UINT Execute( GCPlayerShopUpdateFavorite* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
