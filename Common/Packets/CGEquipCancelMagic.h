#ifndef _CG_EQUIPCANCELMAGIC_H_
#define _CG_EQUIPCANCELMAGIC_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGEquipCancelMagic:	public Packet
	{
	public:
		CGEquipCancelMagic(){};
		virtual		~CGEquipCancelMagic(){};

		// 公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;
		
		virtual PacketID_t		GetPacketID()const			{ return PACKET_CG_EQUIPCANCELMAGIC; }
		virtual UINT			GetPacketSize()const		{ return sizeof(BYTE)*2; }

		VOID					SetEquipPos(BYTE nEquipPos)		{ m_nEquipPos = nEquipPos; }
		BYTE					GetEquipPos()					{ return m_nEquipPos; }

		VOID					SetGemPos(BYTE nGemPos)			{ m_nGemPos = nGemPos; }
		BYTE					GetGemPos()						{ return m_nGemPos; }

	private:
		BYTE					m_nEquipPos;
		BYTE					m_nGemPos;

	};

	class CGEquipCancelMagicFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipCancelMagic() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_EQUIPCANCELMAGIC; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2; }			
	};

	class CGEquipCancelMagicHandler
	{
	public:
		static UINT	Execute(CGEquipCancelMagic* pPacket,Player* pPlayer);
	};


}

using namespace Packets;

#endif