/********************************************************************************
 *	�ļ�����	GWLevelUpHandler.cpp
 *	ȫ·����	d:\Prj\Client\Game\Network\PacketHandler\GWLevelUpHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 9 ��	15:09
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "StdAfx.h"
#include "GWLevelUp.h"

using namespace Packets;

uint GWLevelUpHandler::Execute(GWLevelUp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
