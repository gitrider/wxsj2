/********************************************************************************
 *	�ļ�����	GWEnterTeamFollowHandler.cpp
 *	ȫ·����	d:\Prj\Client\Game\Network\PacketHandler\GWEnterTeamFollowHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	15:39
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "StdAfx.h"
#include "GWEnterTeamFollow.h"

uint GWEnterTeamFollowHandler::Execute( GWEnterTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
