/********************************************************************************
 *	�ļ�����	GWTeamMemberEnterSceneHandler.cpp
 *	ȫ·����	d:\Prj\Client\Game\Network\PacketHandler\GWTeamMemberEnterSceneHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 3 ��	18:10
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "StdAfx.h"
#include "GWTeamMemberEnterScene.h"

uint GWTeamMemberEnterSceneHandler::Execute( GWTeamMemberEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
