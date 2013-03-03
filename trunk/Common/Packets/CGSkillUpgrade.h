/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   11:48
	filename: 	d:\project\project_11_30\Common\Packets\CGSkillUpgrade.h
	file path:	d:\project\project_11_30\Common\Packets
	file base:	CGSkillUpgrade
	file ext:	h
	author:		Richard
	
	purpose:	当用户有技能学习和升级操作时，client发送此消息向server请求验证

	因为技能分为4态：禁止、可学习、可使用、可升级。并且存在递进的关系，所以，等

	及初始化为-1代表禁止，0为可学习，1开始为可使用、并表示实际的技能等级
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

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SKILL_UPGRADE ; }
		virtual UINT			GetPacketSize()const { return sizeof(INT) +  sizeof(INT); }
	//属性访问方法
	public:
		//************************************
		// Method:    SkillID
		// FullName:  Packets::CGSkillUpgrade::SkillID
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// 取得技能ID
		//************************************
		INT GetSkillID() const;
		//************************************
		// Method:    SkillID
		// FullName:  Packets::CGSkillUpgrade::SkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// 设置技能ID
		//************************************
		VOID SetSkillID(INT val);
		//************************************
		// Method:    Level
		// FullName:  Packets::CGSkillUpgrade::Level
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// 获取技能等级
		//************************************
		INT	 GetLevel() const;
		//************************************
		// Method:    SetLevel
		// FullName:  Packets::CGSkillUpgrade::SetLevel
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// 设置技能等级
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