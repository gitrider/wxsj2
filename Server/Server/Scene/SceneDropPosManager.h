/*
掉落位置管理器，每个场景挂一个。
负责读取掉落点是否有掉落物品。
*/

#ifndef _SCENEDROPPOS_MANAGER_H_
#define _SCENEDROPPOS_MANAGER_H_
#include "Type.h"

/*
SceneDropPosManager,负责数据的读取，访问。
*/
class Scene;
class SceneDropPosManager
{
public:
	SceneDropPosManager();
	~SceneDropPosManager();
public:
	//-------------------------------------------------------------------------------------------------
	//Init里分配内存，对应在CleanUp中释放内存
	BOOL		Init(UINT uMapWidth = 0, UINT uMapHeight = 0);
	VOID		CleanUp();

	//-------------------------------------------------------------------------------------------------
public:
	VOID				SetScene( Scene* pScene ) { Assert( pScene ); m_pScene = pScene; }
	BOOL				CanDrop(float pos_x, float pos_y);
	VOID				SetDropPos(float pos_x, float pos_y);
	VOID				ClearDropPos(float pos_x, float pos_y);
protected:
	BYTE				*m_SceneDropPos;
	UINT				m_nDropZNum;
	UINT				m_nDropXNum;

	Scene*				m_pScene;
};

#endif