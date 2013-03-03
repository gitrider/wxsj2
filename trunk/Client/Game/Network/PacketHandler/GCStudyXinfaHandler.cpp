
#include "stdafx.h"
#include "GCStudyXinfa.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\Event\GMEventSystem.h"



uint GCStudyXinfaHandler::Execute( GCStudyXinfa* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// $$
		//GCStudyXinfa::_STUDERESULT_INFO studyResultInfo;
		//studyResultInfo = pPacket->GetStudeResultInfo();

		////ˢ���ķ�������Ŀͻ������ݣ��ķ��ȼ�����ǰ��Ǯ����ǰ����ֵ��

		//CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		//pCharData->Set_XinFa( (INT)studyResultInfo.m_idXinfa, (INT)studyResultInfo.m_StudedLevel );

		//CHAR szText[128];
		//sprintf(szText, "%d;0", (INT)(BASE_ACTION_XINFA_LEVEL_UP));
		//CObjectManager::GetMe()->GetMySelf()->SetChatMoodAction(STRING(szText));

		////���͸��½�����¼�
		//CEventSystem::GetMe()->PushEvent(GE_SKILLSTUDY_SUCCEED);
		//CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);

		////pCharData->Set_Money( studyResultInfo.m_uSpareMoney );
		////pCharData->Set_Exp( studyResultInfo.m_uSpareExperience );

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
