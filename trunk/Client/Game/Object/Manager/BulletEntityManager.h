
/**	�ӵ������� 
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
	�ӵ���Ϣ
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

	INT						nIndex;				// ��Ӧ BulletFlowSystemList ����
	INT						nSenderObjID;		// ������SeverID
	std::vector<ObjID_t>	vTargetIDlist;		// Ŀ��ID�б�
	std::vector<fVector3>	vTargetPosList;		// Ŀ�������б�

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

	INT						nIndex;				// ��Ӧ BulletFlowSystemList ����
	INT						nSenderObjID;		// ������SeverID
	fVector3				vSenderPos;
	ObjID_t	vTargetID;		// Ŀ��ID�б�
	fVector3	vTargetPos;	// Ŀ�������б�

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
	�ӵ�������ص��¼�
*/

// �ӵ�ϵͳ�����ص�
struct BulletFlowCreateCallback : public Fairy::CreateBulletFlowCallback
{
	virtual VOID onCreateBulletFlow(const Fairy::String& bulletFlowTemplateName, 
		const Fairy::TransformInfo& transInfo, VOID* pSkillUserData);
};


// �ӵ������ص�
struct   CreatedBulletCallback : public Fairy::AfterCreatedBulletSystemCallback
{
	virtual VOID onAfterCreatedBulletSystem(Fairy::BulletFlowSystem* bulletFlow, Fairy::BulletSystem* bulletSystem);
};

// �ӵ����ٻص�
struct DeleteBulletCallback : public Fairy::BeforeDeleteBulletSystemCallback
{
	virtual VOID onBeforeDeleteBulletSystem(Fairy::BulletSystem* bulletSystem);
};


// �������ٻص�
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
	�ӵ�ϵͳ�߼�
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

	// �ӵ������Ƿ��Ѿ�����
	BOOL IsTimeOver();

	VOID SetID(INT nID) { m_nID = nID;  }
	INT	 GetID()		{ return m_nID; }		

protected:
	INT m_nID;				// SkillBulletInfoList ����	

	Fairy::BulletFlowSystem* m_pBulletFlow;	// �ӵ�ϵͳ
};




/*---------------------------------------------------*/
/*
	�ӵ�������
*/



class CBulletEntityManager
{
public:
	CBulletEntityManager(void);
	~CBulletEntityManager(void);

	static CBulletEntityManager* GetMe(void) { return s_pMe; }

	VOID Tick();


	/* ---------- �ͻ���ʵ�ֵ��߼��ӵ�----------- */

	// ����һ���ӵ�ʵ��
	CObject_Bullet* CreateBulletEffect( SObject_BulletInit* infoBullet );

	// ����һ���ӵ�ʵ��
	CObject_Bullet* CreateBulletEffect( int nBulletID, int nSenderSeverID=-1, int nTargetSeverID=-1, 
										fVector3* pDesPos=NULL, fVector3* pTargetPos=NULL );

	// �����ӵ�ʵ��������Ϊ����ID
	BOOL DestroyBulletByID( int nID );

	// �����ӵ�ʵ��������Ϊobj
	BOOL DestroyBulletByObj( CObject_Bullet* pBullet );

	// �����ӵ��� ����Ϊ����ID
	CObject_Bullet* GetBulletByID( int nID );

	// �����ӵ��� ����Ϊ����SeverID
	CObject_Bullet* GetBulletBySeverID( int nSeverID );

	//����ɾ���ص�
	VOID	OnObjectDestroy(INT nID);

	// ���ݾ���ֵ���ɾ�����Ч
	VOID CreateExpEffect(int nExp, int nSenderSeverID, int nTargetSeverID);


	

	/* ---------- ����ʵ�ֵ��߼��ӵ�----------- */

	// ��Ӽ����ӵ���Ŀ���б�
	VOID CreateSkillBulletInfo(Fairy::Skill* pSkill, INT nSenderID, const ObjID_t* pTargetIDList, INT nTargetNum);

	// ���һ���ӵ�����
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
