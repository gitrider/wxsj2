/********************************************************************
	created:	2010/5/10
	filename: 	d:\project\project\Common\Packets\CGSkillUpgradeBySouXia.h
	file path:	d:\project\project\Common\Packets
	file base:	CGSkillUpgradeBySouXia
	file ext:	h
	author:		genghao
	
	purpose:	���û���ͨ������¼�ļ���ѧϰ����ʱ��client���ʹ���Ϣ��server������֤
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

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SOUXIASKILL_ADD ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) +  sizeof(BYTE); }
	//���Է��ʷ���
	public:
		inline BYTE	GetBagIndex(VOID)			{return m_iBagIndex;}
		inline BYTE	GetStudySkillType(VOID)		{return m_iStudySkillType;}

		inline VOID	SetBagIndex(BYTE bagIndex)	{m_iBagIndex =  bagIndex;}
		inline VOID	SetStudySkillType(BYTE studySkillType)	{ m_iStudySkillType =  studySkillType;}

	private:
		BYTE		m_iStudySkillType;//	��ǰ�򿪒���¼��ҳ����
		BYTE		m_iBagIndex;	//	������������ڵı�����������Ҫѧϰ��������������������Ƿ�ƥ�䣩
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