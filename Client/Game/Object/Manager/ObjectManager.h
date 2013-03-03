
/**	ObjectManager.h
*/

#ifndef _OBJECT_MANAGER_H
#define _OBJECT_MANAGER_H


#include "GIObjectManager.h"
#include "Type.h"





class CObject;
class CObject_Character;
class CObject_PlayerMySelf;
class CObject_PlayerOther;
class CObject_PlayerNPC;
class CObject_Surface;
class CObjectLoadQueue;
class CObjectDestoryQueue;

/**	管理游戏中的对象
 */
class CObjectManager : public tObjectSystem
{
protected:

	/// 被UI关心的逻辑对象
	struct OBJECT_BECARED
	{
		INT		id;					/// 物体ID
		FLOAT	fLastDistance;		/// 和主角之间的距离
	};

public:

	CObjectManager( VOID );
	virtual ~CObjectManager( VOID );

	virtual VOID Initial( VOID* );
	virtual VOID Release( VOID );
	virtual VOID Tick( VOID );


	static CObjectManager* GetMe( VOID )						{ return s_pMe; }
	
	/** 取得玩家自己 */
	CObject_PlayerMySelf* GetMySelf( VOID )						{ return m_pMySelf; }
	/** 获取主目标UI模型 */
	CObject_PlayerOther* GetTarget_Avatar( VOID )				{ return m_pMainTargetAvatar;}	
	
	/** 获得主角的UI模型 */
//	CObject_PlayerOther* GetActorAvatar(INT index)				{ if((index < 0) || (index >= 2)) return NULL; return m_pAvatar[index]; }
	/** 用默认的参数初始化主角的UI模型 */
	virtual VOID InitActorAvatar(VOID);
	virtual VOID DestoryActorAvatar(VOID);


	/** 产生新对象,并加入数据链
	 */
	virtual tObject* NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL);
	virtual CObject_PlayerOther* NewPlayerOther( INT idServer );
	virtual CObject_PlayerNPC* NewPlayerNPC( INT idServer );
	/** 创建表面物体 */
	virtual CObject_Surface* NewSurfaceObj( string strClass, LPCTSTR ObjName );

	/** 销毁物体,并从数据链上拆除 */
	virtual BOOL DestroyObject(tObject* pObject);

	/** 销毁指定名称的物体（子弹） */
	virtual VOID DetchBullObject( string& name );


	/** 根据ID得到某物体 */
	virtual tObject* FindObject( INT id );
	
	/** 获取一个SERVER OBJ对象的指针 */
	virtual tObject* FindServerObject( INT idServer );

	/** 通过名字来获取对象（现在只用于子弹） */
	virtual tObject* FindBullObject( string& objName );

	/**/
	virtual const std::map<string, CObject*>& GetObjectStrMap(){ return m_mBullObject; }

	/** 获取保存的当前Npc列表的id */
	virtual const std::vector<INT>& GetNpcObjectId(VOID);
	
	/** 获得物品 */
	virtual tObject_Item* FindItem(INT id);
	
	/** 销毁所有物体 */
	virtual VOID ClearAllObject( VOID );
	
	/** 设置主目标对象，如果id非法，取消选中物体 */
	virtual VOID SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT);

	//读取Object的名字	20100706 BLL
	virtual LPCTSTR	 GetMySelfOrTargetName(BOOL bTarget = FALSE);
	
	/** 取得当前选中物体 */
	virtual tObject* GetMainTarget(VOID);

	/** 取得当前选中物体 */
	virtual tObject* GetMainTarget(GUID_t& guid);

	/** 将最近的一个敌人设为主目标 */
	void Debug_LockNearestTarget(void);
	
	/** 设置某ojbect得ServerID */
	virtual BOOL SetObjectServerID( INT idObj, INT idServer );
	
	/** 根据屏幕坐标计算选中得物体 */
	virtual tObject* GetMouseOverObject( INT nX, INT nY, fVector3& fvMouseHitPlan );
	
	/** 关心某物体的指定事件 id-物体ID		bCare-关心或者取消关心 */
	virtual VOID CareObject(INT id, BOOL bCare, std::string szSign);
	
	/** 从本地资源表中读取物品名字 */
	virtual LPCTSTR ItemNameByTBIndex(UINT idTable);

	/** 资源加载队列 */
	CObjectLoadQueue* GetLoadQueue(void)				{ return m_pLoadQueue; }

	/** 销毁队列 */
	CObjectDestoryQueue* GetDestoryQueue(void)			{ return m_pDestoryQueue; }
	

	/** 根据人物名字获得对应的objcet */
	virtual CObject_Character* FindCharacterByName(std::string  szName);
	VOID SetTargetAvatarAsTarget( VOID );
	

	/** 响应分辨率改变 */
	void OnScreenSizeChanged(void);
	
	/** 响应主角等级提高,更改所有npc的等级 */
	void OnMySelfLevelUp(void);


	/** 设置当前选择的是队友 */
	void SetCurSelTeam(GUID_t id);
	GUID_t GetCurSelTeam();

	/** 设置队友是否在当前可视范围 */
	void SetPresent(BOOL  bIsPresent);
	
	/** 当前选择的是否是对友 */
	BOOL IsSelTeam();

	/** 选择的队友是否在眼前 */
	BOOL IsSelTeamInPresent();


	/** 不选择mainTarget */
	void UnSelMainTarget(void);

	/** 将某对象转向目标对象 */
	void FaceToTarget(INT sObj, INT tObj);


private:
	
	INT IDFactory( VOID );

protected:
	
	VOID AttachObject( tObject *pObject );
	VOID DetchObject( tObject *pObject );

	KL_DECLARE_DYNAMIC(CObjectManager);

protected:
	
	static CObjectManager*			s_pMe;

	/// 自已本身
	CObject_PlayerMySelf*			m_pMySelf;
	/// 物体根节点
	CObject*						m_pObjectRoot;
	/// 逻辑物体节点
	CObject*						m_pLogicalObject;
	/// 其他玩家节点
	CObject*						m_pPlayerOther;
	/// NPC节点
	CObject*						m_pNPC;
	/// 地表物体节点
	CObject*						m_pSurface;
	/// 非逻辑物体节点
	CObject*						m_pAlogicalObject;

	typedef std::map<INT, CObject*> CObjectMap;
	/// 根据ID的物体HASH表
	CObjectMap						m_hmObject;
	/// 根据ServerID的物体HASH表
	CObjectMap						m_hmServerObject;
	
	typedef std::map<string, CObject*> CObjectStrMap;
	// 保存子弹对象， 应用于先知技能
	CObjectStrMap					m_mBullObject;
	
	// 保存加载的NPC的内部ID
	// 主要用在客户端预计算自动技能施放范围内的怪
	std::vector<INT>				m_vNpcObjectIndex;

	/// 主目标对象
	CObject*						m_pMainTarget;


	//====== UI模型
	// 将所有的UI相关的模型放在这里统一进行管理

	/// 选中的主目标
	CObject_PlayerOther*			m_pMainTargetAvatar;
//	CObject_PlayerOther*			m_pAvatar[2];			/// 男女主角
															/// 宠物


	/// 目前被关心的逻辑对象
	std::map<std::string, OBJECT_BECARED>	m_mapCaredObject;

	/// 资源加载队列
	CObjectLoadQueue*				m_pLoadQueue;
	/// 资源销毁队列
	CObjectDestoryQueue*			m_pDestoryQueue;


	/// 当前选择的是否是对友
	BOOL							m_bIsSelTeam;

	/// 选择的队友是否在眼前
	BOOL							m_bIsSelTeamInPresent;

	/// 选中队友的gudi
	GUID_t							m_GUID;	


};	// class


#endif	// _OBJECT_MANAGER_H