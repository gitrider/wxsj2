
#include "StdAfx.h"
#include "GITimeSystem.h"
#include "GIException.h"
#include "Global.h"
#include "Dbc\GMDataBase.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Surface\Obj_Special.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GCTargetListAndHitFlags.h"
#include "GIUtil.h"
#include "..\..\Object\Logic\Surface\Obj_Bullet.h"
#include "SkillDataMgr.h"
#include "Object\Manager\BulletEntityManager.h"
#include "object\ObjectCommand.h"


using namespace Packets;


uint GCTargetListAndHitFlagsHandler :: Execute( GCTargetListAndHitFlags* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		const CHAR *pszSenderLocator = NULL;
		BOOL	bHitCharacter	= FALSE;
		INT		nBulletID		= INVALID_ID;
		FLOAT	fAngle			= 0.0f;
		INT		nBulletNum		= 1;
		FLOAT	nDamageAngle	= 0;
		FLOAT	fDamageRange	= 0.0f;		

		switch (pPacket->GetDataType())
		{
			// 由陷阱、地雷触发
		case GCTargetListAndHitFlags::SPECIAL_OBJ_ACTIVATE:
			{
				DBC_DEFINEHANDLE(s_pSpecialDataDBC, DBC_SPECIAL_OBJ_DATA);
				const _DBC_SPECIAL_OBJ_DATA *pSpecialObjData = (const _DBC_SPECIAL_OBJ_DATA*)s_pSpecialDataDBC->Search_Index_EQU(pPacket->GetSkillOrSpecialObjDataID());
				if(pSpecialObjData != NULL)
				{
					nBulletID			= pSpecialObjData->m_nBulletID;
					bHitCharacter		= TRUE;
					pszSenderLocator	= NULL;
				}
			}
			break;
			// 由buff触发
		case GCTargetListAndHitFlags::BUFF_ACTIVETE:
			{
				nBulletID			= 1;
				bHitCharacter		= TRUE;
				pszSenderLocator	= GetCharaLocatorName(LOCATOR_CHAR_ATTACK);	// "人物身体受击点";
			}
			break;
			// 由技能触发
		case GCTargetListAndHitFlags::UNIT_USE_SKILL:
			{
				SCommand_Object cmdTemp;

				ObjID_t	dwTargetIDList[MAGICSEND_MAX_TARGET_NUM] = {0};
				for (INT i=0; i<pPacket->GetTargetNum(); ++i)
				{
					dwTargetIDList[i] = pPacket->GetTargetObjIDByIndex(i);
				}

				cmdTemp.m_wID			= OC_MAGIC_SEND;
				cmdTemp.m_auParam[0]	= 0;
				cmdTemp.m_anParam[1]	= pPacket->GetLogicCount();
				cmdTemp.m_anParam[2]	= pPacket->GetSkillOrSpecialObjDataID();
				cmdTemp.m_anParam[3]	= pPacket->GetTargetNum();
				cmdTemp.m_afParam[4]	= pPacket->GetTargetPos().m_fX;
				cmdTemp.m_afParam[5]	= pPacket->GetTargetPos().m_fZ;
				cmdTemp.m_afParam[6]	= pPacket->GetDir();
				cmdTemp.m_apParam[7]	= (VOID*)&dwTargetIDList;

				pObj->PushCommand(&cmdTemp );

				pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

				AxTrace(0,2,"Magic Send Be Come");
			}
			break;
		default:
			break;
		}

		// 存在攻击目标
		if(bHitCharacter)
		{
			if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			{
				// 如果只有一个敌人， 朝向他
				INT nTargetNum =(INT)(pPacket->GetTargetNum());
				if(1 == nTargetNum)
				{
					CObjectManager::GetMe()->FaceToTarget( (INT)pPacket->GetObjID(), pPacket->GetTargetObjIDByIndex(0) );
				}
			}
		}


		

		pObj->PushDebugString("GCTargetListAndHitFlags");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

