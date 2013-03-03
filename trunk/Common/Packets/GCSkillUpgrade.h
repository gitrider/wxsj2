/********************************************************************
	created:	2008/01/15
	created:	15:1:2008   14:04
	filename: 	Common\Packets\GCSkillUpgrade.h
	file path:	Common\Packets
	file base:	GCSkillUpgrade
	file ext:	h
	author:		Richard
	
	purpose:	server��֤client����ѧϰ�������������client���ؽ��

	��Ϊ���ܷ�Ϊ4̬����ֹ����ѧϰ����ʹ�á������������Ҵ��ڵݽ��Ĺ�ϵ�����ԣ���

	����ʼ��Ϊ-1�����ֹ��0Ϊ��ѧϰ��1��ʼΪ��ʹ�á�����ʾʵ�ʵļ��ܵȼ�

*********************************************************************/
#ifndef GCSkillUpgrade_h__
#define GCSkillUpgrade_h__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSkillUpgrade:public Packet
	{

	public:
		GCSkillUpgrade();
		virtual				~GCSkillUpgrade(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SKILL_UPGRADE; }
		virtual UINT			GetPacketSize() const { return sizeof(INT)+sizeof(INT)+sizeof(BOOL); }

	public:
		//************************************
		// Method:    GetSkillID
		// FullName:  Packets::GCSkillUpgrade::GetSkillID
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// ��ȡ����ID
		//************************************
		INT		GetSkillID() const;
		//************************************
		// Method:    SetSkillID
		// FullName:  Packets::GCSkillUpgrade::SetSkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// ���ü���ID
		//************************************
		VOID	SetSkillID(INT val);
		//************************************
		// Method:    GetLevel
		// FullName:  Packets::GCSkillUpgrade::GetLevel
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// ��ȡ���ܵȼ�
		//************************************
		INT		GetLevel() const;
		//************************************
		// Method:    SetLevel
		// FullName:  Packets::GCSkillUpgrade::SetLevel
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// ���ü��ܵȼ�
		//************************************
		VOID	SetLevel(INT val);

		//************************************
		// Method:    IsSuccessful
		// FullName:  Packets::GCSkillUpgrade::IsSuccessful
		// Access:    public 
		// Returns:   BOOL
		// Qualifier:
		// �����Ƿ�ɹ�
		//************************************
		BOOL	IsSuccessful();
		//************************************
		// Method:    SetSuccessful
		// FullName:  Packets::GCSkillUpgrade::SetSuccessful
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: BOOL val
		//************************************
		VOID	SetSuccessful( BOOL val );
		
	private:
		//����ID
		INT			m_iSkillID;
		//���ܵȼ�
		INT			m_iLevel;
		BOOL		m_bSuccess;
	};

	class GCSkillUpgradeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSkillUpgrade(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SKILL_UPGRADE; }
		UINT		GetPacketMaxSize() const {return sizeof(INT)+sizeof(INT)+sizeof(BOOL); }
	};

	class GCSkillUpgradeHandler
	{
	public:
		static UINT Execute( GCSkillUpgrade* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // GCSkillUpgrade_h__

