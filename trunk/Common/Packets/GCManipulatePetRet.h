// GCManipulatePetRet.h
// 
// ����������ﷵ�ؽ��
//
//////////////////////////////////////////////////////

#ifndef __GC_MANIPULATEPETRET_H__
#define __GC_MANIPULATEPETRET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCManipulatePetRet : public Packet
	{
	public:
		enum ENUM_MANIPULATEPET_RET
		{
			MANIPULATEPET_RET_INVALID = -1,
			MANIPULATEPET_RET_CAPTUREFALID,				// ��׽ʧ��
			MANIPULATEPET_RET_CAPTURESUCC,				// ��׽�ɹ�
			MANIPULATEPET_RET_CALLUPFALID,				// �ٻ�ʧ��
			MANIPULATEPET_RET_CALLUPSUCC,				// �ٻ��ɹ�
			MANIPULATEPET_RET_FREEFALID,				// ����ʧ��
			MANIPULATEPET_RET_FREESUCC,					// �����ɹ�
			MANIPULATEPET_RET_RECALLFALID,				// �ջ�ʧ��
			MANIPULATEPET_RET_RECALLSUCC,				// �ջسɹ�
			MANIPULATEPET_RET_MATINGSUCCESS,			// ����ɹ�
			MANIPULATEPET_RET_UNKNOWN,					// δ֪����
			MANIPULATEPET_RET_NO_TEAM,					// û�����
			MANIPULATEPET_RET_TEAM_MEM_COUNT,			// ������������2��, �����˲���ͬһ������
			MANIPULATEPET_RET_TEAM_LEADER,				// ���Ƕӳ�
			MANIPULATEPET_RET_AREA,						// ��NPC���벻��
			MANIPULATEPET_RET_HORSE_LOCK,				// ���뽻�����������״̬,���ܽ���
			MANIPULATEPET_RET_HORSE_SEX_SAME,			// ���뽻������Ա���ͬ
			MANIPULATEPET_RET_HORSE_HAPPINESS,			// ���뽻�������ֶȲ���100
			MANIPULATEPET_RET_HORSE_GENERATION, 		// ���뽻�������һ����
			MANIPULATEPET_RET_HORSE_LEVEL,				// ���뽻�����ȼ����5
			MANIPULATEPET_RET_HORSE_SPOUSED,			// ���뽻���������ż�Ҳ��ǶԷ�
			MANIPULATEPET_RET_HORSE_SCENE,				// ���뽻��������˲���ͬһ������
			MANIPULATEPET_RET_HORSE_LEVEL15,			// ���뽻�����ȼ��������15��
			MANIPULATEPET_RET_HORSE_CALLUP,				// ���뽻��������ǳ�ս״̬
			MANIPULATEPET_RET_HORSE_VARIATION,			// ���뽻��������Ǳ���
			MANIPULATEPET_RET_HORSE_MATINGLEVEL,		// ���뽻�����ȼ����ϴν�������20��
			MANIPULATEPET_RET_HORSE_MATING,				// ���뽻�������������������ڽ���
			MANIPULATEPET_RET_HORSE_SPOUSE,				// ��û����������ﷱֳ
			MANIPULATEPET_RET_HORSE_MATING_FINISH,		// ��˻�δ��ֳ���
			MANIPULATEPET_RET_HORSE_NO_SOLT,			// û�пո�����

			MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_OK,	// �򿪽������ɹ�
			MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL,// �򿪽������ʧ��

			MANIPULATEPET_RET_HORSE_MATING_LOCK_OK,		// �������������ɹ�
			MANIPULATEPET_RET_HORSE_MATING_LOCK_FAIL,	// ������������ʧ��
			MANIPULATEPET_RET_HORSE_OTHER_MATING_LOCK,	// �Է���������

			MANIPULATEPET_RET_HORSE_MATING_UNLOCK,		// �����������
			MANIPULATEPET_RET_HORSE_OTHER_MATING_UNLOCK,// �����������

			MANIPULATEPET_RET_HORSE_MATING_CONFIRM,		// ���ｻ��ȷ��
			MANIPULATEPET_RET_HORSE_MATING_CONFIRM_FAIL,// ���ｻ��ȷ��
			MANIPULATEPET_RET_HORSE_MATING_CANCEL,		// ���ｻ��ȡ��
			MANIPULATEPET_RET_HORSE_MATING_RETRIEVE		// ��������ɹ����
		};

		GCManipulatePetRet( )
		{
			m_bFighting = FALSE;
			m_Ret = -1;
		}

		~GCManipulatePetRet( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MANIPULATEPETRET ; }
		virtual UINT			GetPacketSize()const
		{ 
			return sizeof(PET_GUID_t) +
				sizeof(BOOL) +
				sizeof(INT) +
				sizeof(ObjID_t);
		}
		
	public:
		//ʹ�����ݽӿ�
		PET_GUID_t			GetGUID(VOID) const { return m_GUID; }
		VOID				SetGUID(PET_GUID_t guid) { m_GUID = guid; }
		BOOL				IsFighting(VOID) const { return m_bFighting; }
		INT					GetManipulateRet(VOID) const { return (ENUM_MANIPULATEPET_RET)m_Ret; }
		VOID				SetManipulateRet(INT Ret) { m_Ret = Ret; }
		VOID				SetFightingFlag(BOOL bFlag) { m_bFighting = bFlag; }
		ObjID_t				GetObjID() const { return m_ObjID; }
		VOID				SetObjID(ObjID_t val) { m_ObjID = val; }

	private:
		PET_GUID_t		m_GUID;
		BOOL			m_bFighting;	// �Ƿ��ڲ�ս״̬
		INT				m_Ret;			// ���ؽ��
		ObjID_t			m_ObjID;		
	};


	class GCManipulatePetRetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCManipulatePetRet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MANIPULATEPETRET; }
		UINT		GetPacketMaxSize()const 
		{ 
			return sizeof(PET_GUID_t) +
	     		   sizeof(BOOL) +
				   sizeof(INT) +
				   sizeof(ObjID_t);
		}
	};

	class GCManipulatePetRetHandler 
	{
	public:
		static UINT Execute( GCManipulatePetRet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
