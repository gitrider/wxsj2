// GCPlayerCallOfHandler.cpp

#include "StdAfx.h"

#include "GCPlayerCallOf.h"
#include "..\..\Event\GMEventSystem.h"

using namespace Packets;

uint GCPlayerCallOfHandler::Execute( GCPlayerCallOf* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//��ʾ�Ի���
	//ȷ����ȡ����ʱ���� CGPlayerCallOfResult
	GUID_t PlayerGuid = pPacket->GetCallerGUID();

	// �������б���ȡ�ҵ������ҵ����� 

	CEventSystem::GetMe()->PushEvent(GE_OPEN_CALLOF_PLAYER);

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
