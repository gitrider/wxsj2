// CGPlayerShopEstablish.h
// 
// �ͻ�ȷ���ж���Ϣ���ݽ��������
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPESTABLISH_H__
#define __CGPLAYERSHOPESTABLISH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
namespace Packets
{
	class CGPlayerShopEstablish : public Packet
	{
	public:
		CGPlayerShopEstablish( )
		{
			m_Type			= 0;
			m_ShopNameSize	= 0;
			memset(m_ShopName, 0, MAX_SHOP_NAME);
		};
		virtual ~CGPlayerShopEstablish( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPESTABLISH; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) + sizeof(BYTE) + m_ShopNameSize*sizeof(CHAR);}

	public:
		BYTE					GetType() const {return m_Type;}	
		VOID					SetType(BYTE Type) {m_Type = Type;}

		VOID					SetShopNameSize( BYTE bSize ){ m_ShopNameSize = bSize ; } ;
		BYTE					GetShopNameSize( ){ return m_ShopNameSize ; } ;

		VOID					SetShopName( CHAR* pShopName ){
			strncpy( m_ShopName, pShopName, m_ShopNameSize ) ;
		};
		CHAR*					GetShopName(){ return (CHAR*)m_ShopName ; } ;

	private:
		BYTE					m_Type;						//�ɿ�������
		BYTE					m_ShopNameSize;				//���ֳ���
		CHAR					m_ShopName[MAX_SHOP_NAME];	//��������
	};

	class CGPlayerShopEstablishFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopEstablish() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPESTABLISH; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) + sizeof(BYTE) + MAX_SHOP_NAME*sizeof(CHAR);};
	};

	class CGPlayerShopEstablishHandler 
	{
	public:
		static UINT Execute( CGPlayerShopEstablish* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
