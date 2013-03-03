#ifndef _CG_EQUIPADSORBMAGIC_H_
#define _CG_EQUIPADSORBMAGIC_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGEquipAdsorbMagic:	public Packet
	{
	public:
		CGEquipAdsorbMagic(){};
		virtual		~CGEquipAdsorbMagic(){};

		// 公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;
		
		virtual PacketID_t		GetPacketID()const			{ return PACKET_CG_EQUIPADSORBMAGIC; }
		virtual UINT			GetPacketSize()const		{ return sizeof(BYTE)*3+sizeof(INT); }

		VOID					SetEquipPosType(BYTE nType)	{ Assert(nType<INVALID_ITEM_POS_TYPE);m_EquipPosType = nType; }
		BYTE					GetEquipPosType()			{ return m_EquipPosType; }

		VOID					SetEquipPos(BYTE nEquipPos)		{ m_nEquipPos = nEquipPos; }
		BYTE					GetEquipPos()					{ return m_nEquipPos; }

		VOID					SetBeadPos(BYTE nBeadPos)	{ m_nBeadPos = nBeadPos; }
		BYTE					GetBeadPos()				{ return m_nBeadPos; }

		/*VOID					setItemID(_ITEM_GUID& id)	{ m_ItemId = id; }
		_ITEM_GUID				getItemID()					{ return m_ItemId; }

		VOID					setBeadID( _ITEM_GUID& beadid ){ m_BeadId = beadid; }
		_ITEM_GUID				getBeadID()					{ return m_BeadId; }*/

		VOID					setMoney(INT nMoney)		{ m_Money = nMoney;}
		INT						getMoney()					{ return m_Money; }

	private:
		INT				m_Money;		//附魔需要的金钱
		BYTE			m_EquipPosType;
		BYTE			m_nEquipPos;
		BYTE			m_nBeadPos;

		//_ITEM_GUID		m_ItemId;
		//_ITEM_GUID		m_BeadId;			// 魂珠的ID

	};

	class CGEquipAdsorbMagicFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipAdsorbMagic() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPADSORBMAGIC; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3+sizeof(INT); }			
	};

	class CGEquipAdsorbMagicHandler
	{
	public:
		static UINT	Execute(CGEquipAdsorbMagic* pPacket,Player* pPlayer);
	};


}

using namespace Packets;

#endif