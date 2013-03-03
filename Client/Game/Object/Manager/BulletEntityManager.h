
/**	子弹管理器 
 */


#pragma once

#include "..\Logic\Surface\Obj_Bullet.h"
#include "EffectSystem\FairyBulletFlowSystem.h"
#include "EffectSystem\FairyBulletSystem.h"
#include "EffectSystem\FairyBulletCallbacks.h"
#include "EffectSystem\FairyBulletFlowSystemManager.h"
#include "EffectSystem\FairySkill.h"



/*---------------------------------------------------*/
/*
	子弹信息
*/

enum BULLETINFOTYPE
{
	BULLETINFOTYPE_UNKNOW = 0,
	BULLETINFOTYPE_TARGET_AND_POS,
};


class BulletInfo
{
public:
	BulletInfo(){ nType = BULLETINFOTYPE_UNKNOW; }
	virtual ~BulletInfo(){}

	INT GetInfoType()const { return nType; }

protected:
	INT	nType;
};



class BulletInfo_Target : public BulletInfo
{
public:

	INT						nIndex;				// 对应 BulletFlowSystemList 索引
	INT						nSenderObjID;		// 发送者SeverID
	std::vector<ObjID_t>	vTargetIDlist;		// 目标ID列表
	std::vector<fVector3>	vTargetPosList;		// 目标坐标列表

	Ogre::String targetPoint;
	Ogre::Vector3 offsetPoistion;
	Ogre::Quaternion offsetRotation;

	Ogre::String attachPoint;


	BulletInfo_Target(UINT uNum);
	BulletInfo_Target(const BulletInfo_Target* pInfo);

	virtual ~BulletInfo_Target();	
};

class BulletFlow_Target : public BulletInfo
{
public:

	INT						nIndex;				// 对应 BulletFlowSystemList 索引
	INT						nSenderObjID;		// 发送者SeverID
	fVector3				vSenderPos;
	ObjID_t	vTargetID;		// 目标ID列表
	fVector3	vTargetPos;	// 目标坐标列表

	Ogre::String targetPoint;
	Ogre::Vector3 offsetPoistion;
	Ogre::Quaternion offsetRotation;

	Ogre::String attachPoint;

	BulletFlow_Target();
	BulletFlow_Target(const BulletFlow_Target* pInfo);



	virtual ~BulletFlow_Target();	
};

/*---------------------------------------------------*/
/*
	子弹的引擎回调事件
*/

// 子弹系统创建回调
struct BulletFlowCreateCallback : public Fairy::CreateBulletFlowCallback
{
	virtual VOID onCreateBulletFlow(const Fairy::String& bulletFlowTemplateName, 
		const Fairy::TransformInfo& transInfo, VOID* pSkillUserData);
};


// 子弹创建回调
struct   CreatedBulletCallback : public Fairy::AfterCreatedBulletSystemCallback
{
	virtual VOID onAfterCreatedBulletSystem(Fairy::BulletFlowSystem* bulletFlow, Fairy::BulletSystem* bulletSystem);
};

// 子弹销毁回调
struct DeleteBulletCallback : public Fairy::BeforeDeleteBulletSystemCallback
{
	virtual VOID onBeforeDeleteBulletSystem(Fairy::BulletSystem* bulletSystem);
};


// 技能销毁回调
struct SkillDestroyCallback : public Fairy::DestroySkillCallback
{
	virtual VOID onDestroySkill(Fairy::Skill* pSkill);
};

struct BulletFindTargetCallback :public Fairy::BulletSystemHitTargetCallback
{
	virtual VOID onHitTargetCallback(Fairy::BulletFlowSystem* bulletFlow,Fairy::BulletSystem* bulletSystem);
};


struct BulletCameraShakeCallback : public Fairy::CameraShakeCallback
{
	virtual VOID onShakeCallBack(float amplitude,float duration,float requency);
};
/*---------------------------------------------------*/
/*
	子弹系统逻辑
*/
class BulletFlowSystem_Logic
{
public:
	BulletFlowSystem_Logic( Fairy::BulletFlowSystem* pBulletFlow, ObjID_t nTargetObjID );
	BulletFlowSystem_Logic( Fairy::BulletFlowSystem* pBulletFlow, const BulletFlow_Target* pTarget );

	~BulletFlowSystem_Logic(VOID);
	Fairy::BulletFlowSystem* getFlowSystem(){ return m_pBulletFlow;}

public:
	VOID Tick( UINT time);

	// 子弹生命是否已经结束
	BOOL IsTimeOver();

	VOID SetID(INT nID) { m_nID = nID;  }
	INT	 GetID()		{ return m_nID; }		

protected:
	INT m_nID;				// SkillBulletInfoList 索引	

	Fairy::BulletFlowSystem* m_pBulletFlow;	// 子弹系统
};




/*---------------------------------------------------*/
/*
	子弹管理器
*/



class CBulletEntityManager
{
public:
	CBulletEntityManager(void);
	~CBulletEntityManager(void);

	static CBulletEntityManager* GetMe(void) { return s_pMe; }

	VOID Tick();


	/* ---------- 客户端实现的逻辑子弹----------- */

	// 创建一个子弹实例
	CObject_Bullet* CreateBulletEffect( SObject_BulletInit* infoBullet );

	// 创建一个子弹实例
	CObject_Bullet* CreateBulletEffect( int nBulletID, int nSenderSeverID=-1, int nTargetSeverID=-1, 
										fVector3* pDesPos=NULL, fVector3* pTargetPos=NULL );

	// 销毁子弹实例，索引为本地ID
	BOOL DestroyBulletByID( int nID );

	// 销毁子弹实例，索引为obj
	BOOL DestroyBulletByObj( CObject_Bullet* pBullet );

	// 查找子弹， 索引为本地ID
	CObject_Bullet* GetBulletByID( int nID );

	// 查找子弹， 索引为本地SeverID
	CObject_Bullet* GetBulletBySeverID( int nSeverID );

	//对象删除回调
	VOID	OnObjectDestroy(INT nID);

	// 根据经验值生成经验特效
	VOID CreateExpEffect(int nExp, int nSenderSeverID, int nTargetSeverID);


	

	/* ---------- 引擎实现的逻辑子弹----------- */

	// 添加技能子弹的目标列表
	VOID CreateSkillBulletInfo(Fairy::Skill* pSkill, INT nSenderID, const ObjID_t* pTargetIDList, INT nTargetNum);

	// 添加一个子弹流程
	INT CreateBulletFlowSystem(Fairy::BulletFlowSystem* pBulletSystem, const BulletFlow_Target* pTarget);	
	

public:
	static BulletFlowCreateCallback		s_BulletFlowCreateCallback;
	static CreatedBulletCallback		s_CreatedBulletCallback;
	static DeleteBulletCallback			s_DeleteBulletCallback;
	static SkillDestroyCallback			s_SkillDestroyCallback;
	static BulletFindTargetCallback		s_BulletFindTargetCallBack;
	static BulletCameraShakeCallback	s_BulletCameraShakeCallback;


protected:

	static CBulletEntityManager* s_pMe;

protected:

	DWORD mFrameTime;
	

	typedef std::map< INT, BulletFlowSystem_Logic* > BulletFlowSystemList;

	BulletFlowSystemList m_BulletFlowSystemList;


};
