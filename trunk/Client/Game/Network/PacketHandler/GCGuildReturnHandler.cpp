
/** GCGuildReturnHandler.cpp
 *
 *	���ܣ�	��������Ϣ����
 *	
 *	�޸ļ�¼��
 *			080308	GUILD_RETURN_INVIT
 *					GUILD_RETURN_CREATE_FAMILY
 *					GUILD_RETURN_ADD_FAMILY_USER
 *					GUILD_RETURN_REMOVE_FAMILY_USER
 *			080411	����ڴ����ɹ����غ��������������Ϣ
 *			
 */

#include "StdAfx.h"
#include "GCGuildReturn.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "../../Interface/GMInterface_Script_Talk.h"
#include "..\..\DataPool\GMDP_CharacterData.h"




uint GCGuildReturnHandler::Execute( GCGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		_GUILD_RETURN* pReturn = pPacket->GetGuildReturn();
		CHAR szText[_MAX_PATH];

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Other_Operate");
		_snprintf(szText, _MAX_PATH, strTemp.c_str());
		//_snprintf(szText, _MAX_PATH, "������������");

 		switch( pReturn->m_ReturnType )
		{
		// ��Ӧ�ɹ�
		case GUILD_RETURN_RESPONSE:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCGuildReturnHandler_Info_Respound_Guild_Create");
				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName);	// "%s��Ӧ�����Ľ�����
			}
			break;

		// �����ɹ�
		case GUILD_RETURN_CREATE:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Create_Success");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "�������ɹ���"

//				INT guildId = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
//				if( INVALID_ID != guildId )
//				{
//					CDataPool::GetMe()->Guild_AskInfo( guildId );
//				}
			}
			break;

		// ���������б�ȴ���׼
		case GUILD_RETURN_JOIN:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Apply_Success_Waite_For_Pass", pReturn->m_GuildName);
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "���Ѿ���%s��ᷢ�������룬��ȴ��ð�ᴦ��"
			}
			break;

		// ��������
		case GUILD_RETURN_EXPEL:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//�Լ����߳����
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					strTemp = NOPARAMMSGFUNC("GCGuildReturnHandler_Info_Kick_Guild_Member");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName, pReturn->m_DestName);
				}
				else
				{
					//���˿���
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{
							//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}

					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// �ܾ�����
		case GUILD_RETURN_REJECT:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//�Լ������뱻�ܾ���
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Refuse_Member1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);
				}
				else
				{
					//���˿���
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{
							//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_You_Kick_Other_Success");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_DestName);

					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// �뿪���
		case GUILD_RETURN_LEAVE:
			{
				//�����ݳ��в���������,�ı�ְλ
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//���Լ��˳����
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();

					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_You_Leave2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);

					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
				}
				else
				{//�������ڿ���ĳ���˳����
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					
					//���Ƶ��
					//
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// ���հ���
		case GUILD_RETURN_RECRUIT:
			{
				STRING strTemp = "";
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)GUILD_POSITION_MEMBER;
						break;
					}
				}

				CDataPool::GetMe()->Guild_SetMaxMemNum(CDataPool::GetMe()->Guild_GetMaxMemNum()+1);

				//ˢ�½���
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);

				//��������
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp	= NOPARAMMSGFUNC("GMGameInterface_Script_Guild_New_Member2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);
				}

				//������
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_New_Member1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_DestName);
				}

				strTemp = COLORMSGFUNC("GMGameInterface_Script_Guild_Welcome_New_Member", pReturn->m_DestName, pReturn->m_GuildName);
				//��Ӱ��������Ϣ
				ADD_GUILD_MSG(strTemp);
			}
			break;

		// ��ʽ�����������ﵽ��
		case GUILD_RETURN_FOUND:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Set_Up");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "�����ʽ������"
			}
			break;

		// ��ɢ���
		case GUILD_RETURN_DISMISS:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Dismiss");
				_snprintf(szText, _MAX_PATH,  strTemp.c_str());	// "����ɢ��"
			}
			break;

		// ����ְ����ְ
		case GUILD_RETURN_PROMOTE:
			{
				//����ַ���
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{
						//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//��İ��Ȩ�޸ı��ˡ�
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// ����ְ�񣬽�ְ
		case GUILD_RETURN_DEMOTE:
			{
				//����ַ���
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//��İ��Ȩ�޸ı��ˡ�
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}

				
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// ����Ȩ�ޣ���Ȩ
		case GUILD_RETURN_AUTHORIZE:
		// ����Ȩ�ޣ����Ȩ��
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{
			}
			break;

		// ����
		case GUILD_RETURN_DEMISE:
			{
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//���Ȩ�޸ı��ˣ���ϲ���Ϊ��������
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Promote_Chief");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
					break;
				}

				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//�����ɹ����Ȩ�޸ı���
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_Change_OK");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
					break;
				}

				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUIDChanged)
					{
						//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_CHIEFTAIN;
					}

					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{
						//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_MEMBER;
					}
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// �յ����˵ļӰ�����
//		case GUILD_PACKET_GC_CONFIRM_GUILD_INVITE:
//
//				if( pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
//				{
//					// ����ID�����ݳ���
//					CDataPool::GetMe()->Guild_SetInviteGuildInfo( pReturn->m_GuildID );
//					CDataPool::GetMe()->Guild_SetInviteInfo( pReturn->m_GUID );
//
//					// ����淢�Ͱ�ť��˸
//					CGameProcedure::s_pEventSystem->PushEvent( GE_GUILD_NOTIFY_INTIVE, 1 );
//				}
//				// ���˿���
//				else
//				{
//				}
//
//			break;

		// ����𸴺�Ĵ���
		case GUILD_RETURN_INVIT:
//			if( pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
			{
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Guild( pReturn->m_GuildID );
				pReturn->m_GuildName;

//				STRING strTemp = "��ͬ���˼���";
//				strTemp += pReturn->m_GuildName;
//				strTemp += "��ᡣ";

				STRING strTemp = "";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;

		// ��������
		case GUILD_RETURN_CREATE_FAMILY:
			{
				pReturn->m_cFamilyID;
				pReturn->m_FamilyNameSize;
				pReturn->m_FamilyDescSize;
				pReturn->m_FamilyName;
				pReturn->m_FamilyDesc;

				STRING strTemp = pReturn->m_FamilyName;
				strTemp += "���崴���ɹ���";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//ˢ�½���
				CEventSystem::GetMe()->PushEvent( GE_GUILD_UPDATE_MEMBERINFO, 0 );
			}
			break;
		// ����ʧ��
		case GUILD_RETURN_CREATE_FAILED:
			{
				STRING strTemp = "���崴��ʧ�ܣ�";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;
		// ��Ӽ����Ա
		case GUILD_RETURN_ADD_FAMILY_USER:
			{
				pReturn->m_cFamilyID;
				pReturn->m_SourNameSize;
				pReturn->m_SourName;

				STRING strTemp = "��ӳ�Ա";
				strTemp += pReturn->m_SourName;
				strTemp += "�ɹ���";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//ˢ�½���
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		// ɾ�������Ա
		case GUILD_RETURN_REMOVE_FAMILY_USER:
			{
				pReturn->m_cFamilyID;
				pReturn->m_SourNameSize;
				pReturn->m_SourName;

				STRING strTemp = "ɾ����Ա";
				strTemp += pReturn->m_SourName;
				strTemp += "�ɹ���";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//ˢ�½���
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// �޸İ����ּ
		case GUILD_RETURN_MODIFY_DESC:
			{
				pReturn->m_SourName;

				STRING strTemp = "�����ּ���޸�Ϊ��";
				strTemp += pReturn->m_SourName;
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break; 

		// ��ɢ����
		case GUILD_RETURN_FAMILY_DISMISS:
			{
				pReturn->m_cFamilyID;

				STRING strTemp = "����м����ѽ�ɢ��";
				strTemp += pReturn->m_SourName;
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;

		// ֧ȡ���
		case GUILD_RETURN_WITHDRAW:
		// ������
		case GUILD_RETURN_DEPOSIT:
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "���", szText, 0 );

		// ����ʾ�����ɹ�������ʾ
		if( GUILD_RETURN_CREATE == pReturn->m_ReturnType )
		{
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "���������3���ڰ������δ" );
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "�ﵽ10�����Զ���ɢ��" );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

