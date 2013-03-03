#ifndef _CG_EQUIP_REFINING_H_
#define _CG_EQUIP_REFINING_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGEquipRefining : public Packet
	{
	public:
		CGEquipRefining(VOID);
		virtual ~CGEquipRefining(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_EQUIP_REFINING ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return  sizeof(BYTE)+sizeof(BYTE);
		}

		VOID				SetEquipItemPos(BYTE pos)	{ m_nEquipBagPos = pos;}
		BYTE				GetEquipItemPos(){ return m_nEquipBagPos;}

		VOID				SetRefiningItemPos(BYTE pos)	{ m_nRefiningItemPos = pos;}
		BYTE				GetRefiningItemPos(){ return m_nRefiningItemPos;}
	private:
		BYTE				m_nEquipBagPos; // 要的炼化的装备在背包的位置索引
		BYTE				m_nRefiningItemPos; // 炼化神符位置索引
	};

	class CGEquipRefiningFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipRefining() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_EQUIP_REFINING; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(BYTE);}
	};

	class CGEquipRefiningHandler
	{
	public:
		static UINT Execute( CGEquipRefining* pPacket, Player* pPlayer );
	};
}
using namespace Packets ;
#endif // _CG_EQUIP_REFINING_H_

