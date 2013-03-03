/*
����λ�ù�������ÿ��������һ����
�����ȡ������Ƿ��е�����Ʒ��
*/

#ifndef _SCENEDROPPOS_MANAGER_H_
#define _SCENEDROPPOS_MANAGER_H_
#include "Type.h"

/*
SceneDropPosManager,�������ݵĶ�ȡ�����ʡ�
*/
class Scene;
class SceneDropPosManager
{
public:
	SceneDropPosManager();
	~SceneDropPosManager();
public:
	//-------------------------------------------------------------------------------------------------
	//Init������ڴ棬��Ӧ��CleanUp���ͷ��ڴ�
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