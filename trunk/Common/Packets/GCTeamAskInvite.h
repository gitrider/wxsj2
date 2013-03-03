#ifndef _GC_TEAMASKINVITE_H_
#define _GC_TEAMASKINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamAskInvite:	public Packet
	{
	public:
		GCTeamAskInvite()
		{
			m_uMemberSize = 0;
			memset( (void*)m_InviterInfo, 0, sizeof(m_InviterInfo) );
		}
		virtual		~GCTeamAskInvite(){}

		//���ýӿ�
		virtual BOOL				Read( SocketInputStream& iStream );
		virtual BOOL				Write( SocketOutputStream& oStream ) const;
		virtual UINT				Execute( Player* pPlayer );

		virtual PacketID_t			GetPacketID() const { return PACKET_GC_TEAMASKINVITE; }
		virtual UINT				GetPacketSize() const
		{
			UINT uSize;

			uSize = sizeof(GUID_t) + sizeof(UCHAR);

			for( INT i=0; i<m_uMemberSize; ++i )
			{
				uSize += sizeof(UCHAR) * 2 + sizeof(UINT) + sizeof(SceneID_t) + sizeof(INT);
				uSize += sizeof(CHAR) * m_InviterInfo[i].m_NickSize + sizeof(USHORT);

				if ( m_InviterInfo[i].m_DetailFlag>0 )
				{
					uSize += sizeof(UINT) * 10;
				}
			}
			
			return uSize;
		}

		struct InviterInfo
		{
			UCHAR					m_DetailFlag;					// �Ƿ�����ϸ��Ϣ
			UCHAR					m_NickSize;
			CHAR					m_szNick[MAX_CHARACTER_NAME];	// 1.�ǳ�
			UINT					m_uFamily;						// 2.����
			SceneID_t				m_Scene;						// 3.����
			INT						m_Level;						// 4.�ȼ�
			USHORT					m_uDataID;						// 5.����Ա�

			// ��������ϸ��Ϣ
			UINT					m_WeaponID;						// 7.����
			UINT					m_AssihandID;					// ������
			UINT					m_CapID;						// 8.ñ��
			UINT					m_ScapularID;					// ���
			UINT					m_ArmourID;						// 9.�·�
			UINT					m_CuffID;						// 10.����
			UINT					m_FootID;						// 11.ѥ��
			UINT					m_uFaceMeshID;					// 12.�沿ģ��
			UINT					m_uHairMeshID;					// 13.ͷ��ģ��
			UINT					m_uHairColor;					// 14.ͷ����ɫ
		};
	public :
		VOID						SetGUID( GUID_t guid ){	m_GUID = guid; }
		GUID_t						GetGUID( ){ return m_GUID; }

//		VOID						SetMemberCount( INT count ) { m_uMemberSize = count; }
		UCHAR						GetMemberCount( ) { return m_uMemberSize; }

		VOID						AddInviterInfo( const InviterInfo& info )
		{
			m_InviterInfo[m_uMemberSize].m_DetailFlag = info.m_DetailFlag;
			m_InviterInfo[m_uMemberSize].m_NickSize = info.m_NickSize;
			strncpy( (CHAR*)m_InviterInfo[m_uMemberSize].m_szNick, info.m_szNick, info.m_NickSize );
			m_InviterInfo[m_uMemberSize].m_uFamily = info.m_uFamily;
			m_InviterInfo[m_uMemberSize].m_Scene = info.m_Scene;
			m_InviterInfo[m_uMemberSize].m_Level = info.m_Level;
			m_InviterInfo[m_uMemberSize].m_uDataID = info.m_uDataID;

			if ( info.m_DetailFlag>0 )
			{
				m_InviterInfo[m_uMemberSize].m_WeaponID = info.m_WeaponID;
				m_InviterInfo[m_uMemberSize].m_AssihandID = info.m_AssihandID;
				m_InviterInfo[m_uMemberSize].m_CapID = info.m_CapID;
				m_InviterInfo[m_uMemberSize].m_ScapularID = info.m_ScapularID;
				m_InviterInfo[m_uMemberSize].m_ArmourID = info.m_ArmourID;
				m_InviterInfo[m_uMemberSize].m_CuffID = info.m_CuffID;
				m_InviterInfo[m_uMemberSize].m_FootID = info.m_FootID;
				m_InviterInfo[m_uMemberSize].m_uFaceMeshID = info.m_uFaceMeshID;
				m_InviterInfo[m_uMemberSize].m_uHairMeshID = info.m_uHairMeshID;
				m_InviterInfo[m_uMemberSize].m_uHairColor = info.m_uHairColor;
			}

			++m_uMemberSize;
		}
		const InviterInfo*			GetInviterInfo(INT idx)
		{
			if( 0>idx || idx>=m_uMemberSize ) { Assert( FALSE ); return NULL; }

			return &m_InviterInfo[idx];
		}

	public :
		GUID_t						m_GUID;	//������
		UCHAR						m_uMemberSize;
		InviterInfo					m_InviterInfo[MAX_TEAM_MEMBER];	//��������Ϣ
	};

	class GCTeamAskInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GCTeamAskInvite(); }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_TEAMASKINVITE; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)
															+ sizeof(UCHAR)
															+ sizeof(GCTeamAskInvite::InviterInfo)*
															MAX_TEAM_MEMBER; }			
	};

	class GCTeamAskInviteHandler
	{
		public:
			static UINT Execute(GCTeamAskInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif