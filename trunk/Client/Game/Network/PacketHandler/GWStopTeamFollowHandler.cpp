/********************************************************************************
 *	�ļ�����	GWStopTeamFollowHandler.cpp
 *	ȫ·����	d:\Prj\Client\Game\Network\PacketHandler\GWStopTeamFollowHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	23:38
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "StdAfx.h"
#include "GWStopTeamFollow.h"

uint GWStopTeamFollowHandler::Execute( GWStopTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
