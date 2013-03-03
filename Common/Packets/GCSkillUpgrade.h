/********************************************************************
	created:	2008/01/15
	created:	15:1:2008   14:04
	filename: 	Common\Packets\GCSkillUpgrade.h
	file path:	Common\Packets
	file base:	GCSkillUpgrade
	file ext:	h
	author:		Richard
	
	purpose:	server验证client技能学习和升级请求后向client返回结果

	因为技能分为4态：禁止、可学习、可使用、可升级。并且存在递进的关系，所以，等

	及初始化为-1代表禁止，0为可学习，1开始为可使用、并表示实际的技能等级

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
		// 获取技能ID
		//************************************
		INT		GetSkillID() const;
		//************************************
		// Method:    SetSkillID
		// FullName:  Packets::GCSkillUpgrade::SetSkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// 设置技能ID
		//************************************
		VOID	SetSkillID(INT val);
		//************************************
		// Method:    GetLevel
		// FullName:  Packets::GCSkillUpgrade::GetLevel
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// 获取技能等级
		//************************************
		INT		GetLevel() const;
		//************************************
		// Method:    SetLevel
		// FullName:  Packets::GCSkillUpgrade::SetLevel
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// 设置技能等级
		//************************************
		VOID	SetLevel(INT val);

		//************************************
		// Method:    IsSuccessful
		// FullName:  Packets::GCSkillUpgrade::IsSuccessful
		// Access:    public 
		// Returns:   BOOL
		// Qualifier:
		// 升级是否成功
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
		//技能ID
		INT			m_iSkillID;
		//技能等级
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

