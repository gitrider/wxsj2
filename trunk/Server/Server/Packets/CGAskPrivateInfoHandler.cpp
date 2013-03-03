

#include "stdafx.h"
#include "CGAskPrivateInfo.h"
#include "Log.h"
#include "Player.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPrivateInfo.h"

uint CGAskPrivateInfoHandler::Execute( CGAskPrivateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t SelfID = pHuman->GetID();
	if( SelfID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	BOOL bSelf = FALSE;
	if( SelfID == pPacket->GetPlayerID() )
	{
		bSelf = TRUE;
	}

	if(bSelf)
	{
		//�����Լ�����Ϣ
		const _PRIVATE_INFO_DB_LOAD *SelfInfo;
		SelfInfo = pHuman->GetDB()->GetPrivateInfoDB();

		GCPrivateInfo msg;
		msg.SetSelf(1);
		msg.SetAge(pHuman->GetDB()->GetPrivateInfoDB()->m_Age);
		msg.SetBloodType(pHuman->GetDB()->GetPrivateInfoDB()->m_BloodType);
		msg.SetCharGUID((UINT)pHuman->GetGUID());
		msg.SetCityLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_City) );
		msg.SetCity((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_City);
		msg.SetConsella(pHuman->GetDB()->GetPrivateInfoDB()->m_Consella);
		msg.SetEmailInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo) );
		msg.SetEmailInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo);
		msg.SetInfoType(pHuman->GetDB()->GetPrivateInfoDB()->m_InfoType);
		msg.SetJobInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo) );
		msg.SetJobInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo);
		msg.SetLuckWordLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		msg.SetLuckWord((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		msg.SetSchoolInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo) );
		msg.SetSchoolInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo);
		msg.SetProvince(pHuman->GetDB()->GetPrivateInfoDB()->m_Province);
		msg.SetSex(pHuman->GetDB()->GetPrivateInfoDB()->m_Sex);
		msg.SetYearAnimal(pHuman->GetDB()->GetPrivateInfoDB()->m_YearAnimal);

		pGamePlayer->SendPacket(&msg);
	}
	else
	{
		//���ͱ��˵���Ϣ
		Obj_Human *pTarget = (Obj_Human*)(pScene->GetObjManager()->GetObj(pPacket->GetPlayerID()));
		if( pTarget==NULL )
		{
			Assert( pTarget ) ;
			return PACKET_EXE_ERROR ;
		}
		
		GCPrivateInfo msg;
		msg.SetSelf(0);

		//��Ҫ����  GetPrivateInfoDB()->m_InfoType ��������ȷ���Ƿ���ʲô���ӵ�����
		if( 2 == pTarget->GetDB()->GetPrivateInfoDB()->m_InfoType)  //��ȫ����
		{
			msg.SetLuckWordLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
			msg.SetLuckWord((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE ;
		}
		if( 1 == pTarget->GetDB()->GetPrivateInfoDB()->m_InfoType )  //���ѹ���
		{
			GUID_t SelfGuid = pHuman->GetGUID();

			//Ӧ��ʹ�öԷ���Ϊ��������ж�
			if( ! pTarget->GetHumanRelation()->IsFriend(SelfGuid) )
			{
				msg.SetLuckWordLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
				msg.SetLuckWord((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord);
				pGamePlayer->SendPacket(&msg);
				return PACKET_EXE_CONTINUE ;
			}
		}
		//��ȫ����(�����ѹ���)
		msg.SetAge(pTarget->GetDB()->GetPrivateInfoDB()->m_Age);
		msg.SetBloodType(pTarget->GetDB()->GetPrivateInfoDB()->m_BloodType);
		msg.SetCharGUID((UINT)pTarget->GetGUID());
		msg.SetCityLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_City) );
		msg.SetCity((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_City);
		msg.SetConsella(pTarget->GetDB()->GetPrivateInfoDB()->m_Consella);
		msg.SetEmailInfoLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_EmailInfo) );
		msg.SetEmailInfo((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_EmailInfo);
		msg.SetInfoType(pHuman->GetDB()->GetPrivateInfoDB()->m_InfoType);
		msg.SetJobInfoLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_JobInfo) );
		msg.SetJobInfo((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_JobInfo);
		msg.SetLuckWordLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		msg.SetLuckWord((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		msg.SetSchoolInfoLength( (BYTE)strlen(pTarget->GetDB()->GetPrivateInfoDB()->m_SchoolInfo) );
		msg.SetSchoolInfo((CHAR *)&pTarget->GetDB()->GetPrivateInfoDB()->m_SchoolInfo);
		msg.SetProvince(pTarget->GetDB()->GetPrivateInfoDB()->m_Province);
		msg.SetSex(pTarget->GetDB()->GetPrivateInfoDB()->m_Sex);
		msg.SetYearAnimal(pTarget->GetDB()->GetPrivateInfoDB()->m_YearAnimal);

		pGamePlayer->SendPacket(&msg);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskPrivateInfoHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
