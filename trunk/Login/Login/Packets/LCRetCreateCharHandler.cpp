#include "stdafx.h" 
#include "LCRetCreateChar.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
//#include "DBCreateCharOp.h"
#include "DBCharCreate.h"
#include "PacketFactoryManager.h"
#include "ProcessManager.h"
#include "LCRetCharList.h"
#include "DBCharList.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "Log.h"
#include "CharConfig.h"
#include "DBThreadManager.h"


UINT	LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		TID CurrentThreadID = MyGetCurrentThreadID();
		if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID)
		{
			LoginPlayer*	pLoginPlayer = (LoginPlayer*)pPlayer ;
			if( pLoginPlayer==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_CONTINUE ;
			}

			//����˺��Ƿ���ȷ
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount()) != 0)
			{
				
				g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCreateCharHandler::Execute() ....OK,acc=%s,packacc not same",
					pLoginPlayer->GetAccount());
				return PACKET_EXE_CONTINUE;
			}

			if(pPacket->GetResult() == ASKCREATECHAR_NEXTDB_CREATE)
			{
				if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCreateCharHandler::Execute() ....Fail,acc=%s not in PS_LOGIN_NORMAL",
						pLoginPlayer->GetAccount());
					return PACKET_EXE_CONTINUE;
				}

				if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
				{//����ɹ�������Ϣ���͵�DB����
					pLoginPlayer->m_LastDBOpTime = g_pTimeManager->CurrentTime();
					return PACKET_EXE_NOTREMOVE;	
				}
				else
				{//DB ѹ���������û����³���
					LCRetCreateChar Msg;
					Msg.SetAccount(pPacket->GetAccount());
					Msg.SetResult(ASKCREATECHAR_SERVER_BUSY);
					pLoginPlayer->SendPacket(&Msg);

					g_pLog->FastSaveLog(LOG_FILE_0, "LCRetCreateCharHandler....Fails!, code = ASKCREATECHAR_SERVER_BUSY,acc=%s",
						pPacket->GetAccount()) ;
					return PACKET_EXE_CONTINUE;
				}
			}
			else
			{
				pLoginPlayer->SendPacket(pPacket);
				g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCreateCharHandler::Execute() ....OK,acc=%s",pLoginPlayer->GetAccount());
			}
		}
		else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
		{
			GUID_t	PlayerCharGUID = 1;

			LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
			Assert(pLoginPlayer);
			PlayerID_t	PlayerID	= pLoginPlayer->PlayerID();

			//����˺��Ƿ���ȷ
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount()) != 0)
			{

				g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCreateCharHandler::Execute() ....OK,acc=%s,packacc not same",
					pLoginPlayer->GetAccount());
				return PACKET_EXE_CONTINUE;
			}
            
			if(pPacket->GetResult() == ASKCREATECHAR_NEXTDB_CREATE)
			{
				pLoginPlayer->SetDBOperating(TRUE);

				PlayerCharGUID = pPacket->GetGUID();
				Assert(PlayerCharGUID!=INVALID_ID);

				ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
				if(!pInterface)
				{// ��Ϊ��ȫ���ʶ���ӵ�[4/16/2007]
					pLoginPlayer->SetDBOperating(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				INT CharNumber = pLoginPlayer->GetCharNumber();

				ASKCREATECHAR_RESULT	Result = ASKCREATECHAR_INTERNAL_ERROR;
				LCRetCreateChar* pMsg = (LCRetCreateChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCREATECHAR);
				if(!pMsg)
				{
					AssertEx(FALSE,"���� LCRetCreateChar ��Ϣʧ��");
				}
				pMsg->SetAccount(pPacket->GetAccount());

				LCRetCharList* pRetListMsg = (LCRetCharList*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLIST)	;
				if(!pRetListMsg)
				{
					AssertEx(FALSE,"���� LCRetCharList ��Ϣʧ��");
				}

				//���û�������
				FULLUSERDATA		UserData;
				DBCharFullData		CharFullDataObject(pInterface);
				CharFullDataObject.SetCharGuid(PlayerCharGUID);

				BOOL bRetLoad =	CharFullDataObject.Load();
				if(!bRetLoad)
				{	
					//��֤����ʧ��
					pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
					g_pProcessManager->SendPacket(pMsg,PlayerID);
					g_pPacketFactoryManager->RemovePacket(pRetListMsg);		// fix memory leak[4/16/2007]
					pLoginPlayer->SetDBOperating(FALSE);
					g_pLog->FastSaveLog(LOG_FILE_0, "LCRetCreateCharHandler::Execute() ...Load()....Fails!,acc = %s,guid=%X",
						pPacket->GetAccount(),PlayerCharGUID) ;
					return PACKET_EXE_CONTINUE;
				}

				CharFullDataObject.ParseResult(&UserData);
				// yangjun[12/5/2006]
				//�ݲ����DBChardbVersion�ļ��
				g_CharConfig.InitCharAttr(&UserData, pPacket->GetCountry(), pPacket->GetMenPai());
				//���ô���ʱ��
				BOOL bRetSave = CharFullDataObject.Save(&UserData); //�����ʼ����Ϣ
				if(!bRetSave)
				{
					//��ʼ����������ʧ��
					pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
					g_pProcessManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0, "LCRetCreateCharHandler::Execute() ...Save()....Fails!,acc = %s,guid = %X",
						pPacket->GetAccount(),PlayerCharGUID) ;
					g_pPacketFactoryManager->RemovePacket(pRetListMsg);		// fix memory leak[4/16/2007]
					pLoginPlayer->SetDBOperating(FALSE);
					return PACKET_EXE_CONTINUE;
				}
				bRetSave	=	CharFullDataObject.ParseResult(&UserData);//����ȫ������
				if(!bRetSave)
				{
					//��ʼ����������ʧ��
					pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
					g_pProcessManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0, "LCRetCreateCharHandler::Execute() ...ParseResult....Fails!,acc=%s,guid=%X",
						pPacket->GetAccount(),PlayerCharGUID) ;
					g_pPacketFactoryManager->RemovePacket(pRetListMsg);		// fix memory leak[4/16/2007]
					pLoginPlayer->SetDBOperating(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				//�����µĽ�ɫ�б�
				DBCharList	CharListObject(pInterface);
				CharListObject.SetAccount(pPacket->GetAccount());
				bRetLoad = 	CharListObject.Load();
				if(bRetLoad)
				{
					CharListObject.ParseResult(pRetListMsg->GetCharBaseInfo());

					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_SUCCESS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());

					if(CharListObject.GetCharNumber()> (UINT)CharNumber)
					{
						Result = ASKCREATECHAR_SUCCESS;
						pLoginPlayer->m_bAlreadyCheckCreateCode = FALSE;
					}
					else
					{
						Result = ASKCREATECHAR_INTERNAL_ERROR;
					}
					pLoginPlayer->SetCharNumber(CharListObject.GetCharNumber());

					for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
					{
						pLoginPlayer->SetCharGUID(pRetListMsg->GetCharBaseInfo(i)->m_GUID,i);
					}
				}
				else //����ʧ�ܣ������ǶϿ�������
				{
					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_OP_FAILS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());
				}
			
				pMsg->SetResult(Result);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				g_pProcessManager->SendPacket(pRetListMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				g_pLog->FastSaveLog(LOG_FILE_0, "LCRetCreateCharHandler::Execute()....OK! \
									Account = %s CharGuid = %X CharName = %s",\
									pPacket->GetAccount(),PlayerCharGUID,UserData.m_Human.m_Name) ;
			}
			return PACKET_EXE_CONTINUE;
		}
		else
		{
			AssertEx(FALSE,"LCRetCreateCharHandler �߳���Դִ�д���!");
		}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}