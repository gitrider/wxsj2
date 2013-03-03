/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   11:48
	filename: 	d:\project\project_11_30\Common\Packets\CGSkillUpgrade.h
	file path:	d:\project\project_11_30\Common\Packets
	file base:	CGSkillUpgrade
	file ext:	h
	author:		Richard
	
	purpose:	���û��м���ѧϰ����������ʱ��client���ʹ���Ϣ��server������֤

	��Ϊ���ܷ�Ϊ4̬����ֹ����ѧϰ����ʹ�á������������Ҵ��ڵݽ��Ĺ�ϵ�����ԣ���

	����ʼ��Ϊ-1�����ֹ��0Ϊ��ѧϰ��1��ʼΪ��ʹ�á�����ʾʵ�ʵļ��ܵȼ�
*********************************************************************/

#ifndef CGSkillUpgrade_h__
#define CGSkillUpgrade_h__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGSkillUpgrade : public Packet 
	{
	public:
		CGSkillUpgrade( );
		virtual ~CGSkillUpgrade( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SKILL_UPGRADE ; }
		virtual UINT			GetPacketSize()const { return sizeof(INT) +  sizeof(INT); }
	//���Է��ʷ���
	public:
		//************************************
		// Method:    SkillID
		// FullName:  Packets::CGSkillUpgrade::SkillID
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// ȡ�ü���ID
		//************************************
		INT GetSkillID() const;
		//************************************
		// Method:    SkillID
		// FullName:  Packets::CGSkillUpgrade::SkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// ���ü���ID
		//************************************
		VOID SetSkillID(INT val);
		//************************************
		// Method:    Level
		// FullName:  Packets::CGSkillUpgrade::Level
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// ��ȡ���ܵȼ�
		//************************************
		INT	 GetLevel() const;
		//************************************
		// Method:    SetLevel
		// FullName:  Packets::CGSkillUpgrade::SetLevel
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// ���ü��ܵȼ�
		//************************************
		VOID SetLevel(INT val);

	private:
		INT m_iSkillID;
		INT m_iLevel;
	};


	class CGSkillUpgradeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGSkillUpgrade() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SKILL_UPGRADE ; }
		UINT		GetPacketMaxSize()const { return sizeof(CGSkillUpgrade) - sizeof(Packet); }
	};


	class CGSkillUpgradeHandler 
	{
	public:
		static UINT Execute( CGSkillUpgrade* pPacket, Player* pPlayer ); 
	};

};

using namespace Packets ;

#endif // CGSkillUpgrade_h__