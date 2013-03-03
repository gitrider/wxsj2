#ifndef _CHAR_CONFIG_H_
#define _CHAR_CONFIG_H_
#include "Type.h"

struct FULLUSERDATA;


enum	CHAR_TYPE
{
	CT_NORMAL	= 0,
	CT_NUMBER,
};

#define MAX_SCENE_CHAR_TYPE 10

class DBChooseSceneTable
{
	
public:

	DBChooseSceneTable();

	CFG_CHOOSE_SCENE	mSceneTable[MAX_SCENE_CHAR_TYPE];

	CFG_CHOOSE_SCENE*	GetSceneInfo(CHAR_TYPE ctype);

	BOOL				GetPosBySceneID(SceneID_t sid,CHAR_TYPE ctype,WORLD_POS& pos);

	BOOL				isValidSceneID(SceneID_t sid,CHAR_TYPE ctype);
};

extern	DBChooseSceneTable	g_DBSceneTable;

class CharConfig
{
public:

	CharConfig();
	~CharConfig();
	static	BOOL			InitCharAttr(FULLUSERDATA* pData, INT country, INT menpai);			  //在Login 上初始化角色基本信息
	static	BOOL			GetCharChooseScene(DB_CHOOSE_SCENE* pInfo,CHAR_TYPE); //获取角色选择场景列表
public:

	static FULLUSERDATA*	pUserData;
};


extern CharConfig g_CharConfig;


#endif