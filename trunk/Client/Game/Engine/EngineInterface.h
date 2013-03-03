
/** 引擎接口
 */

#ifndef _ENGINE_INTERFACE_H
#define _ENGINE_INTERFACE_H


#include "GIGfxSystem.h"
#include "GIEventSystem.h"



struct EVENT;

class CCamera;
class CDebugPanel;
class CProcessBar;
class CHelpPanel;
class CFakeObjectEntityManager;
class CUIMapLight;
class TerrainGrid;


namespace Fairy 
{ 
	class System;
	class TerrainGrid;
	class PostFilterManager;
	class EnviromentObject;
};

namespace Ogre
{
	class RaySceneQuery;
};



enum ENUM_OBJ_TYPE
{
	NOONE_TYPE	= -1,	
	STATICENTITY_TYPE =1,						//1
	PARTICLE_TYPE = 1<<1,						//2
	TERRAINLIQUIDOBJ_TYPE= 1<<2,				//4
	EFFECTOBJ_TYPE= 1<<3 ,						//8
	BULLETOBJ_TYPE= 1<<4 ,						//16
	LOGICMODEL_TYPE= 1<<5,						//32 100000
	ALL_TYPE = (1<<6) -1,
};

class CEngineInterface : public tGfxSystem
{
public:

	CEngineInterface(VOID);
	virtual ~CEngineInterface(VOID);

	static CEngineInterface* GetMe(VOID)					{ return s_pMe; }


	/** 初始化渲染层 
	*/
	virtual VOID Initial(VOID*);

	/** 销毁渲染层 
	*/
	virtual VOID Release(VOID);

	/** 逻辑轮循函数 
	*/
	virtual VOID Tick(VOID);


	/** 渲染场景 */
	virtual VOID RenderFrame(VOID);
	/** 渲染Loading画面 */
	virtual VOID RenderLodingFrame(LPCTSTR szLoding);

	/** 响应WM_PAINT消息 */
	virtual VOID OnPaint(VOID);
	/** 窗口大小发生改变 */
	virtual VOID OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam);
	/** 取得渲染窗口 */
	virtual HWND GetWindow(VOID) const;
	/** 将当前屏幕截图到文件中 */
	virtual BOOL PrintScreen(LPTSTR szReturnBuf, INT nBufSize);

	/** 变量截获函数，用于控制各种渲染效果的开关 */
	static VOID	WINAPI _OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);


	/** 得到Fairy渲染系统 */
	Fairy::System* GetFairySystem(VOID)						{ return m_pFairySystem; }
	
	/** 游戏坐标与渲染坐标之间的放缩比例 */
	fVector3 GetSacle(VOID) const;

	// 重新设置旧的显示设置， 全屏显示等
	// 登录界面之后。 进入游戏中， 调用这个函数。
	// void ResetOldShowSetting();


	/** 检测是否要透明
	 *	在游戏中遮挡人物的建筑等对象会被透明
	 */
	virtual void CheckObjectTransparent();

public:

/** 坐标转换相关
 */

	/** 将一种坐标转化为另一种坐标,转化成成功返回TRUE
	 *	当源坐标非法的时候，返回FALSE
	 */
	virtual BOOL Axis_Trans(AXIS_TYPE typeSource, const fVector3& fvSource, AXIS_TYPE typeTar, fVector3& fvTarget);

	/** 检查坐标是否合法 */
	virtual BOOL Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis);
	
public:

/** 摄像机相关函数 
 */

	/** 
	 *	设置当前摄像机 
	 */
	virtual VOID Camera_SetCurrent(CAMERA_TYPE theType);

	/**
	 *	设置因UI变化后导致的摄象机观察点的变动
	 */
	virtual VOID Camera_SetOffset( OFFSET_TAG eOT, FLOAT fParallelOffset, FLOAT fVerticalOffset = 0.0f );

	/** 
	 *	移动相机所看的位置到某一点,一般是玩家所在的位置(世界三维坐标系) 
	 */
	virtual VOID Camera_SetLookAt( const fVector3& fvPos );
	
	/** 
	 *	缩放相机视口 [0.0f, 1.0f]  0-最近距离 1-最远距离
	 */
	virtual VOID Camera_SetZoom(FLOAT fZoom);
	virtual VOID Camera_SetZoom(BOOL bUp,INT multiple);
	virtual FLOAT Camera_GetZoom(VOID) const;

	/** 设置相机方向 [0, 2л)， 以z轴指向的方向为0度，逆时针为正方向 
	 */
	virtual VOID Camera_Direction(FLOAT fDir);
	virtual VOID Camera_AddDirection(FLOAT fMultiple);
	virtual FLOAT Camera_GetDirection(VOID) const;

	/** 取得屏幕射线 */
	virtual VOID Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay);

	/** 震荡一次 */
	virtual VOID Camera_VibrateOneTime();
	/** 
	 *	VibRange	y方向的上下偏移量
	 *	StepLen		震荡周期个数
	 *	Times		时间
	*/
	virtual VOID Camera_VibrateOneTime(FLOAT fAmplitude, FLOAT fDuration, UINT fFrequency);


/** 人物选择, 创建, 界面用到的摄像机接口.
 */

	/** 设置眼睛的位置 */
	virtual VOID Camera_SetEyePos(const fVector3& fvPos);
	/** 设置观察点的位置 */
	virtual VOID Camera_SetLookAtPos(const fVector3& fvPos);

public:

/** 调试信息
 */

	/** 加入滚动Deubg信息 */
	virtual VOID PushDebugString(LPCTSTR szMsg, ...);
	/** 设置Debug状态信息 */
	virtual VOID SetDebugString(LPCTSTR szName, LPCTSTR szValue);
	/** 设置选中物体信息 */
	virtual	VOID SetHitObjText(LPCTSTR szHitObjText);


	static const CHAR CEngineInterface::sz32BitTexturesVar[];				// 颜色质量，比特数(16/32位)
	static const CHAR CEngineInterface::szDefaultTextureFilteringVar[];		// 采样(高/中/低)
	static const CHAR CEngineInterface::szLightmapQualityVar[];				// 地形阴影(高/低/无)
	static const CHAR CEngineInterface::szAnimationQualityVar[];			// 静态物体动画(开/关)
	static const CHAR CEngineInterface::szPostFilterEnabledVar[];			// 全屏泛光效果(开/关)
	static const CHAR CEngineInterface::szFSAAVar[];						// 反混角(1/2/4)
	static const CHAR CEngineInterface::szShadowTechniqueVar[];				// 人物阴影(开/关)
	static const CHAR CEngineInterface::szvsyncVar[];						// 垂直同步(开/关)

public:

/** 场景相关 
 */

	/** 预加载场景 */
	virtual VOID			Scene_PreLoad(const STRING& theScenePreload);
	/** 加载场景 */
	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine);
	/**卸载场景*/
	virtual	VOID			Scene_UnLoad(void);
	/** 显示地形网格 */
//	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize);
	/** 显示设置全局渲染效果 */
	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName, BOOL state = TRUE);
	/** 进入/退出死亡全局渲染效果 */
	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter);
	/** 设置Lightmap */
	virtual VOID			Scene_SetLightmapQuality(INT nQuality);
	/** 设置环境特效(位置) */
	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos);
	/** 设置材质32位/16位转化 */
	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE);
	/** 设置采样（高是各向异性3、中是三线性2、低是双线性1(远景清晰度)） */
	virtual VOID			Scene_SetTextureFiltering(INT nOptions);
	/** 设置静态物体动画 */
	virtual VOID			Scene_SetAnimationQuality(INT nOptions);
	/** 设置全屏泛光效果 */
	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting);
	/** 设置人物实时阴影 */
	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting);
	/** 显示地形网格切换*/
	virtual VOID Scene_ShowGridSwitch(INT nZoneSize);


	//-----------------------------------------------------------------------
	/*  对类型对象设置显示和不显示 游戏中类型有以下几种：
	LightObject::getFactory());
	StaticEntityObject::getFactory());
	ActorObject::getFactory());
	ModelObject::getFactory());
	ParticleSystemObject::getFactory()
	SkyBoxObject::getFactory()
	SkyDomeObject::getFactory()
	SkyPlaneObject::getFactory()
	EnviromentObject::getFactory()
	TerrainLiquidObject::getFactory()
	EffectObject::getFactory()
	BulletObject::getFactory()
	LogicModelObject::getFactory())
	RegionObject::getFactory()

	NB:  SetShowObjectByType(StaticEntityObject::getFactory()->getType(),OFF);--关闭建筑物的显示
	*/



	virtual void SetShowObjectByType( LPCTSTR szName);


	/** 
		在某点增加一个粒子 
	 */
	virtual VOID Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos);
	
	//
	virtual VOID Debug_SetFakeCamera(int nType, float fValue);
	
	/** 
		显示/隐藏DebugPanel 
	 */
	virtual VOID Debug_ToggleShowDebugPanel(VOID);

	virtual VOID Debug_ToggleShowBoundingBox(VOID);
	
public:

/**================ 创建可渲染物体相关
 */

	/** 
		创建一个可渲染实体  ETYPE_* 
	 */
	virtual tEntityNode* New_EntityObject(tEntityNode::EntityType type);

	/** 
		删除一个渲染体 
	 */
	VOID Destroy_EntityObject(tEntityNode* pNode);
	
	/** 
	 *	寻找和鼠标接触的实体
	 *	nX, nY - 鼠标位置
	 */
	virtual tEntityNode* Find_HitFairyObject(INT nX, INT nY);

public:

/**================ UI显示模型相关 
 */

	/** 
		创建一个Fake渲染对象 
	 */
	virtual VOID FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, tEntityNode* pParentNode, LPCTSTR szCameraName, 
		int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	
	/** 
		销毁一个Fake渲染对象
	 */
	virtual VOID FakeObj_Destroy(LPCTSTR szObjName);
	
	/** 
		隐藏/显示Fake渲染对象 
	 */
	virtual VOID FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);

	virtual bool FakeObj_SetAttach(tEntityNode* pNode, tEntityNode* pAttachNode);


protected:

	/// 引用 Fairy渲染系统
	Fairy::System*				m_pFairySystem;

	/// 将ogre摄像机作进一步包装,使其更好控制
	CCamera*					m_pCamera_Current;
	/// 人物选择流程摄像机
	CCamera*					m_pCamera_CharView;
	/// 第三人称人物视角摄像机，
	CCamera*					m_pCamera_Scene;
	/// 摄象机偏移量
//	FLOAT						m_fParallelOffset;
//	FLOAT						m_fSaveParallelOffset;
//	FLOAT						m_fOldParallelOffset;
	FLOAT						m_fVerticalOffset;

	/// 客户区宽度
	FLOAT						m_fFullClientWidth;
	BOOL						m_bFullWidth;
	FLOAT						m_fClientWidth;
	/// 保存客户区单个尺寸对应的游戏单位（M）
	FLOAT						m_fUnitGameToScene;

	OFFSET_TAG					m_eOffsetTag;
	BOOL						m_bLeftOffset;
	BOOL						m_bRightOffset;

	/// 鼠标射线和场景交集
	Ogre::RaySceneQuery*		m_pRaySceneQuery;

	/// PostFilter系统(全局渲染器)
	Fairy::PostFilterManager*	m_pPostFilterManager;

	/// 环境特效
	Fairy::EnviromentObject*	m_pEnviromentObject;

	//地形网格
	Fairy::TerrainGrid* m_pTerrainGrid;

	/// 死亡特效
	BOOL						m_bDeathMode;
	INT							m_nDeachModePercent; //[0,100]

	/// 调试板
	CDebugPanel*				m_pDebugPanel;

	/// FakeManager
	CFakeObjectEntityManager*	m_pFakeObjectManager;

	CUIMapLight* m_pMapLight;
	
protected:
	
	static CEngineInterface*	s_pMe;


	KL_DECLARE_DYNAMIC(CEngineInterface);

};	// class



class StringConvert
{
public:

	//utf<->mbcs
	static VOID		mbcs_to_utf8(const std::string& strIn, std::string& strOut);
	static VOID		utf8_to_mbcs(const std::string& strIn, std::string& strOut);

};



#endif	// _ENGINE_INTERFACE_H