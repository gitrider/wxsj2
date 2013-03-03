// CGManipulatePet.h
// 
// ����������ﷵ�ؽ��
//
//////////////////////////////////////////////////////

#ifndef __CG_MANIPULATEPET_H__
#define __CG_MANIPULATEPET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	enum ENUM_MANIPULATE_TYPE
	{
		MANIPULATE_INVALID = -1,			// ��Ч
		MANIPULATE_CREATEPET,				// �ٻ�����
		MANIPULATE_DELETEPET,				// �ջس���
		MANIPULATE_FREEPET,					// ��������
		MANIPULATE_ASKOTHERPETINFO,			// �쿴������ҵĳ�����Ϣ(�������ѵ���...)
		MANIPULATE_CREATEHORSE,				// �ٻ�����
		MANIPULATE_DELETEHORSE,				// �ջ�����
		MANIPULATE_FREEHORSE,				// �ջ�����
		MANIPULATE_HORSE_ITEM,				// ʹ����Ʒ
		MANIPULATE_HORSE_CHECKUP,			// ����������֮ǰ����client���ɳ��ʣ�û������Ӱ��
		MANIPULATE_HORSE_NICKNAME,			// �޸��ǳ�
		MANIPULATE_HORSE_OPEN_MATING_UI,	// �򿪽������
		MANIPULATE_HORSE_MATING_LOCK,		// ������������
		MANIPULATE_HORSE_MATING_UNLOCK,		// �����������
		MANIPULATE_HORSE_MATING_CONFIRM,	// ���ｻ��ȷ��
		MANIPULATE_HORSE_MATING_CANCEL,		// ���ｻ��ȡ��
		MANIPULATE_HORSE_MATING_RETRIEVE,	// �����
		MANIPULATE_NUMBERS
	};

	class CGManipulatePet : public Packet
	{
	public:

		CGManipulatePet( )
		{
		}

		~CGManipulatePet( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MANIPULATEPET ; }
		virtual UINT			GetPacketSize()const 
		{
				UINT uSize = sizeof(ObjID_t) + sizeof(PET_GUID_t) + sizeof(INT)*5 + sizeof(m_sNickName);
				return uSize;
		}

		
	public:
		VOID			SetObjID(const ObjID_t objID) { m_ObjID = objID; }
		ObjID_t			GetObjID(VOID) const { return m_ObjID; }

		VOID			SetGUID(PET_GUID_t guid){ m_PetGUID = guid; }
		PET_GUID_t		GetGUID(VOID)const{ return m_PetGUID; }

		// nType	:	ENUM_MANIPULATE_TYPE
		VOID			SetManipulateType(INT nType){ m_nType = nType; }
		INT				GetManipulateType(VOID)const{ return m_nType; }
		ObjID_t			GetCharObjID() const { return m_CharObjID; }
		VOID			SetCharObjID(ObjID_t val) { m_CharObjID = val; }
		INT				GetTypeData() const { return m_iTypeData; }
		VOID			SetTypeData(INT val) { m_iTypeData = val; }
		const CHAR*		GetNickName() const { return &m_sNickName[0]; }
		VOID			SetNickName(const CHAR* val) 
		{
			if( strlen(val) >= MAX_NICK_NAME-1 )
				strncpy( m_sNickName, val, sizeof(m_sNickName) );
			else
				strcpy( m_sNickName, val );
			m_sNickName[MAX_NICK_NAME-1] = 0;
		}
		INT				GetBagIndex() const { return m_iBagIndex; }
		VOID			SetBagIndex(INT val) { m_iBagIndex = val; }
		UINT			GetItemID() const { return m_uItemID; }
		VOID			SetItemID(UINT val) { m_uItemID = val; }
	private:
		ObjID_t			m_ObjID;					// �����ObjID;
		ObjID_t			m_CharObjID;	
		PET_GUID_t		m_PetGUID;
		INT				m_nType;
		INT				m_iTypeData;				//�ɾ���Ĳ������ͽ�������ֵ����
		CHAR			m_sNickName[MAX_NICK_NAME];	//�����ǳ�
		INT				m_iBagIndex;
		UINT			m_uItemID;

	};


	class CGManipulatePetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGManipulatePet() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MANIPULATEPET; }
		UINT		GetPacketMaxSize()const 
		{

			UINT uSize = sizeof(CGManipulatePet) - sizeof(Packet);
			return uSize;
		}
	};

	class CGManipulatePetHandler 
	{
	public:
		static UINT Execute( CGManipulatePet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
