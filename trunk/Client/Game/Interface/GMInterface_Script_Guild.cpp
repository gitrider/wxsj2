
/**	GMInterface_Script_Guild.cpp
 *	
 *	���ܣ�	�����صĲ���
 *	
 *	�޸ļ�¼��
 *			080303	���� ������� INT Guild::GuildInvite(LuaPlus::LuaState* state) 
 *					�޸� FixGuildInfo�� ʵ���޸İ������
 *			080305	DestoryFamily
 *					AgreeInvite
 *		`	080307	CharacterCheck
 *					CreateFamily
 *					CreateFamilyConfirm
 *					AdjustFamilyMember
 *					TransferMemberToFamily
 *					KickMemberFormFamily
 *					GetFamilyMembersInfo ��ü���ĳ�Ա��ϸ��Ϣ
 *			080314	�޸� OpenMenu 
 *			080320	�޸Ĵ���������̣� ���жϽ�Ǯ
 *			080327	�޸� AdjustMemberTitle ��CGCharUpdateCurTitle��Ϣ������óƺŵĲ���
 *			
 */

#include "StdAfx.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "GIUtil.h"
#include "LuaPlus.h"
#include "DB_Struct.h"
#include "GameDefine_Guild.h"
#include "GameStruct_Guild.h"
#include "Network\NetManager.h"
#include "DBC/GMDataBase.h"
#include "DataPool/GMDataPool.h"
#include "DataPool/GMDP_Struct_Guild.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerNPC.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Object/Manager/ObjectManager.h"
#include "Event/GMEventSystem.h"
#include "Procedure/GameProcedure.h"

#include "CGGuild.h"
#include "CGGuildApply.h"
#include "CGGuildJoin.h"
#include "CGCharUpdateCurTitle.h"

#include "GMInterface_Script_Talk.h"
#include "GMInterface_Script_Guild.h"
#include "Input\GMInputSystem.h"




namespace SCRIPT_SANDBOX
{
	Guild Guild::s_Guild;
	LuaPlus::LuaObject* Guild::s_pMetaTable = NULL;

	enum
	{
		ERR_GUILD_ALREADYIN_MSG = 0,	//����Ѿ���һ�������
		ERR_GUILD_NOTIN_MSG,			//��Ҳ��ڰ����
		ERR_GUILD_NOPOW_MSG,			//���Ȩ�޲���
		ERR_GUILD_NOTHAVEASSCHIEF_MSG,	//û�и�����

		ERR_GUILD_CREATE_LEVEL_TOO_LOW,	//�����ȼ�����
		ERR_GUILD_NAME_EMPTY,			//�����Ϊ��
		ERR_GUILD_NAME_INVALID,			//�����ﺬ�����д�
		ERR_GUILD_NAME_CANTUSE,			//��������ȫ���˱��еĴ�
		ERR_GUILD_DESC_EMPTY,			//����Ϊ��
		ERR_GUILD_DESC_INVALID,			//�����ﺬ�����д�
		ERR_GUILD_MONEY_NOT_ENOUGH,		//��������Ǯ����

		ERR_GUILD_JOIN_LEVEL_TOO_LOW,	//����ȼ�����
		ERR_GUILD_POW_NORECRUIT,		//û��Ȩ������
		ERR_GUILD_POW_NOEXPEL,			//û��Ȩ������
	};

	Guild::Guild()
	{
		m_MsgArray.resize(0);

		m_iMemberBak	= INVALID_ID;
		m_iFamilyNum	= 0;
		m_iCurFamilyNum	= INVALID_ID;
		m_iMemberSelect	= INVALID_ID;
	}

	Guild::~Guild()
	{
	}

	INT Guild::CreateGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild CreateGuild Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild CreateGuild Wrong Param2");
			return -1;
		}

		STRING szGuildName = args[2].GetString();
		STRING szGuildDesc = args[3].GetString();

		if(!szGuildName.empty() && !szGuildDesc.empty())
		{
			//��ᴴ���ʸ���
			if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 20 )	// ����С��20
			{
				ShowMsg(ERR_GUILD_CREATE_LEVEL_TOO_LOW);
				state->PushInteger(-1);
				return 1;
			}

			// Ǯ����Ҳ���ý� 555555555555
			if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 500000 )
			{
				ShowMsg(ERR_GUILD_MONEY_NOT_ENOUGH);
				state->PushInteger(-1);
				return 1;
			}

			if( INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
			{

				if( !CharacterCheck( szGuildName ) )
				{
					ShowMsg(ERR_GUILD_NAME_INVALID);
					state->PushInteger(-1);
					return 1;
				}

				//�����ַ�����
				if( !CGameProcedure::s_pUISystem->CheckStringFilter( szGuildDesc, FT_NAME ) )
				{
					ShowMsg(ERR_GUILD_DESC_INVALID);
					state->PushInteger(-1);
					return 1;
				}


				m_MsgArray.push_back(szGuildName);
				m_MsgArray.push_back(szGuildDesc);

				//���ͨ��
				state->PushInteger(1);
				//��ʾȷ�Ͽ�
				CEventSystem::GetMe()->PushEvent( GE_GUILD_CREATE_CONFIRM, szGuildName.c_str() );
			}
			else
			{
				//�Ѿ�ӵ��һ�����MSG
				ShowMsg(ERR_GUILD_ALREADYIN_MSG);
				state->PushInteger(-1);
			}
		}
		else
		{
			if(szGuildName.empty())
			{
				ShowMsg(ERR_GUILD_NAME_EMPTY);
				state->PushInteger(-1);
			}

			if(szGuildDesc.empty())
			{
				ShowMsg(ERR_GUILD_DESC_EMPTY);
				state->PushInteger(-1);
			}
		}

		return 1;
	}

	// ��������
	INT	Guild::CreateFamily(LuaPlus::LuaState* state)
	{
		// ���崴���ʸ���
		if( -1 == m_iMemberSelect )
		{
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "��ûѡ���ɫ�������� ����" );
			return 0;
		}


		// �Ƿ�����˰��
		INT nGuildId = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
		if( INVALID_ID != nGuildId )
		{
			// �ͻ������Ȩ�޼��
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{

				// ��ԱС��3���޷�����
				if( m_AllMembers.size() >= 3 )
				{
					// ��ʾ��������Ի���
					CEventSystem::GetMe()->PushEvent( GE_GUILD_CREATE_FAMILY, m_AllMembers[m_iMemberSelect] );	// ͨ��ѡ���������ȡ���峤����
				}
				else
				{
					CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "����Ա����3�ˣ� �޷��������� ����" );
					return 1;
				}
			}
			else
			{
				//Ȩ�޲���
				ShowMsg( ERR_GUILD_NOPOW_MSG );
				return 1;
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�㻹δ������ ����" );
			return 1;
		}

		return 1;
	}


	INT Guild::CreateGuildConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild CreateGuildConfirm Wrong Param1");
			return -1;
		}

		INT confirmId = args[2].GetInteger();
		if(1 == confirmId && m_MsgArray.size() == 2) //create
		{

			//���ʹ��������Ϣ��
			CGGuildApply pk;

			pk.SetGuildNameSize((BYTE)m_MsgArray[0].size());
			pk.SetGuildName((CHAR*)m_MsgArray[0].c_str());		// 
			pk.SetGuildDescSize((BYTE)m_MsgArray[1].size());
			pk.SetGuildDesc((CHAR*)m_MsgArray[1].c_str());

			CNetManager::GetMe()->SendPacket(&pk);
		}
		else if(confirmId == 2)	//destory
		{
			//���Ͱ��ɾ����
			CGGuild ck;
			ck.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DISMISS;

			CNetManager::GetMe()->SendPacket(&ck);
		}
		else if(confirmId == 3) //quit
		{
			//�����˳�����
			CGGuild dk;
			
			dk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_LEAVE;
			GUILD_CGW_LEAVE* pLeave = (GUILD_CGW_LEAVE*)dk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_LEAVE);

			if(pLeave)
			{
				CNetManager::GetMe()->SendPacket(&dk);
			}
		}

		m_MsgArray.clear();
		return 0;
	}

	// ȷ�ϴ������� 
	INT Guild::CreateFamilyConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param2");
			return -1;
		}

		STRING szFamilyName = args[2].GetString();
		STRING szFamilyDesc = args[3].GetString();

		if( !szFamilyName.empty() && !szFamilyDesc.empty() )
		{
			if( !CharacterCheck( szFamilyName ) )
			{
				ShowMsg(ERR_GUILD_NAME_INVALID);
				return 1;
			}

			//�����ַ�����
			if( !CGameProcedure::s_pUISystem->CheckStringFilter( szFamilyDesc, FT_NAME ) )
			{
				ShowMsg(ERR_GUILD_DESC_INVALID);
				return 1;
			}


			// ���ʹ����������Ϣ

			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_CREATE_FAMILY;

			GUILD_CGW_CREATE_FAMILY* pCreateFamily = (GUILD_CGW_CREATE_FAMILY*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_CREATE_FAMILY );
			if( pCreateFamily )
			{
				pCreateFamily->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();

				pCreateFamily->m_ShaikhIndex = m_AllMembers[m_iMemberSelect];
				pCreateFamily->m_ShaikhGuid = CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_AllMembers[m_iMemberSelect])->m_Guid;

				pCreateFamily->m_cDescSize = (UCHAR)strlen( szFamilyDesc.c_str() );
				strncpy( pCreateFamily->m_szFamilyDesc, szFamilyDesc.c_str(), MAX_GUILD_FAMILY_DESC_SIZE * sizeof(CHAR) );
				pCreateFamily->m_szFamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE] = 0 ;

				pCreateFamily->m_cNameSize = (UCHAR)strlen( szFamilyName.c_str() );
				strncpy( pCreateFamily->m_szFamilyName, szFamilyName.c_str(), MAX_GUILD_FAMILY_NAME_SIZE * sizeof(CHAR) );
				pCreateFamily->m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE] = 0 ;


				CNetManager::GetMe()->SendPacket( &pk );
			}

		}

		return 0;
	}


	// ��World��������ϸ��Ϣ
	INT Guild::AskGuildDetailInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;

			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_INFO;

				STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

				pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
				strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
				pAskInfo->m_Password[MAX_PWD] = 0 ;

				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}

	// ��World�������Ա��Ϣ
	INT Guild::AskGuildMembersInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO;
				
				STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

				pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
				strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
				pAskInfo->m_Password[MAX_PWD] = 0 ;

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);

			CDataPool::GetMe()->Guild_ClearMemInfo();

			// ��ʾ�ս���
//			CEventSystem::GetMe()->PushEvent( GE_GUILD_SHOW_MEMBERINFO );
			CEventSystem::GetMe()->PushEvent( GE_UNITE_GF_SHOW, 1 );
		}

		return 0;
	}

	// ��World������ְλ��Ϣ
	INT Guild::AskGuildAppointPosInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild AskGuildAppointPosInfo Wrong Param1");
			return -1;
		}
		

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// �ͻ������Ȩ�޼���
			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_ASSIGN )	// ְ�����Ȩ
			{
				CGGuild pk;
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

				GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
				if(pAskInfo)
				{
					pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
					pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_APPOINT_POS;	// ����п�������ְλ

					STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

					pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
					strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
					pAskInfo->m_Password[MAX_PWD] = 0 ;

					CNetManager::GetMe()->SendPacket(&pk);
				}

				m_iMemberBak = args[2].GetInteger();
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;
	}

	// �����λ������
	INT Guild::ChangeGuildLeader(LuaPlus::LuaState* state)
	{
		return 0;
	}

	// ���ɾ��
	INT Guild::DestoryGuild(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//�ͻ������Ȩ�޼���
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				CEventSystem::GetMe()->PushEvent(GE_GUILD_DESTORY_CONFIRM, CDataPool::GetMe()->Guild_GetName());
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;		
	}

	// �޸İ����Ϣ����ּ��
	INT Guild::FixGuildInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild FixGuildInfo Wrong Param2");
			return -1;
		}
		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild FixGuildInfo Wrong Param3");
			return -1;
		}

		STRING strOp2 = args[2].GetString();
		STRING strOp3 = args[3].GetString();

		if( strOp2.empty() || strOp3.empty() ) 
			return 0;


		// �Ƿ��а��
		INT nGuild = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
		if( INVALID_ID != nGuild )
		{
			// ֻ�а��������޸�
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{
				if( strOp2 == "Desc" )
				{
					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_MODIFY_DESC;	//

					GUILD_CGW_MODIFY_DESC* pModifyDesc = (GUILD_CGW_MODIFY_DESC*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_MODIFY_DESC );
					if( pModifyDesc )
					{
						pModifyDesc->m_DescSize = (UCHAR)strlen( strOp3.c_str() );

						strncpy( pModifyDesc->m_szGuildDesc, strOp3.c_str(), MAX_GUILD_DESC_SIZE * sizeof(CHAR) );
						pModifyDesc->m_szGuildDesc[MAX_GUILD_DESC_SIZE] = 0 ;

						CNetManager::GetMe()->SendPacket( &pk );
					}

				}
				
			}
		}

		return 1;
	}


	// �޸����а���ְλ
	INT Guild::AdjustMemberAuth(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			// �ı�ְλʱ�ı�����Ҫ�޸��ĸ���ҵ�ְλ��Ϣ
			m_iMemberBak = -1;
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param1");
			return -1;
		}

		INT lidx = args[2].GetInteger();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// �ͻ������Ȩ�޼���
			//�Ƿ��ǰ���
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
//			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_ASSIGN )	// ְ�����Ȩ
			{
				if( lidx >= 0 && CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx) && m_iMemberBak >= 0 )
				{
					// ��ð��ɿ�����ְλ
					GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx);

					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_APPOINT;	// �������

					GUILD_CGW_APPOINT* pAuth = (GUILD_CGW_APPOINT*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_APPOINT);

					if( pAuth && pAppoint && CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak) )
					{
						// ����ְλ�����
						pAuth->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();		// ��ȡ���ID
						pAuth->m_CandidateGUID = CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak)->m_Guid;		// ͨ��������ð�����Ϣ
						pAuth->m_NewPosition = (UCHAR)pAppoint->m_PosID;

						CNetManager::GetMe()->SendPacket(&pk);
					}
				}
			}
			else
			{
				//Ȩ�޲���
				ShowMsg(ERR_GUILD_NOPOW_MSG);
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		m_iMemberBak = -1;
		return 0;
	}

	// �޸İ���Ա�ĳƺ�
	INT Guild::AdjustMemberTitle(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param2");
			return -1;
		}

		INT idx			= args[2].GetInteger();
		STRING sTitle	= args[3].GetString();


		// �������ĳƺźϷ���
		if( !CharacterCheck( sTitle ) )
		{
			ShowMsg( ERR_GUILD_NAME_INVALID );
			return 1;
		}


		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// �����б���ѡ��ĳ�Ա
			// ���͵Ĳ����� ����ID��Ҫ�޸ĵĳ�ԱID���ƺ�
			
			// ��ûȷ����ô���б��л�ȡ��Ϣ������ �� ���ƣ�


			// ����������ȡ��Ա����
			GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

			if( pMember && pMember->m_Guid != INVALID_ID )
			{
				// Ȩ���ж�
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_AUTHORIZE )	// Ȩ�޵���Ȩ Ӧ�ù���
				{

					// ���³ƺ�
					CGCharUpdateCurTitle Msg;


					Msg.setTitleType( _TITLE::BANGPAI_TITLE );	// ���ɳƺ�
//					Msg.setTitleData( 0 );
//					Msg.setTitle( sTitle.c_str() );
					Msg.setTitleID( CDataPool::GetMe()->Title_GetCurTitle( _TITLE::BANGPAI_TITLE ) );
					Msg.setGuid( pMember->m_Guid );


					Msg.setTitleSize( (UCHAR)strlen( sTitle.c_str() ) );
					Msg.setTitleName( sTitle.c_str() );

					CNetManager::GetMe()->SendPacket( &Msg );

				}
				else
					//Ȩ�޲���
					ShowMsg( ERR_GUILD_POW_NORECRUIT );

			}
		}
		else
		{
			// ���ڰ��MSG
			ShowMsg( ERR_GUILD_NOTIN_MSG );
		}

		return 0;
	}

	// �����Ա����Ϣ
	INT	Guild::GetFamilyMembersInfo(LuaPlus::LuaState* state)
	{
		return 0;
	}

	//����ɾ��
	INT	Guild::DestoryFamilyConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild DestoryFamily Wrong Param1");
			return -1;
		}

		INT familyIdx = args[2].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();
		if( familyIdx > nFamilyNum )
		{
			return 0;
		}

		STRING strFamilyName = "";

		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( familyIdx );
		if( familyInfo )
		{
			strFamilyName = familyInfo->m_sFamilyName;
		}


		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			//�ͻ������Ȩ�޼���
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				// ����ѡ��ļ���
				CDataPool::GetMe()->Guild_SetSelectFamilyId( familyIdx );

				CEventSystem::GetMe()->PushEvent( GE_GUILD_DESTORY_FAMILY_CONFIRM, CDataPool::GetMe()->Guild_GetName(), strFamilyName.c_str() );
			}
		}
		else
		{
			//���ڰ��MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}
	INT	Guild::DestoryFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		INT familyIdx = CDataPool::GetMe()->Guild_GetSelectFamilyId( );

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();
		if( familyIdx > nFamilyNum )
		{
			return 0;
		}


		GuildDetailInfo_t* guildInfo = CDataPool::GetMe()->Guild_GetDetailInfo();
		if( guildInfo )
		{
		}


		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_FAMILY_DISMISS;	// ɾ������

		GUILD_CGW_FAMILY_DISMISS* pDestoryFamily = (GUILD_CGW_FAMILY_DISMISS*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_FAMILY_DISMISS );
		if( pDestoryFamily )
		{
			pDestoryFamily->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();;
			pDestoryFamily->m_nFamilyID	= familyIdx;

			CNetManager::GetMe()->SendPacket( &pk );
		}

		return 0;
	}

	// ��ӻ�ɾ�������Ա
	INT Guild::AdjustFamilyMember(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param2");
			return -1;
		}

/*
		// ���
		if(  )
		{
			// ��ʾȷ�϶Ի���
		}
		// ɾ��
		else 
		{
			// ��ʾɾ���Ի���
		}
*/

		return 0;
	}

	// �ƶ���Ա������
	INT Guild::TransferMemberToFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild TransferMemberToFamily Wrong Param1");
			return -1;
		}

		if(!(args[3].IsInteger()))
		{
			KLThrow("LUA: Guild TransferMemberToFamily Wrong Param2");
			return -1;
		}


		INT familyIdx	= args[2].GetInteger();
		INT idx			= args[3].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if( pMember && pMember->m_Guid != INVALID_ID )
		{
			// ���ͼ�����ӳ�Ա��Ϣ
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ADD_FAMILY_USER;

			GUILD_CGW_ADD_FAMILY_USER* pAddFamily = (GUILD_CGW_ADD_FAMILY_USER*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_ADD_FAMILY_USER );
			if( pAddFamily )
			{
				pAddFamily->m_cFamilyIndex = familyIdx;			// ����id
				pAddFamily->m_iGuildIndex  = idx;				// Ҫ��ӵĳ�Ա�ڰ���Ա�б��е�����
				pAddFamily->m_TargetGuid   = pMember->m_Guid;	// Ҫ��ӵĳ�Ա�� server id

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}

		return 0;
	}

	// �Ӽ������߳�
	INT Guild::KickMemberFormFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild KickMemberFormFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild KickMemberFormFamily Wrong Param2");
			return -1;
		}

		INT familyIdx	= args[2].GetInteger();
		INT idx			= args[3].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if( pMember && pMember->m_Guid != INVALID_ID )
		{
			// ���ͼ���ɾ����Ա��Ϣ
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_REMOVE_FAMILY_USER;

			GUILD_CGW_REMOVE_FAMILY_USER* pRemoveFamily = (GUILD_CGW_REMOVE_FAMILY_USER*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_REMOVE_FAMILY_USER );
			if( pRemoveFamily )
			{
				pRemoveFamily->m_cFamilyIndex = familyIdx;
				pRemoveFamily->m_iGuildIndex  = idx;
				pRemoveFamily->m_TargetGuid   = pMember->m_Guid;

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}

		return 0;
	}

	//��������
	INT Guild::InviteMemberToFamily(LuaPlus::LuaState* state)
	{
		return 0;
	}

	//��������ȷ��
	INT Guild::FamilyInviteConfirm(LuaPlus::LuaState* state)
	{
		return 0;
	}

	INT Guild::GetFamilyMemberNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

		if( idx < 0 || idx > nFamilyNum )
			return 0;


		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( idx );

		if( familyInfo )
		{
			state->PushInteger( familyInfo->m_iFamilyUserNum );
		}

		return 1;
	}

	//��ȡ�����Ա�ڳ�Ա�б��е�����
	INT Guild::GetFamilyMemberIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param1");
			return -1;
		}

		if(!(args[3].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param2");
			return -1;
		}


		INT idx			= args[2].GetInteger();
		INT nFamilyIdx	= args[3].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

		if( nFamilyIdx < 0 || nFamilyIdx > nFamilyNum )
			return 0;

		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( nFamilyIdx );

		if( familyInfo )
		{
			if( idx >= 0 && idx < familyInfo->m_iFamilyUserNum )
			{
				state->PushInteger( familyInfo->m_aFamilyUser[idx] );
			}
		}

		return 1;
	}


	INT Guild::SetLuaFamilyNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild SetLuaFamilyNum Wrong Param1");
			return -1;
		}

		m_iCurFamilyNum = args[2].GetInteger();

		return 0;
	}
	INT Guild::GetLuaFamilyNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( INVALID_ID == m_iCurFamilyNum )
			m_iCurFamilyNum = 0;	// CDataPool::GetMe()->Guild_GetFamilyNum();

		state->PushInteger( m_iCurFamilyNum );

		return 1;
	}

	// ���洴���ļ��崰������
	INT Guild::SetFamilyWindowData(LuaPlus::LuaState* state)
	{
		// ����
		// ����
		// λ��

		INT		index = 0;
		STRING	name = "";

		CDataPool::GetMe()->Guild_AddFamilyWindowData( index, name );
		
		

		return 0;
	}
	// 
	INT Guild::GetFamilyWindowData(LuaPlus::LuaState* state)
	{
		INT		index = 0;
		STRING	name = "";

		name = CDataPool::GetMe()->Guild_GetFamilyWindowData( index );

		state->PushString( name.c_str() );

		return 1;
	}
	// ɾ�������ָ������
	INT Guild::DelFamilyWindowData(LuaPlus::LuaState* state)
	{
		INT		index = 0;

		CDataPool::GetMe()->Guild_DelFamilyWindowData( index );

		return 0;
	}



	INT Guild::GetGuildInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetGuildInfo Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild GetGuildInfo Wrong Param2");
			return -1;
		}

		INT idx = args[2].GetInteger();
		STRING strOp = args[3].GetString();

		if( idx >= CDataPool::GetMe()->Guild_GetInfoNum() ) 
			return 0;

		if( strOp.empty() ) 
			return 0;


		// �����Ϣ
		GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(idx);

		if(pGuild && pGuild->m_GuildID != INVALID_ID)
		{
			if(strOp == "ID")
			{
				state->PushInteger(pGuild->m_GuildID);
			}
			else if(strOp == "ChiefName")
			{
				state->PushString(pGuild->m_ChiefName.c_str());
			}
			else if(strOp == "Name")
			{
				state->PushString(pGuild->m_GuildName.c_str());
			}
			else if(strOp == "Desc")
			{
				state->PushString(pGuild->m_GuildDesc.c_str());
			}
			else if(strOp == "CityName")
			{
				state->PushString(pGuild->m_CityName.c_str());
			}
			else if(strOp == "Status")
			{
				state->PushInteger((INT)pGuild->m_uGuildStatus);
			}
			else if(strOp == "Level")
			{
				state->PushInteger((INT)pGuild->m_bGuildLevel);
			}
			else if(strOp == "FoundTime")
			{
				STRING strTime;
				ConvertServerTime(pGuild->m_nFoundedTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Count")
			{
				state->PushInteger((INT)pGuild->m_uGuildUserCount);
			}
		}
		else
		{
			if(strOp == "ID")
			{
				state->PushInteger(0);
			}
			else if(strOp == "ChiefName")
			{
				state->PushString("");
			}
			else if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Desc")
			{
				state->PushString("");
			}
			else if(strOp == "CityName")
			{
				state->PushString("");
			}
			else if(strOp == "Status")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Level")
			{
				state->PushInteger(0);
			}
			else if(strOp == "FoundTime")
			{
				state->PushString("");
			}
			else if(strOp == "Count")
			{
				state->PushInteger(0);
			}
		}

		return 1;
	}

	INT Guild::GetGuildNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Guild_GetInfoNum());
		return 1;
	}

	INT Guild::GetMembersInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetMembersInfo Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild GetMembersInfo Wrong Param2");
			return -1;
		}

		INT idx = args[2].GetInteger();
		STRING strOp = args[3].GetString();

		// ����Ա��Ϣ
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		
		if( pMember && strOp == "GUID" )
		{
			state->PushInteger( (INT)pMember->m_Guid );
		}
		else if( pMember && pMember->m_Guid != INVALID_ID )
		{
			if(strOp == "Name")
			{
				state->PushString(pMember->m_szName.c_str());
			}
			else if(strOp == "Level")
			{
				state->PushInteger(pMember->m_bLevel);
			}
			else if(strOp == "MenPai")
			{
				state->PushInteger(pMember->m_bMenPaiID);
			}
			else if(strOp == "CurCon")
			{
				state->PushInteger(pMember->m_iCurContribute);
			}
			else if(strOp == "MaxCon")
			{
				state->PushInteger(pMember->m_iMaxContribute);
			}
			else if(strOp == "JoinTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iJoinTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "LogOutTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iLogOutTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Position")
			{
				state->PushInteger(pMember->m_bPosition);
			}
			else if(strOp == "Online")
			{
				state->PushInteger(pMember->m_bIsOnline);
			}
			else if(strOp == "ShowColor")
			{
				state->PushString(GetShowColor_For_Lua(idx).c_str());
			}
			else if(strOp == "Family")
			{
				// ��������
				GuildFamilyInfo* pFamily = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( pMember->m_iFamilyID );
				if( pFamily )
				{
					state->PushString( pFamily->m_sFamilyName.c_str() );
				}
				else
				{
					state->PushString( "�޼���" );
				}
			}
			else if(strOp == "Title")
			{
				state->PushString( pMember->m_szTitleName.c_str() );
			}
			else if(strOp == "Sex")
			{
				state->PushInteger( pMember->m_iSex );
			}
		}

		return 1;
	}

	// ����Լ��İ�����Ϣ
	INT Guild::GetMyGuildInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild GetMyGuildInfo Wrong Param1");
			return -1;
		}
		STRING strOp = args[2].GetString();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			if(strOp == "Name")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetName());
			}
			else if(strOp == "Desc")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetDesc());
			}
			else if(strOp == "Appoint")
			{
				if(!(args[3].IsInteger()))
				{
					KLThrow("LUA: Guild GetMyGuildInfo Wrong Param2");
					return -1;
				}
				INT idx = args[3].GetInteger();

				GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(idx);
				if(pAppoint && pAppoint->m_PosID != -1)
				{
					state->PushString(pAppoint->m_szPosName.c_str());
				}
			}
			else if(strOp == "Position")
			{
				INT nGuildPos = CDataPool::GetMe()->Guild_GetCurPosition();
				if(  nGuildPos >= 0 || nGuildPos < GUILD_POSITION_SIZE )
					state->PushInteger( nGuildPos );
				else
					state->PushInteger( -1 );
			}
		}
		else
		{
			if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Desc")
			{
				state->PushString("��δ������ᡣ");
			}
			else if(strOp == "Appoint")
			{
				state->PushString("");
			}
			else if(strOp == "Position")
			{
				state->PushInteger( -1 );
			}
		}

		return 1;
	}

	INT Guild::GetMyGuildDetailInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild GetMyGuildDetailInfo Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			GuildDetailInfo_t* pDetail = CDataPool::GetMe()->Guild_GetDetailInfo();
			if( pDetail )
			{
				if(strOp == "Name")
				{
					state->PushString(pDetail->m_GuildName.c_str());
				}
				else if(strOp == "Creator")
				{
					state->PushString(pDetail->m_GuildCreator.c_str());
				}
				else if(strOp == "ChiefName")
				{
					state->PushString(pDetail->m_GuildChairMan.c_str());
				}
				else if(strOp == "CityName")
				{
					state->PushString(pDetail->m_CityName.c_str());
				}
				else if(strOp == "Level")
				{
					state->PushInteger((INT)pDetail->m_nLevel);
				}
				else if(strOp == "Scene")
				{
					state->PushInteger(pDetail->m_nPortSceneID);
				}
				else if(strOp == "MemNum")
				{
					state->PushInteger(pDetail->m_MemNum);
				}
				else if(strOp == "Lon")
				{
					state->PushInteger(pDetail->m_Longevity);
				}
				else if(strOp == "Con")
				{
					state->PushInteger(pDetail->m_Contribute);
				}
				else if(strOp == "Honor")
				{
					state->PushInteger(pDetail->m_Honor);
				}
				else if(strOp == "FMoney")
				{
					state->PushInteger(pDetail->m_FoundedMoney);
				}
				else if(strOp == "Ind")
				{
					state->PushInteger(pDetail->m_nIndustryLevel);
				}
				else if(strOp == "Agr")
				{
					state->PushInteger(pDetail->m_nAgrLevel);
				}
				else if(strOp == "Com")
				{
					state->PushInteger(pDetail->m_nComLevel);
				}
				else if(strOp == "Def")
				{
					state->PushInteger(pDetail->m_nDefLevel);
				}
				else if(strOp == "Tech")
				{
					state->PushInteger(pDetail->m_nTechLevel);
				}
				else if(strOp == "Ambi")
				{
					state->PushInteger(pDetail->m_nAmbiLevel);
				}
			}// end of if(pDetail)
		}
		else
		{
			if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Creator")
			{
				state->PushString("");
			}
			else if(strOp == "ChiefName")
			{
				state->PushString("");
			}
			else if(strOp == "CityName")
			{
				state->PushString("");
			}
			else if(strOp == "Level")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Scene")
			{
				state->PushInteger(-1);
			}
			else if(strOp == "MemNum")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Lon")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Con")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Honor")
			{
				state->PushInteger(0);
			}
			else if(strOp == "FMoney")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Ind")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Agr")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Com")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Def")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Tech")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Ambi")
			{
				state->PushInteger(0);
			}
		}

		return 1;
	}

	INT Guild::GetMyGuildPower(LuaPlus::LuaState* state)
	{
		const char* szOk = "1";
		const char* szFalse = "0";

		STRING strR = "";
		BYTE pow = CDataPool::GetMe()->Guild_GetCurAccess();
		
		strR += ( pow & GUILD_AUTHORITY_ASSIGN )	? szOk:szFalse;	// ְ�����Ȩ
		strR += ( pow & GUILD_AUTHORITY_AUTHORIZE )	? szOk:szFalse;	// Ȩ�޵���Ȩ
		strR += ( pow & GUILD_AUTHORITY_RECRUIT )	? szOk:szFalse;	// ���հ���Ȩ
		strR += ( pow & GUILD_AUTHORITY_EXPEL )		? szOk:szFalse;	// ��������Ȩ
		strR += ( pow & GUILD_AUTHORITY_DEMISE )	? szOk:szFalse;	// ����Ȩ
		strR += ( pow & GUILD_AUTHORITY_WITHDRAW )	? szOk:szFalse;	// ֧ȡ����Ȩ
		strR += ( pow & GUILD_AUTHORITY_DEPOSIT )	? szOk:szFalse;	// ������Ȩ
		strR += ( pow & GUILD_AUTHORITY_LEAVE )		? szOk:szFalse;	// �뿪���Ȩ
		strR += ( pow & GUILD_AUTHORITY_DIMISS )	? szOk:szFalse;	// ��ɢ���Ȩ

        state->PushString(strR.c_str());

		return 1;
	}

	INT Guild::GetMembersNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetMembersNum Wrong Param1");
			return -1;
		}
		INT sel = args[2].GetInteger();

		enum
		{
			TOTAL_NUMBERS = 1,	//ȫ�����ڣ������ֵ�����Ѿ����߳���ԭ���ڵ���Ϣ
			MAX_NUMBERS,		//�����ڣ���ͬ����ܰ��������������ǲ�ͬ��

			REAL_NUMBERS,		//��ʵ���ڣ���������Ч�����ݣ����������е��˺���ʽ����

			SHOW_MEMBERS_NUMBERS,	//Lua��ʾ��Ա��list�ؼ���Ҫ������
			SHOW_TRAINEES_NUMBERS,	//Lua��ʾԤ����list�ؼ���Ҫ������
			SHOW_FAMILY_NUMBERS,	
		};

		INT Num = 0;
		switch(sel) 
		{
		case TOTAL_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMemInfoNum();
			break;
		case MAX_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMaxMemNum();
			break;
		case REAL_NUMBERS:
			for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
			{
				GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
				if(pMember && pMember->m_Guid != INVALID_ID &&
				   pMember->m_bIsOnline == 1 && pMember->m_bPosition > GUILD_POSITION_TRAINEE)	// Ԥ�����ڣ�����׼
				Num++;
			}
			break;
		case SHOW_MEMBERS_NUMBERS:
			Num = (INT)m_AllMembers.size();
			break;
		case SHOW_TRAINEES_NUMBERS:
			Num = (INT)m_AllTrainees.size();
			break;
		// ����м��������
		case SHOW_FAMILY_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetFamilyNum();
			break;

		default:
			Num = -1;
			break;
		};

		state->PushInteger(Num);
		return 1;
	}

	// Lua��ʾ��list�ؼ���λ��
	INT Guild::GetShowMembersIdx(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetShowMembersIdx Wrong Param1");
			return -1;
		}
		INT showIdx = args[2].GetInteger();

		if( showIdx < 0 || showIdx >= (INT)( m_AllMembers.size() ) ) 
			return 0;

        state->PushInteger( m_AllMembers[showIdx] );
		return 1;
	}

	INT Guild::GetShowTraineesIdx(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetShowTraineesIdx Wrong Param1");
			return -1;
		}

		INT showIdx = args[2].GetInteger();

		if(showIdx < 0 || showIdx >= (INT)(m_AllTrainees.size())) return 0;

		state->PushInteger(m_AllTrainees[showIdx]);
		return 1;
	}

	// ������
	INT Guild::JoinGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild JoinGuild Wrong Param1");
			return -1;
		}

		INT selGuild = args[2].GetInteger();
		
		// �Ѿ����˾Ͳ�����������
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�Ѿ������˰�ᣬ �˳����а��������� ����" );
			return 0;
		}

		// ѡ�����Ч���ж�
		if( selGuild >= 0 && selGuild < CDataPool::GetMe()->Guild_GetInfoNum() )
		{
			GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(selGuild);
			if( pGuild && pGuild->m_GuildID != INVALID_ID )
			{
				// �ȼ���С��10
				if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 10 )
				{
					ShowMsg(ERR_GUILD_JOIN_LEVEL_TOO_LOW);
					return 0;
				}

				INT nServerId = CObjectManager::GetMe()->GetMySelf()->GetServerID();

				//���ͼ��������Ϣ��
				CGGuildJoin pk;

				pk.SetGuildID( pGuild->m_GuildID );
				pk.SetObjectID( nServerId );

				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		return 0;
	}

	// �������
	INT Guild::GuildInvite(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild GuildInvite Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();


//		if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
//		{
//			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "���Ȩ�޲�������������ʧ�� ����" );
//			return 0;
//		}


		CObject* obj = NULL;

		// Ŀ�������Ч���ж�
		{
			// ͨ�����֣� �ж��Ƿ��ж����ֵĲ���
			if( !strOp.empty() )
			{
				obj = CObjectManager::GetMe()->FindCharacterByName( strOp );
			}
			// ��ȡ��Ŀ��
			else
			{
				obj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
			}

			if( !obj )
			{
				CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "���������Ҳ����ڡ�����ʧ�� ����" );
				return 0;
			}
		}
		
		// ��ҵȼ��ж�
		if( ( (CObject_Character*)obj )->GetCharacterData()->Get_Level() < 10 )
		{
			if( "" != strOp )
			{
				// PlayerName�ȼ����㣬�޷�������
			}
			else
			{
				// ���������ҵȼ�����10��������ʧ��
			}

			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "���������ҵȼ�����10��������ʧ�� ����" );
			return 0;
		}


		if( strOp.empty() )
		{
			CObject_Character* obj_char = (CObject_Character*)(CObjectManager::GetMe()->GetMainTarget());
			strOp = obj_char->GetCharacterData()->Get_Name();
		}

		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_INVITE;	// ����

		GUILD_CGW_INVITE* pInvite = (GUILD_CGW_INVITE*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_INVITE );
		if( pInvite )
		{
			strncpy( pInvite->m_szTargetName, strOp.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
			pInvite->m_szTargetName[MAX_CHARACTER_NAME] = 0 ;

			pInvite->m_NameSize = (UCHAR)strlen( strOp.c_str() );

			CNetManager::GetMe()->SendPacket( &pk );

			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�����Ѿ������� �ȴ���һظ���" );
		}

		return 0;
	}

	// ��ʾ�����صĶԻ���
	INT Guild::ShowInputDialog(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild ShowInputDialog Wrong Param1");
			return -1;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild ShowInputDialog Wrong Param2");
			return -1;
		}

		INT nOp		= args[2].GetInteger();
		INT nMember = args[3].GetInteger();


		switch( nOp )
		{
		// ���������
		case 0:
			{
				// Ȩ���ж�
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_RECRUIT )
				{
					// ��ʾ�Ի���
					CEventSystem::GetMe()->PushEvent( GE_GUILD_INVITE_OBJECT );
				}
				else
				{
					//Ȩ�޲���
					ShowMsg( ERR_GUILD_POW_NORECRUIT );
				}
			}
			break;

		// �޸ĳ�Ա�ƺ�
		case 1:
			{
				GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex( nMember );
				

				// Ȩ���ж�
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_AUTHORIZE )	// Ȩ�޵���Ȩ Ӧ�ù���
				{
					if( pMember )
					{
						// ��ʾ�Ի���
						CEventSystem::GetMe()->PushEvent( GE_GUILD_MODIFY_TITLE, pMember->m_szName.c_str(), "", nMember );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "��Ա������ ����" );
					}
				}
				else
				{
					//Ȩ�޲���
					ShowMsg( ERR_GUILD_POW_NORECRUIT );
				}
			}
			break;
		};


		return 0;
	}

	// �յ����������Ϣ�� ��ͬ���ͬ���Ĵ���
	INT Guild::AgreeInvite(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild AgreeInvite Wrong Param2");
			return -1;
		}

		BOOL bOp = args[2].GetInteger();

		// ��ʾ��Ϣ
		if( bOp )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "��ͬ�������롣" );
		}
		else
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "��ܾ������룡" );
		}

		// GUILD_PACKET_CG_INVITE

		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_INVITE_CONFIRM;	// ȷ������

		GUILD_CGW_INVITE_CONFIRM* pInviteConfirm = (GUILD_CGW_INVITE_CONFIRM*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_INVITE_CONFIRM );
		if( pInviteConfirm )
		{
			pInviteConfirm->m_GuildId	= CDataPool::GetMe()->Guild_GetInviteGuildId( );	// 
			pInviteConfirm->m_AgreeInvite= bOp;		// 


			STRING sInviteName = CDataPool::GetMe()->Guild_GetInviteGuildObjName();

			strncpy( pInviteConfirm->m_InvitorName, sInviteName.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
			pInviteConfirm->m_InvitorName[MAX_CHARACTER_NAME] = 0 ;

			pInviteConfirm->m_InvitorNameSize = (UCHAR)strlen( sInviteName.c_str() );


			CNetManager::GetMe()->SendPacket( &pk );
		}

		return 0;
	}


	// �߳����,�ܾ�����
	INT Guild::KickGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild KickGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			// ��������Ȩ
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_EXPEL)	
			{
				// �����߳�������Ϣ��
				CGGuild pk;
				
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_EXPEL;
				GUILD_CGW_EXPEL* pExpel = (GUILD_CGW_EXPEL*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_EXPEL);
				if(pExpel)
				{
					pExpel->m_GuildUserGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket(&pk);
				}
			}
			else
			{
				//Ȩ�޲���
				ShowMsg(ERR_GUILD_POW_NOEXPEL);
			}
		}

		return 0;
	}

	// ���ɻ�Ա
	INT Guild::RecruitGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild RecruitGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		
		if( pMember && pMember->m_Guid != INVALID_ID && pMember->m_bPosition == GUILD_POSITION_TRAINEE )	// Ԥ�����ڣ�����׼
		{
			// Ȩ���ж�
			if( CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_RECRUIT )
			{
				//�������ջ�Ա����Ϣ��
				CGGuild pk;
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_RECRUIT;

				GUILD_CGW_RECRUIT* pRecruit = (GUILD_CGW_RECRUIT*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_RECRUIT );
				if( pRecruit )
				{
					pRecruit->m_ProposerGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket( &pk );
				}
			}
			else
			{
				//Ȩ�޲���
				ShowMsg(ERR_GUILD_POW_NORECRUIT);
			}
		}

		return 0;
	}

	INT Guild::QuitGuild(LuaPlus::LuaState* state)
	{
		// �ж���Ч��
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_LEAVE )	// �뿪���Ȩ�����Ȩ���е���ࣩ
			{
				// �������
				LPCTSTR pGuildName = CDataPool::GetMe()->Guild_GetName();
				// ��ʾ�˳�ȷ�Ͽ�
				CEventSystem::GetMe()->PushEvent( GE_GUILD_QUIT_CONFIRM, pGuildName );
			}
			else
			{
				//Ȩ�޲���
				ShowMsg( ERR_GUILD_NOPOW_MSG );
			}
		}
		return 0;
	}

	INT Guild::DemisGuild(LuaPlus::LuaState* state)
	{
		//�Ƿ��а��
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//�Ƿ��ǰ���
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				BOOL bAss = FALSE;
                //�����Ƿ��и�����
				for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
				{
					GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
					if(pMember && pMember->m_Guid != INVALID_ID)
					{
						if(pMember->m_bPosition == GUILD_POSITION_ASS_CHIEFTAIN)
						{
							bAss = TRUE;
							break;
						}
					}
				}

				if(bAss && CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE)
				{
					//�������ð�
					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DEMISE;
					GUILD_CGW_DEMISE* pDemis = (GUILD_CGW_DEMISE*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_DEMISE);

					if(pDemis)
					{
						CNetManager::GetMe()->SendPacket(&pk);
					}
				}

				//��ʾ������Ϣ
				if(!bAss)
				{
					ShowMsg(ERR_GUILD_NOTHAVEASSCHIEF_MSG);
				}

				if(!(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE))
				{
					ShowMsg(ERR_GUILD_NOPOW_MSG);
				}
			}
		}
		return 0;
	}

	INT	Guild::PrepareMembersInfomation(LuaPlus::LuaState* state)
	{
		PerpareMembersBeforeShow();
		return 0;
	}

	VOID Guild::ShowMsg(INT msgType, BOOL bTalk, BOOL bTip)
	{
		STRING strMsg = "";

		switch(msgType) {
		case ERR_GUILD_ALREADYIN_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_AlreadyIn");
			break;
		case ERR_GUILD_NOTIN_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Not_Guild_Member");
			break;
		case ERR_GUILD_NOPOW_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_CantDo");
			break;
		case ERR_GUILD_NOTHAVEASSCHIEF_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Change_Demiss_No_Assistant_Chief");
			break;
		case ERR_GUILD_CREATE_LEVEL_TOO_LOW:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Create");
			break;
		case ERR_GUILD_NAME_EMPTY:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_Empty_Create");
			break;
		case ERR_GUILD_NAME_INVALID:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_Invalid_Create");
			break;
		case ERR_GUILD_NAME_CANTUSE:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_CantUse_Create");
			break;
		case ERR_GUILD_DESC_EMPTY:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Desc_Empty_Create");
			break;
		case ERR_GUILD_DESC_INVALID:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Desc_Invalid_Create");
			break;
		case ERR_GUILD_MONEY_NOT_ENOUGH:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Money_Too_Low_To_Create");
			break;
		case ERR_GUILD_JOIN_LEVEL_TOO_LOW:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Join");
			break;
		case ERR_GUILD_POW_NORECRUIT:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Recruit");
			break;
		case ERR_GUILD_POW_NOEXPEL:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Expel");
			break;
		default:
			break;
		}
		
		m_MsgArray.clear();

		if(strMsg.empty()) return;
		if(!bTalk && !bTip) return;

		if(bTalk)
		{
			ADDTALKMSG(strMsg);
		}
		
		if(bTip)
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strMsg.c_str() );
		}
	}

	VOID Guild::ConvertServerTime(INT dTime, STRING& strTime)
	{
		INT min,hour,day,month,year;

		min = dTime % 100;
		dTime /= 100;

		hour = dTime % 100;
		dTime /= 100;

		day = dTime % 100;
		dTime /= 100;

		month = dTime % 100;
		dTime /= 100;

		//year = 2000 + dTime % 100;
		year = dTime % 100;
		dTime /= 100;

		//CHAR szFormat[] = "%d��%d��%d�� %d:%d";
		//CHAR szFormat[] = "%d��%d��%d��";
		CHAR szFormat[] = "%02d-%02d-%02d";
		CHAR szDate[32] = {0};
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day,hour,min);
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day);
		_snprintf(szDate,sizeof(szDate),szFormat, month,day,year);

		strTime = szDate;
	}

	STRING Guild::GetShowColor_For_Lua(INT idx)
	{
		STRING strColor;

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			switch(pMember->m_bPosition) {
			case GUILD_POSITION_CHIEFTAIN:
			case GUILD_POSITION_ASS_CHIEFTAIN:
			case GUILD_POSITION_ELITE_MEMBER:
//			case GUILD_POSITION_COM:
//			case GUILD_POSITION_AGRI:
//			case GUILD_POSITION_INDUSTRY:
//			case GUILD_POSITION_HR:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineLeaderColor:m_LuaShowColors.m_LeaveLeaderColor;
				break;
			case GUILD_POSITION_TRAINEE:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineMemberColor:m_LuaShowColors.m_LeaveMemberColor;
				break;
			case GUILD_POSITION_MEMBER:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineTraineeColor:m_LuaShowColors.m_LeaveTraineeColor;
				break;
			default:
				break;
			}
		}

		return strColor;
	}


	// ����ÿ�γ�Ա�б��е�ѡ��
	INT Guild::SetMemberListSelect(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild SetMemberListSelect Wrong Param1");
			return -1;
		}

		m_iMemberSelect = args[2].GetInteger();

		return 0;
	}

	INT Guild::OpenMenu(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild::OpenMenu Wrong Param1");
			return -1;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild::OpenMenu Wrong Param2");
			return -1;
		}

		enum
		{
			JOIN_MENU = 0,	// �����б�������
			TRANSFER_MENU,	// ת�Ƴ�Ա������
		};

		// 0	�����б����
		// 1	ת�Ƴ�Ա������
		INT nType = args[2].GetInteger();
		INT nMember = args[3].GetInteger();


		// �õ����λ��
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

		// �������
		std::vector< STRING > vParam;

		CHAR szMember[32]; _snprintf(szMember, 32, "%d", nMember);
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);

		switch( nType )
		{
		case JOIN_MENU:
			{
				vParam.push_back("GuildJoinMenu");

				vParam.push_back("-1");
				vParam.push_back( szMember );
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
			}
			break;
		case TRANSFER_MENU:
			{
				vParam.push_back("GuildTransferMenu");
				vParam.push_back("-1");
				vParam.push_back( szMember );
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);

				// ��ȡ��ǰ�ļ�������
				INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

				CHAR szStr[32]; _snprintf(szStr, 32, "%d", nFamilyNum);
				vParam.push_back(szStr);

				GuildFamilyInfo* pFamily = NULL;
				// ��ӵ��˵���
				for( INT i = 0; i < nFamilyNum; ++i )
				{
					pFamily = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( i );
					if( pFamily )
					{
						vParam.push_back( pFamily->m_sFamilyName );
					}
				}

				// ��Ǽ���������
				vParam.push_back("-1");
			}
			break;
		};

		CEventSystem::GetMe()->PushEvent( GE_SHOW_CONTEXMENU, vParam );

		return 0;
	}


	VOID Guild::PerpareMembersBeforeShow()
	{
		MEMBER_TABLE	onlineLeaders;		//�����쵼
		MEMBER_TABLE	leaveLeaders;		//�����쵼

		MEMBER_TABLE	onlineMembers;		//���߳�Ա
		MEMBER_TABLE	leaveMembers;		//���߳�Ա

		MEMBER_TABLE	onlineTrainees;		//����Ԥ����Ա
		MEMBER_TABLE	leaveTrainees;		//����Ԥ����Ա


		//clear
		m_AllMembers.clear();
		m_AllTrainees.clear();

		for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
		{
			GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
			if(pMember && pMember->m_Guid != INVALID_ID)
			{
				Name_Idx curMember;
				curMember.m_MemberIdx	= i;
				curMember.m_MemberName	= pMember->m_szName;
				curMember.m_Position	= pMember->m_bPosition;		// ְλ

				switch(pMember->m_bPosition) 
				{
				// ����
				case GUILD_POSITION_CHIEFTAIN:
				// ������
				case GUILD_POSITION_ASS_CHIEFTAIN:
				// ��Ӣ����
				case GUILD_POSITION_ELITE_MEMBER:
				// ���峤
				case GUILD_POSITION_FAMILY_BOSS:
					( pMember->m_bIsOnline > 0 ) ? onlineLeaders.push_back( curMember ) : leaveLeaders.push_back( curMember );
					break;

				// Ԥ�����ڣ�����׼
				case GUILD_POSITION_TRAINEE:
					( pMember->m_bIsOnline > 0 ) ? onlineTrainees.push_back( curMember ) : leaveTrainees.push_back( curMember );
					break;

				// ��ͨ����
				case GUILD_POSITION_MEMBER:
					( pMember->m_bIsOnline > 0 ) ? onlineMembers.push_back( curMember ) : leaveMembers.push_back( curMember );
					break;

				default:
					break;
				}
			}
		}


		//================
		// �쵼����

		// ����
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_position);
		// ����
		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_position);


		//================
		// ��ʽ��������

		std::stable_sort(onlineMembers.begin(), onlineMembers.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveMembers.begin(), leaveMembers.end(), SimpleCmpFunc::less_strcmp);

		// �������г�Ա��

		//0.�����쵼
		for(UINT i=0; i<onlineLeaders.size(); ++i)
		{
			m_AllMembers.push_back(onlineLeaders.at(i).m_MemberIdx);
		}
		//1.���߳�Ա
		for(UINT i=0; i<onlineMembers.size(); ++i)
		{
			m_AllMembers.push_back(onlineMembers.at(i).m_MemberIdx);
		}
		//2.�����쵼
		for(UINT i=0; i<leaveLeaders.size(); ++i)
		{
			m_AllMembers.push_back(leaveLeaders.at(i).m_MemberIdx);
		}
		//3.���߳�Ա
		for(UINT i=0; i<leaveMembers.size(); ++i)
		{
			m_AllMembers.push_back(leaveMembers.at(i).m_MemberIdx);
		}



		//================
		// Ԥ����������

		std::stable_sort(onlineTrainees.begin(), onlineTrainees.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveTrainees.begin(), leaveTrainees.end(), SimpleCmpFunc::less_strcmp);

		//��������Ԥ����Ա��

		//0.����Ԥ����Ա
		for(UINT i=0; i<onlineTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(onlineTrainees.at(i).m_MemberIdx);
		}
		//1.����Ԥ����Ա
		for(UINT i=0; i<leaveTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(leaveTrainees.at(i).m_MemberIdx);
		}

	}



	// �ַ��Ϸ����
	BOOL Guild::CharacterCheck( const STRING& str )
	{
		// ��ȫƥ�����
		if( !CGameProcedure::s_pUISystem->CheckStringFullCompare( str, "guild" ) )
		{
			return FALSE;
		}
        
		// �����ַ�����
		if( !CGameProcedure::s_pUISystem->CheckStringFilter( str, FT_NAME ) )
		{
			return FALSE;
		}

		// ���ƷǷ��ַ�����
		if( !KLU_CheckStringValid( str.c_str() ) )
		{
			return FALSE;
		}


		return TRUE;
	}


//  SimpleCmpFunc------------------------------------------------------------------
	BOOL SimpleCmpFunc::less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcmp(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcoll(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_position(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if(s1.m_Position <= s2.m_Position)
			return FALSE;
		else
			return TRUE;
	}
	
}