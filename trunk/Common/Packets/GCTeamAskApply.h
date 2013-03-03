#ifndef _GC_TEAMASKAPPLY_H_
#define _GC_TEAMASKAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamAskApply:	public Packet
	{
	public:
		GCTeamAskApply()
		{
			m_SourNameSize = 0;
			m_DestNameSize = 0;

			memset( (void*)m_SourName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		}
		virtual		~GCTeamAskApply(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_TEAMASKAPPLY; }
		virtual UINT			GetPacketSize() const
		{
			UINT uSize;

			uSize = sizeof(GUID_t)*2 + sizeof(UCHAR)*3 + sizeof(UINT) + sizeof(SceneID_t) + sizeof(INT);
			uSize += sizeof(CHAR)*(m_SourNameSize + m_DestNameSize) + sizeof(USHORT) ;

			if ( m_DetailFlag>0 )
			{
				uSize += sizeof(UINT)*10;
			}

			return uSize;
		}

	public :

		VOID					SetSourGUID( GUID_t guid ) { m_SourGUID = guid; }
		GUID_t					GetSourGUID( ) { return m_SourGUID; }

		VOID					SetDestGUID( GUID_t guid ){ m_DestGUID = guid; }
		GUID_t					GetDestGUID( ) { return m_DestGUID; }

		VOID					SetSourName( const CHAR* pName )
		{
			strncpy( m_SourName, pName, MAX_CHARACTER_NAME-1 );
			m_SourNameSize = (UCHAR)strlen(m_SourName);
		}
		const CHAR*				GetSourName( ){ return m_SourName; }

		VOID					SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*				GetDestName( ){ return m_DestName; }

		VOID					SetFamily( UINT family ) { m_uFamily = family; }
		UINT					GetFamily( ) { return m_uFamily; }

		VOID					SetScene( SceneID_t id ){ m_Scene = id; }
		SceneID_t				GetScene( ){ return m_Scene; }

		VOID					SetLevel(INT lvl) { m_Level = lvl; }
		INT						GetLevel() { return m_Level; }

		VOID					SetDetailFlag(INT flag) { m_DetailFlag = flag; }
		UCHAR					GetDetailFlag() { return m_DetailFlag; }

		VOID					SetDataID(USHORT dataid) { m_uDataID = dataid; }
		USHORT					GetDataID() { return m_uDataID; }

		VOID					SetWeaponID(UINT weaponid) { m_WeaponID = weaponid; }
		UINT					GetWeaponID() { return m_WeaponID; }

		VOID					SetAssihandID(UINT AssihandID) { m_AssihandID = AssihandID; }
		UINT					GetAssihandID() { return m_AssihandID; }

		VOID					SetScapularID(UINT ScapularID) { m_ScapularID = ScapularID; }
		UINT					GetScapularID() { return m_ScapularID; }

		VOID					SetCapID(UINT capid) { m_CapID = capid; }
		UINT					GetCapID() { return m_CapID; }

		VOID					SetArmourID(UINT armorid) { m_ArmourID = armorid; }
		UINT					GetArmourID() { return m_ArmourID; }

		VOID					SetCuffID(UINT cuffid) { m_CuffID = cuffid; }
		UINT					GetCuffID() { return m_CuffID; }

		VOID					SetFootID(UINT footid) { m_FootID = footid; }
		UINT					GetFootID() { return m_FootID; }

		VOID					SetFaceModel(UINT uFaceMeshID) { m_uFaceMeshID = uFaceMeshID; }
		UINT					GetFaceModel() const { return m_uFaceMeshID; }

		VOID					SetHairModel(UINT uHairMeshID) { m_uHairMeshID = uHairMeshID; }
		UINT					GetHairModel() const { return m_uHairMeshID; }

		VOID					SetHairColor(UINT uHairColor) { m_uHairColor = uHairColor; }
		UINT					GetHairColor() const { return m_uHairColor; }

	public :
		GUID_t					m_SourGUID;		//������
		GUID_t					m_DestGUID;		//��������
		UCHAR					m_SourNameSize;
		UCHAR					m_DestNameSize;
		CHAR					m_SourName[MAX_CHARACTER_NAME];			// 1.����������
		CHAR					m_DestName[MAX_CHARACTER_NAME];			// ������������
		UINT					m_uFamily;								// 2.����
		SceneID_t				m_Scene;								// 3.����
		INT						m_Level;								// 4.�ȼ�
		USHORT					m_uDataID;								// 5.����Ա�
		UCHAR					m_DetailFlag;							// �Ƿ�����ϸ��Ϣ

		// ��������ϸ��Ϣ
		UINT					m_WeaponID;								// 7.������
		UINT					m_AssihandID;							// ������

		UINT					m_CapID;								// 8.ñ��
		UINT					m_ScapularID;							// ���
		UINT					m_ArmourID;								// 9.�·�
		UINT					m_CuffID;								// 10.����
		UINT					m_FootID;								// 11.ѥ��
		UINT					m_uFaceMeshID;							// 12.�沿ģ��
		UINT					m_uHairMeshID;							// 13.ͷ��ģ��
		UINT					m_uHairColor;							// 14.ͷ����ɫ
	};

	class GCTeamAskApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GCTeamAskApply(); }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_TEAMASKAPPLY; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2+
															sizeof(UCHAR)*3+
															sizeof(CHAR)*MAX_CHARACTER_NAME*2+
															sizeof(UINT)*11+
															sizeof(SceneID_t)+
															sizeof(INT)+
															sizeof(USHORT);
															}			
	};

	class GCTeamAskApplyHandler
	{
		public:
			static UINT Execute(GCTeamAskApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif