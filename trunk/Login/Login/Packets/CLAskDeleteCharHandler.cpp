#include "stdafx.h" 
#include "CLAskDeleteChar.h"
#include "LCRetDeleteChar.h"
#include "DBDeleteCharOp.h"
#include "ProcessPlayerManager.h"
#include "LoginPlayer.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "DBCharList.h"
#include "PacketFactoryManager.h"
#include "LCRetCharList.h"
#include "ProcessManager.h"
#include "Log.h"
#include "DBThreadManager.h"

#define		MAX_LOGIN_DELETE_CHAR_LEVEL		1

UINT CLAskDeleteCharHandler::Execute(CLAskDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			//û��Ȩ�ޣ�ʲôҲ����
			return PACKET_EXE_CONTINUE;
		}

		GUID_t	TryLoginGuid = pPacket->GetCharGuid();

		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//�û������Ƿ�,�û����������ɫ��������
			LCRetDeleteChar Msg;
			Msg.SetResult(ASKDELETECHAR_NOT_OWNER);
			pLoginPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskDeleteCharHandler::Execute()....Fails!,code = ASKDELETECHAR_NOT_OWNER,acc=%s,tryguid=%X",
				pLoginPlayer->GetAccount(),TryLoginGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		UINT uTime = g_pTimeManager->CurrentTime();
		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			LCRetDeleteChar Msg;
			Msg.SetResult(ASKDELETECHAR_OP_TIMES);
			//����DB����Ƶ����Ϣ
			pLoginPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskDeleteCharHandler::Execute()....database busy,acc=%s,tryguid=%X",
				pLoginPlayer->GetAccount(),TryLoginGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		pPacket->SetAccount(pLoginPlayer->GetAccount());
		//����Ҳ�������DB ����
		//��������������ʱ��
		if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		{//����ɹ�������Ϣ���͵�DB����
			pLoginPlayer->m_LastDBOpTime = uTime;	
			return PACKET_EXE_NOTREMOVE;	
		}
		else
		{//DB ѹ���������û����³���
			LCRetDeleteChar Msg;
			Msg.SetResult(ASKDELETECHAR_SERVER_BUSY);
			pLoginPlayer->SendPacket(&Msg);
			return PACKET_EXE_CONTINUE;
		}

	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{

		PlayerID_t	PlayerID	= pPacket->GetPlayerID();
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);
		
		ASKDELETECHAR_RESULT	Result;
		LCRetDeleteChar* pMsg = (LCRetDeleteChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETDELETECHAR);
		if(!pMsg)
		{
			AssertEx(FALSE,"���� LCRetDeleteChar ��Ϣʧ��");
		}

		if(pLoginPlayer->GetDBOperating() == TRUE)
		{
			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskDeleteCharHandler::Execute()....���ݿ������ͻ!,acc = %s",
				pLoginPlayer->GetAccount()) ;
			return PACKET_EXE_NOTREMOVE;
		}

		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLAskDeleteChar DBOperation Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			g_pPacketFactoryManager->RemovePacket(pMsg);	// fix memory leak[4/16/2007]
			return PACKET_EXE_CONTINUE;
		}
		pLoginPlayer->SetDBOperating(TRUE);
		
		LCRetCharList* pRetListMsg = (LCRetCharList*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLIST)	;
		if(!pRetListMsg)
		{
			
			AssertEx(FALSE,"���� LCRetCharList ��Ϣʧ��");
		}
		
		INT CharNumber = pLoginPlayer->GetCharNumber();
		if(CharNumber==0)
		{
			Result = ASKDELETECHARR_EMPTY; //��ɫ����
		}
		else if(CharNumber ==-1)
		{
			Result = ASKDELETECHAR_OP_ERROR;
		}
		else
		{

			ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
			if(!pInterface)
			{// ��Ϊ��ȫ���ʶ���ӵ�[4/16/2007]
				g_pPacketFactoryManager->RemovePacket(pMsg);	// fix memory leak[4/16/2007]
				g_pPacketFactoryManager->RemovePacket(pRetListMsg);		// fix memory leak[4/16/2007]
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_CONTINUE;
			}

			//ɾ����¼
			DBDeleteCharOp DeleteCharOp(pInterface);
			DeleteCharOp.SetAccount(pPacket->GetAccount());
			DeleteCharOp.SetCharGuid(pPacket->GetCharGuid());
			BOOL bRetDelete = 	DeleteCharOp.Delete();
			if(!bRetDelete)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"DeleteCharOp.Delete()....acc=%s,Get Errors: %s ",
					pPacket->GetAccount(),DeleteCharOp.GetErrorMessage());
				INT ErrorCode = DeleteCharOp.GetErrorCode();
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_INTERNAL_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				g_pPacketFactoryManager->RemovePacket(pRetListMsg);		// fix memory leak[4/16/2007]
				pLoginPlayer->SetDBOperating(FALSE);
				g_pLog->FastSaveLog(LOG_FILE_0, "CLAskDeleteCharHandler::Execute()....Fails! code = ASKDELETECHAR_INTERNAL_ERROR \
				 guid = %d,acc = %s",pPacket->GetCharGuid(),pPacket->GetAccount()) ;
				return PACKET_EXE_NOTREMOVE;

			}
			DeleteCharOp.ParseResult(&Result);

			//�����µĽ�ɫ�б�
			DBCharList	CharListObject(pInterface);
			CharListObject.SetAccount(pPacket->GetAccount());
			BOOL bRetLoad = 	CharListObject.Load();
			if(bRetLoad)
			{

				CharListObject.ParseResult(pRetListMsg->GetCharBaseInfo());
				pRetListMsg->SetAccount(pPacket->GetAccount());
				pRetListMsg->SetResult(ASKCHARLIST_SUCCESS);
				pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());

				if(CharListObject.GetCharNumber()<(UINT)CharNumber)
				{
					Result = ASKDELETECHAR_SUCCESS;
				}
				else if(CharListObject.GetCharNumber()==(UINT)CharNumber)
				{
					Result = ASKDELETECHAR_INTERNAL_ERROR;
					for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
					{
						if( pRetListMsg->GetCharBaseInfo(i)->m_GUID == pPacket->GetCharGuid() &&
							pRetListMsg->GetCharBaseInfo(i)->m_Level >= MAX_LOGIN_DELETE_CHAR_LEVEL)
						{
							Result = ASKDELETECHAR_LEVEL10_ERROR;
							break;
						}
					}// end of for
				}
				else
				{
					Result = ASKDELETECHAR_INTERNAL_ERROR;
				}
				pLoginPlayer->SetCharNumber(CharListObject.GetCharNumber());

				for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
				{
					pLoginPlayer->SetCharGUID(pRetListMsg->GetCharBaseInfo(i)->m_GUID,i);
				}
			}
			else //����ʧ�ܣ������ǶϿ�������
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"CharListObject.Load()....acc=%s,Get Errors: %s ",
					pPacket->GetAccount(),
					CharListObject.GetErrorMessage());

				pRetListMsg->SetAccount(pPacket->GetAccount());
				pRetListMsg->SetResult(ASKCHARLIST_OP_FAILS);
				pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());
			}

		}


		pMsg->SetAccount(pPacket->GetAccount());
		pMsg->SetResult(Result);
		g_pProcessManager->SendPacket(pMsg,PlayerID);
		g_pProcessManager->SendPacket(pRetListMsg,PlayerID);
		pLoginPlayer->SetDBOperating(FALSE);
		

		g_pLog->FastSaveLog(LOG_FILE_0, "CLAskDeleteCharHandler::Execute()....OK!	guid=%X,acc=%s",
			pPacket->GetCharGuid(),pPacket->GetAccount()) ;
		return PACKET_EXE_NOTREMOVE;
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}