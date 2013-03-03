// GCPlayerShopStallStatus.h
// 
// �����̵��С
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPSTALLSTATUS_H__
#define __GCPLAYERSHOPSTALLSTATUS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCPlayerShopStallStatus : public Packet
	{
	public:
		GCPlayerShopStallStatus( )
		{
			m_nStallIndex = 0;
			m_bStatus	  = 0;
			m_FinalStallNum=0;	

		}
		virtual ~GCPlayerShopStallStatus( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPSTALLSTATUS; }
		virtual UINT			GetPacketSize()const { return sizeof(_PLAYERSHOP_GUID) + 
															  sizeof(BYTE) + 
															  sizeof(BYTE) + 
															  sizeof(BYTE);	}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_nStallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_nStallIndex = nStallIndex;}

		BYTE					GetStatus(VOID) const {return m_bStatus;}
		VOID					SetStatus(BYTE bStatus) {m_bStatus = bStatus;}

		BYTE					GetFinalStallNum(VOID) const {return m_FinalStallNum;}
		VOID					SetFinalStallNum(BYTE FinalStallNum) {m_FinalStallNum = FinalStallNum;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_nStallIndex;		//��̨��
		BYTE					m_bStatus;			//�¹�̨״̬
		BYTE					m_FinalStallNum;	//���չ�̨��
	};

	class GCPlayerShopStallStatusFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopStallStatus() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPSTALLSTATUS; }
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) +
														sizeof(BYTE) + 
														sizeof(BYTE);}
	};

	class GCPlayerShopStallStatusHandler 
	{
	public:
		static UINT Execute( GCPlayerShopStallStatus* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
