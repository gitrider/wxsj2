/********************************************************************
	created:	2010/5/10
	filename: 	d:\project\project\Common\Packets\CGSkillUpgradeBySouXia.h
	file path:	d:\project\project\Common\Packets
	file base:	CGSkillUpgradeBySouXia
	file ext:	h
	author:		genghao
	
	purpose:	当用户有通过捜侠录的技能学习操作时，client发送此消息向server请求验证
*********************************************************************/

#ifndef _CG_SOUXIA_SKILL_ADD_H_
#define _CG_SOUXIA_SKILL_ADD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGSkillUpgradeBySouXia : public Packet 
	{
	public:
		CGSkillUpgradeBySouXia( );
		virtual ~CGSkillUpgradeBySouXia( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SOUXIASKILL_ADD ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) +  sizeof(BYTE); }
	//属性访问方法
	public:
		inline BYTE	GetBagIndex(VOID)			{return m_iBagIndex;}
		inline BYTE	GetStudySkillType(VOID)		{return m_iStudySkillType;}

		inline VOID	SetBagIndex(BYTE bagIndex)	{m_iBagIndex =  bagIndex;}
		inline VOID	SetStudySkillType(BYTE studySkillType)	{ m_iStudySkillType =  studySkillType;}

	private:
		BYTE		m_iStudySkillType;//	当前打开捜侠录的页类型
		BYTE		m_iBagIndex;	//	搜侠记忆棒所在的背包索引（想要学习的搜侠记忆棒所属类型是否匹配）
	};


	class CGSkillUpgradeBySouXiaFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGSkillUpgradeBySouXia() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SOUXIASKILL_ADD ; }
		UINT		GetPacketMaxSize()const { return sizeof(CGSkillUpgradeBySouXia) - sizeof(Packet); }
	};


	class CGSkillUpgradeBySouXiaHandler 
	{
	public:
		static UINT Execute( CGSkillUpgradeBySouXia* pPacket, Player* pPlayer ); 
	};

};

using namespace Packets ;

#endif // _CG_SOUXIA_SKILL_ADD_H_