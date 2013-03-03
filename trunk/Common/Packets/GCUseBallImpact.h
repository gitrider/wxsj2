/********************************************************************
	created:	2007/11/27
	created:	27:11:2007   10:10
	filename: 	D:\project\projectNew\Common\Packets\GCUseBallImpact.h
	file path:	D:\project\projectNew\Common\Packets
	file base:	GCUseBallImpact 
	file ext:	h
	author:		Richard
	
	purpose:	��֪��ͬ�༼���ڴ���ʱ��Ҫ֪ͨ�ͻ��˴����ض�Ч������Ϣ�����: 

				����ID, С����������ͻ���Ӧ���ݼ���ID�ж�Ӧ��������Ч��
*********************************************************************/

#ifndef GCUseBallImpact_h__27_11_2007_10_10
#define GCUseBallImpact_h__27_11_2007_10_10

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCUseBallImpact:	public Packet
	{
	public:
		GCUseBallImpact()
		{
			m_nSkillID		= INVALID_ID;
			m_nCharacterID	= INVALID_ID;
			m_uBallNum		= 0;
		}
		virtual		~GCUseBallImpact(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_USEBALLIMPACT; }
		virtual UINT			GetPacketSize()const
		{ 
			return sizeof(SkillID_t) + sizeof(UINT) + sizeof(ObjID_t); 
		}

	public :
		//�ο�����ע��
		VOID		SetSkillID( SkillID_t nSkillID ) { m_nSkillID = nSkillID; }
		//�ο�����ע��
		SkillID_t	GetSkillID( ) { return m_nSkillID; }
		//�ο�����ע��
		VOID		SetBallNum( UINT uBallNum ) { m_uBallNum = uBallNum; }
		//�ο�����ע��
		UINT		GetBallNum( ) { return m_uBallNum; }
		//�ο�����ע��
		VOID		SetCharacterID( ObjID_t uID ) { m_nCharacterID = uID; }
		//�ο�����ע��
		ObjID_t		GetCharacterID( ) { return m_nCharacterID; }

		


	private :
		SkillID_t	m_nSkillID;			//������Ч���ļ���ID
		UINT		m_uBallNum;			//������С������
		ObjID_t		m_nCharacterID;		//����ʹ���ߵ�ID
	};

	class GCUseBallImpactFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseBallImpact(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GC_USEBALLIMPACT; }
		UINT		GetPacketMaxSize() const { return sizeof(SkillID_t) + sizeof(UINT) + sizeof(ObjID_t); }			
	};

	class GCUseBallImpactHandler
	{
	public:
		static UINT Execute(GCUseBallImpact* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif //GCUseBallImpact_h__27_11_2007_10_10
