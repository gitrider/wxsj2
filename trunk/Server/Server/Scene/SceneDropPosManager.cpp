/*
掉落位置管理器，每个场景挂一个。
负责读取掉落点是否有掉落物品。
*/

#include "stdafx.h"
#include "SceneDropPosManager.h"
#include "Scene.h"
#include "Map.h"
#include "Log.h"

#define MAX_SCENE_DROPPOS_SIZE	(512)

SceneDropPosManager::SceneDropPosManager()
{
	__ENTER_FUNCTION
	m_SceneDropPos		=	NULL;
	m_nDropZNum	=	0;
	m_nDropXNum	=	0;
	__LEAVE_FUNCTION
}

SceneDropPosManager::~SceneDropPosManager()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

BOOL		SceneDropPosManager::Init(UINT uMapWidth, UINT uMapHeight)
{
	__ENTER_FUNCTION

		if( uMapWidth>MAX_SCENE_DROPPOS_SIZE || uMapHeight>MAX_SCENE_DROPPOS_SIZE )
			return FALSE;

	//都等于0代表所有清空
	if(uMapWidth == 0 || uMapHeight == 0)
	{
		Log::SaveLog( SERVER_LOGFILE, "SceneDropPosManager::Init error uMapWidth = %d, uMapHeight = %d!\n",
			uMapWidth, 
			uMapHeight 
			);
		return TRUE;
	}

	while ((m_nDropZNum+1)*g_Config.m_ConfigInfo.m_DropPosDistance < uMapHeight)
	{
		++m_nDropZNum;
	}
	while ((m_nDropXNum+1)*g_Config.m_ConfigInfo.m_DropPosDistance < uMapWidth)
	{
		++m_nDropXNum;
	}

	//非零，new内存
	m_SceneDropPos = new BYTE[m_nDropZNum*m_nDropXNum];
	if( m_SceneDropPos == NULL )
	{
		Assert(0);
		return FALSE;
	}
	
	memset(m_SceneDropPos, 0, m_nDropZNum*m_nDropXNum*sizeof(BYTE));
	
	WORLD_POS pos;
	for (UINT i=0; i<m_nDropZNum; ++i)
	{
		for (UINT j=0; j<m_nDropXNum; ++j)
		{
			pos.m_fX =  (j+1)*g_Config.m_ConfigInfo.m_DropPosDistance;
			pos.m_fZ =  (i+1)*g_Config.m_ConfigInfo.m_DropPosDistance;
			if (!m_pScene->GetMap()->IsCanGo(pos))
			{
				m_SceneDropPos[j+i*m_nDropXNum] = -1;
			}
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;

}

VOID		SceneDropPosManager::CleanUp()
{
	__ENTER_FUNCTION

		if(m_SceneDropPos == NULL || m_nDropZNum == 0 || m_nDropXNum == 0)
			return;

	SAFE_DELETE_ARRAY(m_SceneDropPos);
	m_nDropZNum = 0;
	m_nDropXNum  = 0;

	__LEAVE_FUNCTION
}


BOOL		SceneDropPosManager::CanDrop(float pos_fx, float pos_fz)
{
	__ENTER_FUNCTION

	if(m_SceneDropPos == NULL || m_nDropZNum == 0 || m_nDropXNum == 0)
		return FALSE;
	if ((pos_fx<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fx>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropXNum))
	{
		return FALSE;
	}
	if ((pos_fz<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fz>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropZNum))
	{
		return FALSE;
	}

	WORLD_POS pos;
	pos.m_fX = pos_fx;
	pos.m_fZ = pos_fz;

	UINT pos_x = (UINT)(pos_fx/g_Config.m_ConfigInfo.m_DropPosDistance);
	UINT pos_z = (UINT)(pos_fz/g_Config.m_ConfigInfo.m_DropPosDistance);

	if(pos_x <= m_nDropXNum && pos_z <= m_nDropZNum)
	{
		BYTE nInvalidPos = -1;
		if (m_SceneDropPos[(pos_x-1) + m_nDropXNum*(pos_z-1)] == nInvalidPos)
		{
			return FALSE;
		}
		BOOL bAlive = m_SceneDropPos[(pos_x-1) + m_nDropXNum*(pos_z-1)];
		return !bAlive;
	}
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

VOID		SceneDropPosManager::SetDropPos(float pos_fx, float pos_fz)
{
	__ENTER_FUNCTION
	
	if(m_SceneDropPos == NULL || m_nDropZNum == 0 || m_nDropXNum == 0)
		return;
	if ((pos_fx<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fx>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropXNum))
	{
		Assert(0);
	}
	if ((pos_fz<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fz>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropZNum))
	{
		Assert(0);
	}

	UINT pos_x = (UINT)(pos_fx/g_Config.m_ConfigInfo.m_DropPosDistance);
	UINT pos_z = (UINT)(pos_fz/g_Config.m_ConfigInfo.m_DropPosDistance);

	if(pos_x <= m_nDropXNum && pos_z <= m_nDropZNum)
	{
		m_SceneDropPos[(pos_x-1) + m_nDropXNum*(pos_z-1)] = 1;
	}
	else
	{
		Assert(0);
	}
	__LEAVE_FUNCTION

}

VOID		SceneDropPosManager::ClearDropPos(float pos_fx, float pos_fz)
{
	__ENTER_FUNCTION
	if(m_SceneDropPos == NULL || m_nDropZNum == 0 || m_nDropXNum == 0)
		return;
	if ((pos_fx<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fx>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropXNum))
	{
		Assert(0);
	}
	if ((pos_fz<g_Config.m_ConfigInfo.m_DropPosDistance)
		||(pos_fz>g_Config.m_ConfigInfo.m_DropPosDistance*m_nDropZNum))
	{
		Assert(0);
	}
	UINT pos_x = (UINT)(pos_fx/g_Config.m_ConfigInfo.m_DropPosDistance);
	UINT pos_z = (UINT)(pos_fz/g_Config.m_ConfigInfo.m_DropPosDistance);

	if(pos_x <= m_nDropXNum && pos_z <= m_nDropZNum)
	{
		m_SceneDropPos[(pos_x-1) + m_nDropXNum*(pos_z-1)] = 0;
	}
	else
	{
		Assert(0);
	}
	__LEAVE_FUNCTION

}