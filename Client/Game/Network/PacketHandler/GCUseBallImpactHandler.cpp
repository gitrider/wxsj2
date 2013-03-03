#include "StdAfx.h"
#include "Global.h"
#include "..\..\Procedure\GameProcedure.h"
#include "GCUseBallImpact.h"
#include "SkillDataMgr.h"
#include "..\..\Object\Manager\objectManager.h"
#include "..\..\Object\Logic\Surface\Obj_Bullet.h"


uint GCUseBallImpactHandler::Execute(GCUseBallImpact* pPacket, Player* pPlayer)
{
__ENTER_FUNCTION

	if (CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//static int nBulletCounter = 0;
  // 
		////**********
		////
		///*char a[1024];
		//sprintf(a, "GCUseBallImpactHandler::Execute --- m_nSkillID %d    m_uBallNum %d    m_nCharacterID %d\n", 
		//	pPacket->GetSkillID(), pPacket->GetBallNum(), pPacket->GetCharacterID() );
		//OutputDebugString(a);*/
		////
		////**********

		//std::list<CObject_Bullet*> vBulletList;

		////查找角色身上现有多少球
		//const std::map<string, CObject*>& OjectMap = CObjectManager::GetMe()->GetObjectStrMap();
		//std::map<string, CObject*>::const_iterator itLie = OjectMap.begin();		
		//
		//for (; itLie!=OjectMap.end(); ++itLie)
		//{			
		//	//是不是子弹类
		//	if (g_theKernel.IsKindOf(itLie->second->GetClass(), GETCLASS(CObject_Bullet)))
		//	{
		//		CObject_Bullet* pBullet = (CObject_Bullet*)itLie->second;
		//		if (pBullet && pBullet->GetTargetID() == pPacket->GetCharacterID() && pBullet->GetServerID() == -1)
		//		{
		//			vBulletList.push_back(pBullet);
		//		}
		//	}
		//}
		//

		////球数不够添加新球
		//bool bResetAngle = false;
		//if (vBulletList.size() < pPacket->GetBallNum())
		//{
		//	const SSkillData* pClientSkill = GetSkillDataMgr()->GetSkillData( pPacket->GetSkillID() );
		//	if (pClientSkill)
		//	{
		//		SObject_BulletInit initBullet;

		//		initBullet.m_nBulletID		=	pClientSkill->m_nBulletID;
		//		initBullet.m_bSingleTarget	=	true;
		//		initBullet.m_nContrailType	=	BULLET_CONTRAIL_TYPE_ROTATE;
		//		initBullet.m_idSend			=	pPacket->GetCharacterID();
		//		initBullet.m_idTarget		=	pPacket->GetCharacterID();

		//		int nNum = pPacket->GetBallNum() - vBulletList.size();

		//		for (int i=0; i<nNum; ++i)
		//		{
		//			++nBulletCounter;
		//			char name[256];
		//			sprintf(name, "CObject_Bullet_%d", nBulletCounter);

		//			CObject_Bullet *pBullet = (CObject_Bullet*)CObjectManager::GetMe()->NewSurfaceObj( "CObject_Bullet", name );
		//			pBullet->Initial( &initBullet );

		//			vBulletList.push_back(pBullet);					
		//		}

		//		bResetAngle = true;
		//	}
		//}
		////身上的球多的时候清除掉多余的
		//else if(vBulletList.size() > pPacket->GetBallNum())
		//{
		//	//const SSkillData* pClientSkill = GetSkillDataMgr()->GetSkillData( pPacket->GetSkillID() );
		//	//if (pClientSkill)
		//	//{
		//	//	for (; itLie!=OjectMap.end(); ++itLie)
		//	//	{
		//	//		//是不是子弹类
		//	//		if (g_theKernel.IsKindOf(itLie->second->GetClass(), GETCLASS(CObject_Bullet)))
		//	//		{
		//	//			CObject_Bullet* pBullet = (CObject_Bullet*)itLie->second;
		//	//			if (pBullet && pBullet->GetTargetID() == pPacket->GetCharacterID() && pBullet->GetServerID() == -1)
		//	//			{
		//	//				vBulletList.push_back(pBullet);
		//	//			}
		//	//		}
		//	//	}

		//	//	bResetAngle = true;
		//	//}
		//}
		//

		////从新进行角度排序
		//if (bResetAngle)
		//{
		//	int k = 0;
		//	std::list<CObject_Bullet*>::iterator iter = vBulletList.begin();
		//	for (; iter!=vBulletList.end(); ++iter)
		//	{
		//		float fAngle = 360.0f / vBulletList.size() * k;
		//		(*iter)->SetAngle(fAngle);
		//		(*iter)->CumulateTimeClear();
		//		++k;
		//	}
		//}
		
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
