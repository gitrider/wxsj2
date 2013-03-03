// CGShopRepair.h
// 
// 客户端通知服务器要修什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPSPECIALREPAIR_H__
#define __CGSHOPSPECIALREPAIR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopSpecialRepair : public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromEquip,
		};
	public:
		CGShopSpecialRepair( )
		{
			m_Opt = FromBag;
			m_nBagIndex			=	INVALID_ITEM_POS;
			m_nItemNum			=	0;
		};
		virtual ~CGShopSpecialRepair( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPSPECIALREPAIR ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*3;}

	public:
		BYTE					GetBagIndex(VOID) const {return m_nBagIndex;};
		VOID					SetBagIndex(BYTE nNumber) {m_nBagIndex = nNumber;};

		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE Opt) {m_Opt = Opt;};

		VOID					SetItemNum(UINT nItemNum) { m_nItemNum = nItemNum; }
		UINT					GetItemNum(void) { return m_nItemNum; }

	private:
		BYTE					m_Opt;
		BYTE					m_nBagIndex;			//格子索引
		BYTE					m_nItemNum;		

	};

	class CGShopSpecialRepairFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopSpecialRepair() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPSPECIALREPAIR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*3;};
	};

	class CGShopSpecialRepairHandler 
	{
	public:
		static UINT Execute( CGShopSpecialRepair* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif