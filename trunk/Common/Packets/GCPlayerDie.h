// GCPlayerDie.h
// 
// ��������
//
//////////////////////////////////////////////////////

#ifndef __GCPLAYERDIE_H__
#define __GCPLAYERDIE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerDie : public Packet
	{
	public:
		GCPlayerDie( )
		{
			m_bCanRelive	= FALSE;
			m_uTime			= 0;
		}
		virtual ~GCPlayerDie( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYER_DIE ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BOOL)+
			sizeof(UINT); }

	public:
		//ʹ�����ݽӿ�
		VOID				SetCanRelive(BOOL bCanRelive) { m_bCanRelive = bCanRelive; }
		BOOL				IsCanRelive(VOID)const { return m_bCanRelive; }

		VOID				SetTime(UINT uTime) { m_uTime = uTime; }
		UINT				GetTime(VOID)const { return m_uTime; }

	private:
		BOOL			m_bCanRelive;		// �Ƿ���Ը���
		UINT			m_uTime;			// ʱ��
	};


	class GCPlayerDieFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerDie() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYER_DIE; }
		UINT		GetPacketMaxSize()const { return	sizeof(BOOL)+
			sizeof(UINT); }
	};

	class GCPlayerDieHandler 
	{
	public:
		static UINT Execute( GCPlayerDie* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCPLAYERDIE_H__
