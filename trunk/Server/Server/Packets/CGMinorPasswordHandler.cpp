
#include "stdafx.h"
#include "CGMinorPasswd.h"

#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCMinorPasswd.h"
#include "TimeManager.h"

using namespace Packets;


uint CGMinorPasswdHandler::Execute( CGMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	CG_MINORPASSWD* pPasswdPacket;
	pPasswdPacket = pPacket->GetMinorPassword();

	GCMinorPasswd Msg;
	Msg.GetMinorPassword()->CleanUp();
	INT nErr = 0;

	switch( pPasswdPacket->m_Type )
	{
	case MREQT_PASSWDSETUP:					// ѯ�ʶ��������Ƿ��Ѿ�����
		{
			Msg.GetMinorPassword()->m_Type = MRETT_PASSWDSETUP;
			RETURN_MINORPASSWD_SETUP* pSend = &(Msg.GetMinorPassword()->m_ReturnSetup);
			pSend->CleanUp();
			pSend->SetSetupFlag( pHuman->__IsPasswordSetup() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X get the minor password setup info.",
				pHuman->GetGUID() );
		}
		break;
	case MREQT_DELETEPASSWDTIME:			// ѯ���Ƿ���ǿ�ƽ���׶�
		{
			UINT uRemainTime;

			uRemainTime = pHuman->__GetPasswordDeleteRemainTime();
			if ( uRemainTime == UINT_MAX )
			{
				return PACKET_EXE_CONTINUE;
			}

			Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDTIME;
			RETURN_DELETE_MINORPASSWD_TIME* pSend = &(Msg.GetMinorPassword()->m_ReturnDeleteTime);
			pSend->CleanUp();
			pSend->SetDeleteTime( uRemainTime );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X get the minor password delete time.",
				pHuman->GetGUID() );
		}
		break;

    case MREQT_SETPROTECTTYPE:              // �������뱣��ѡ��
        {
            pHuman->__SetPasswordProtect(pPacket->GetMinorPassword()->m_nFlag);
            Msg.GetMinorPassword()->m_Type = MRETT_SETPASSWDPROTECT;

            RETURN_MINORPASSWD_SETUP* pSend = &(Msg.GetMinorPassword()->m_ReturnSetup);
            pSend->CleanUp();
            pSend->SetSetupFlag( pPacket->GetMinorPassword()->m_nFlag );

            g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set the minor password protect type.",
                pHuman->GetGUID() );
        }
        break;

    case MREQT_SETPROTECTTIME:
        {
            if (pHuman->__SetProtectTime(pPacket->GetMinorPassword()->m_cTime))
            {
                Msg.GetMinorPassword()->m_Type = MRETT_SETPROTECTTIMESUCC;

                RETURN_DELETE_MINORPASSWD_TIME* pSend = &(Msg.GetMinorPassword()->m_ReturnDeleteTime);
                pSend->CleanUp();
                pSend->SetDeleteTime(pPacket->GetMinorPassword()->m_cTime);

                g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set theprotect time.",
                    pHuman->GetGUID() );
            }
            else
            {
                Msg.GetMinorPassword()->m_Type = MRETT_ERR_SETPROTECTTIMEFAIL;
            }
        }
        break;

	case MREQT_SETPASSWD:					// ���ö�������
		{
			if( pHuman->__IsPasswordSetup() )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_SETPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set the minor password duplicated.",
					pHuman->GetGUID() );
			}
			else if( pHuman->__SetPassword( pPasswdPacket->m_OnePasswd.GetPasswd() ) )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_SETPASSWDSUCC;
			}

			else
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_SETPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;
	case MREQT_MODIFYPASSWD:				// �޸Ķ�������
		{
			REQUEST_MINORPASSWD_MODIFY* pRecv = &(pPasswdPacket->m_ModifyPasswd);

			if( pHuman->ComparePasswd( pRecv->GetOldPasswd() )
			 && pHuman->__SetPassword( pRecv->GetNewPasswd() )
			 )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_MODIFYPASSWDSUCC;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X change the minor password.",
					pHuman->GetGUID() );
			}
			else
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_MODIFYPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X change the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;

    case MREQT_CLEANPASSWD:
        {
            REQUEST_MINORPASSWD_MODIFY* pRecv = &(pPasswdPacket->m_ModifyPasswd);

            if( pHuman->ComparePasswd( pRecv->GetOldPasswd() )
                && pHuman->__SetPassword( pRecv->GetNewPasswd() )
                )
            {
                Msg.GetMinorPassword()->m_Type = MRETT_CLEANPASSWDSUCC;

                RETURN_MINORPASSWD_SETUP* pSend = &(Msg.GetMinorPassword()->m_ReturnSetup);
                pSend->CleanUp();
                pSend->SetSetupFlag(0);

                g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X clean the minor password.",
                    pHuman->GetGUID() );
            }
            else
            {
                Msg.GetMinorPassword()->m_Type = MRETT_ERR_CLEANPASSWDFAIL;

                g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X clean the minor password failed.",
                    pHuman->GetGUID() );
            }
        }
        break;

	case MREQT_UNLOCKPASSWD:				// ��ʱȡ�����뱣��
		{
			if( pHuman->ComparePasswd( pPasswdPacket->m_OnePasswd.GetPasswd() ) )
			{
				pHuman->__UnlockPassword();
				Msg.GetMinorPassword()->m_Type = MRETT_UNLOCKPASSWDSUCC;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X unlock the minor password.",
					pHuman->GetGUID() );


                pHuman->__SetProtectFlag( FALSE );
			}
			else
			{
				pHuman->__LockPassword();	// ֻҪ����ʧ�ܣ�����������������ǰ�Ƿ��Ѿ�����
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_UNLOCKPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X unlock the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;
	case MREQT_DELETEPASSWD:				// ǿ�ƽ����������
		{
			if( pHuman->__GetPasswordDeleteTime() > 0
			 && pHuman->__IsValidToDeletePassword()
			)
			{								// ����� 15 �����ǿ�ƽ���������ɹ�
				pHuman->__DelPassword();
				Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDSUCC;

				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X delete the minor password success.",
					pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			else if( pHuman->__GetPasswordDeleteTime() == 0 )
			{								// ����ǿ�ƽ��ʱ��
				pHuman->__SetPasswordDeleteTime( (UINT)g_pTimeManager->GetANSITime() );
			}

			// ��ʾʣ��ʱ��
			Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDTIME;
			RETURN_DELETE_MINORPASSWD_TIME* pSend = &(Msg.GetMinorPassword()->m_ReturnDeleteTime);
			pSend->CleanUp();
			pSend->SetDeleteTime( pHuman->__GetPasswordDeleteRemainTime() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X wants to delete the minor password.",
				pHuman->GetGUID() );
		}
		break;
	default:
		return PACKET_EXE_CONTINUE;
	}

	pGamePlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
