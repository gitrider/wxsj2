#ifndef __GCCHARUSESKILL_H__
#define __GCCHARUSESKILL_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharUseSkill : public Packet
	{
	public:
		GCCharUseSkill(){Reset();}
		virtual ~GCCharUseSkill(){}

		VOID Reset( VOID )
		{			
			m_nSkillID			= INVALID_ID;
			m_bIsMySelf			= FALSE;	
			m_nSenderID			= INVALID_ID;
		}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const {return PACKET_GC_CHAR_USESKILL;}
		virtual UINT			GetPacketSize()const
		{
			UINT uSize = 0;

			uSize =	sizeof(m_bIsMySelf) + sizeof(m_nSkillID) + sizeof(ObjID_t);

			return uSize;
		}
	public:
		//ʹ�����ݽӿ�
		VOID			SetSenderID(ObjID_t id) {m_nSenderID = id;}
		ObjID_t			GetSenderID(VOID)const {return m_nSenderID;}

		VOID			SetSkillID(SkillID_t id) {m_nSkillID = id;}
		SkillID_t		GetSkillID(VOID)const {return m_nSkillID;}

		VOID			SetIsMyself(BOOL bEnable) {m_bIsMySelf = bEnable;}
		BOOL			GetIsMyself(VOID)const {return m_bIsMySelf;}

	private:
		SkillID_t		m_nSkillID;			// ����ID
		BOOL			m_bIsMySelf; 		// ����һ��ǹ����ͷ�
		ObjID_t			m_nSenderID;		// ����ʩ���ߵ�ID
	};	


	class GCCharUseSkillFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharUseSkill(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHAR_USESKILL; }
		UINT		GetPacketMaxSize()const
		{
			return 	sizeof(SkillID_t) + sizeof(BOOL) + sizeof(ObjID_t);
		}
	};

	class GCCharUseSkillHandler 
	{
	public:
		static UINT Execute( GCCharUseSkill* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
