/********************************************************************************
 *	文件名：	GWTeamMemberEnterScene.cpp
 *	全路径：	d:\Prj\Common\Packets\GWTeamMemberEnterScene.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 3 日	17:48
 *
 *	功能说明：	
 *	修改记录：
 *********************************************************************************/

#include "stdafx.h"
#include "GWTeamMemberEnterScene.h"

BOOL GWTeamMemberEnterScene::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerID, sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL GWTeamMemberEnterScene::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT GWTeamMemberEnterScene::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTeamMemberEnterSceneHandler::Execute(this, pPlayer);
	
__LEAVE_FUNCTION

	return FALSE;
}
