
/** GIWorldSystem.h
 */

#pragma once


#include "GINode.h"
#include "GIGfxSystem.h"
#include "GISound.h"
#include "GIObjectManager.h"



//======================================================================
/// 游戏场景接口
struct _DBC_SCENE_DEFINE;

struct MAP_POS_DEFINE
{
	fVector3			pos;
	DWORD				dwSceneID;		// 所在场景id
	char				name[ 128 ];
	INT					nServerID;
	char				utf8name[128];	//优化名称
	bool				opti;
	
	MAP_POS_DEFINE()
	{
		opti = false;
		memset(utf8name,0,128);
	}
	
	void Set_Name(const char* strName);
	void Set_Pos_Name(const char* strName);
	void Set_Pos_Utf8Name(const char* strName);
};

/// 对外提供的地形高度查询接口(使用渲染系统坐标，考虑地形和行走面)
typedef bool (__stdcall* FUNC_GETTERRAINHEIGHT)(float x, float z, float& y);

class tScene
{
public:

	/** 取得场景定义 */
	virtual const _DBC_SCENE_DEFINE* GetSceneDefine(VOID) const  = 0;
	/** 场景位置是否合法 */
	virtual BOOL IsValidPosition(const fVector2& fvPos) = 0;
	/** 根据逻辑坐标取得地形高度，考虑行走面，如果传入坐标非法，返回FALSE */
	virtual BOOL GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight) = 0;

};

//======================================================================
//游戏世界接口
//

enum MINIMAP_TYPE
{	
	MINIMAP_TYPE_ANIMY		= 0,	// 敌人
	MINIMAP_TYPE_TEAM		= 1,	// 队友

	MINIMAP_TYPE_FAMILY		 = 2,	// 家族
	MINIMAP_TYPE_GUILD	     = 3,	// 帮会
	MINIMAP_TYPE_HOMELAND	 = 4,	// 本国别的玩家	
	MINIMAP_TYPE_NOHOMELAND  = 5,	// 非本国别的玩家		

	MINIMAP_TYPE_EXPOBJ			= 6,	// 生长点
	MINIMAP_TYPE_ACTIVE			= 7,	// 激活方向点
	MINIMAP_TYPE_SCENEPOS 		= 8,	// 场景传送点
	MINIMAP_TYPE_FLASH			= 9,	// 闪光点

	MINIMAP_TYPE_EXPNPC			 = 10,	// 普通npc
	MINIMAP_TYPE_MISCOMNPC		 = 11,	// 任务NPC( 完成 )
	MINIMAP_TYPE_MISNOCOMNPC	 = 12,  // 任务NPC( 未完成 )
	MINIMAP_TYPE_ACCEPTNPC		 = 13,	// 任务NPC( 可接 )	

	MINIMAP_TYPE_NUM,	

	MINIMAP_TYPE_MYSELF,// 玩家自己
	MINIMAP_TYPE_PET,	// 宠物
	
};

class KYLIN_API tWorldSystem : public tNode
{
public:

	enum WORLD_STATION
	{
		WS_NOT_ENTER,				//没有进入任何场景
		WS_ENTER_ING,				//正在进入一个场景(已经发送了CGEnterScene)
		WS_ENTER,					//成功进入一个场景
		WS_ASK_CHANGE_SCANE,		//要求切换到一个新的场景(已经发送了CGAskChangeScene)
	};	

public:

	/**	进入某场景
	 *	\param nSceneID	
	 *		场景ID/玩家城市id
	 *	\param nCityLevel
	 *		玩家城市等级，如果小于0，表示是普通场景!
	 */
	virtual BOOL EnterScene(INT nSceneID, INT nCityLevel, INT nResID)= 0;
	/** 取得当前激活的场景 */
	virtual tScene* GetActiveScene(void) = 0;
	/** 取得当前状态 */
	virtual WORLD_STATION GetStation(void) const = 0;
	/** 对外提供的地形高度查询接口(使用渲染系统坐标，考虑地形和行走面) */
	virtual FUNC_GETTERRAINHEIGHT GetTerrainHeightFunc(void) = 0;
	/** 当前场景的ServerID */
	virtual INT GetActiveSceneID(void) const = 0;


	virtual std::list< MAP_POS_DEFINE >* GetObjectListByClass( INT nType ) = 0;
	virtual void setMinimapSerachRange( int nRange ) = 0;
	virtual void UpdateMinimapData() = 0;
	virtual void MinimapNeedFresh() = 0;


	virtual fVector3 GetMyselfPos() = 0;
	virtual int GetMyselfDir() = 0;
	virtual const CHAR* GetSceneName( INT nSceneID ) = 0;


	KL_DECLARE_DYNAMIC(tWorldSystem);

};
