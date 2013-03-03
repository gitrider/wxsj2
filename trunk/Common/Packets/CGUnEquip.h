#ifndef	_CG_UNEQUIP_H_
#define _CG_UNEQUIP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGUnEquip:public Packet
	{

	public:
		CGUnEquip(){};
		virtual				~CGUnEquip(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_UNEQUIP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(BYTE);
		}

	public:
		VOID			setEquipPoint(BYTE index){m_EquipPoint	= index;}
		BYTE			getEquipPoint(){return m_EquipPoint;}
		VOID			setBagIndex(BYTE index) {m_BagIndex = index;}
		BYTE			getBagIndex() { return m_BagIndex;}


	private:
		BYTE			m_EquipPoint;		//װ�����Ϣ
		BYTE			m_BagIndex;			//ȡ���������ڰ��е�λ��
	};


	class CGUnEquipFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUnEquip(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_UNEQUIP; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(BYTE); }
	};


	class	CGUnEquipHandler
	{
	public:
		static UINT	Execute( CGUnEquip* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

















#endif	