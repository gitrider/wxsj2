
/** GCNotifyMailHandler.cpp
 *	����˵����	�����¼����ʼ�֪ͨ��ֻ֪ͨʣ��δ�����ʼ�������
 */

#include "StdAfx.h"
#include "GCNotifyMail.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_Struct_Mail.h"
#include "..\..\Procedure\GameProcedure.h"



uint GCNotifyMailHandler::Execute( GCNotifyMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CMailPool* pMailPool = CDataPool::GetMe()->GetMailPool();

		// ����ʣ��δ���ʼ�����
		pMailPool->SetMailLeftCount( (INT)pPacket->GetMailCount() );

		// �� UI �õ���Ϣ��ͼ����˸�ȵȣ�
		pMailPool->InformUI();
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
