// GCStudyXinfa.h
// 
// ����NPC������ID
// 
//////////////////////////////////////////////////////

#ifndef __GCSTUDYXINFA_H__
#define __GCSTUDYXINFA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCStudyXinfa : public Packet
	{
	public:
		struct _STUDERESULT_INFO   //ѧϰ��Ľ�����ı���ʲô��
		{
			UINT					m_uSpareMoney;			//	���������ʣ��Ľ�Ǯ
			UINT					m_uSpareExperience;		//	���������ʣ��ľ���
			
			short					m_idXinfa;
			short					m_StudedLevel;			// ����֮��ĵȼ�
		};

		GCStudyXinfa( ){};
		virtual ~GCStudyXinfa( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STUDYXINFA_H; }
		virtual UINT			GetPacketSize()const { return	sizeof(_STUDERESULT_INFO);}

	public:
		_STUDERESULT_INFO		GetStudeResultInfo(VOID) const {return m_StudeResult;};
		VOID					SetStudeResultInfo(_STUDERESULT_INFO *StudeResult) 
		{
			m_StudeResult.m_uSpareMoney		= StudeResult->m_uSpareMoney;
			m_StudeResult.m_uSpareExperience	= StudeResult->m_uSpareExperience;
			m_StudeResult.m_idXinfa				= StudeResult->m_idXinfa;
			m_StudeResult.m_StudedLevel			= StudeResult->m_StudedLevel;
		}

	private:

		_STUDERESULT_INFO			m_StudeResult;

	};


	class GCStudyXinfaFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStudyXinfa() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STUDYXINFA_H; };
		UINT		GetPacketMaxSize()const { return sizeof(GCStudyXinfa::_STUDERESULT_INFO);};
	};

	class GCStudyXinfaHandler 
	{
	public:
		static UINT Execute( GCStudyXinfa* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


