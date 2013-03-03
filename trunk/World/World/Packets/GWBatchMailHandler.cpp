/********************************************************************************
 *	�ļ�����	GWBatchMailHandler.cpp
 *	ȫ·����	D:\Prj\World\World\Packets\GWBatchMailHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 3 �� 4 ��	15:25
 *
 *	����˵����	���������ʼ�
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GWBatchMail.h"
#include "Log.h"
#include "MailCenter.h"
#include "OnlineUser.h"

uint GWBatchMailHandler::Execute( GWBatchMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = NULL;
	BATCH_MAIL* pBatchMail = pPacket->GetBatchMail();

	MAIL mail;
	mail.m_GUID = pBatchMail->GetGUID();
	strncpy( mail.m_szSourName, pBatchMail->GetSourName(), MAX_CHARACTER_NAME - 1 );
	mail.m_SourSize = (BYTE)strlen( mail.m_szSourName );
	strncpy( mail.m_szContex, pBatchMail->GetMailContent(), MAX_MAIL_CONTEX - 1 );
	mail.m_ContexSize = (WORD)strlen( mail.m_szContex );
	mail.m_uFlag = pBatchMail->GetMailFlag();
	mail.m_uCreateTime = pBatchMail->GetCreateTime();

	for( BYTE i=0; i<pBatchMail->GetReceiverCount(); ++i )
	{
		strncpy( mail.m_szDestName, pBatchMail->GetDestName(i), MAX_CHARACTER_NAME - 1 );
		mail.m_DestSize = (BYTE)strlen( mail.m_szDestName );

		pUser = g_pOnlineUser->FindUser( mail.m_szDestName );
		if( pUser )
		{//��ǰ�û�����
			if( pUser->RecvMail( &mail ) == FALSE )
			{
				Log::SaveLog( WORLD_LOGFILE, "GWBatchMailHandler...dName=%s refuse sName=%s's mail.",
					pUser->GetName(), mail.m_szSourName );

				return PACKET_EXE_CONTINUE;
			}
		}
		else
		{
			g_pMailCenter->MailTo( &mail );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWBatchMailHandler...sName=%s dName=%s",
			mail.m_szSourName, mail.m_szDestName );

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
