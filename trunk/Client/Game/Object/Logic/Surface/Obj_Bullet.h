
/** 子弹算法
 */

#ifndef __OBJ_BULLET_H__
#define __OBJ_BULLET_H__


#include "Type.h"
#include "Obj_Effect.h"



struct SObject_BulletInit : public SObject_EffectInit
{
	INT			m_nSendID;			// 发射者ID
	INT			m_nTargetID;		// 目标ID
	INT			m_nBulletID;		// 子弹ID，表索引用	
	INT			m_nSendLogicCount;	// 消息逻辑计数
	fVector3	m_fvTargetPos;		// 目标坐标，目标ID无效时可用

	SObject_BulletInit( VOID )
	{
		m_nSendID			= INVALID_ID;
		m_nTargetID			= INVALID_ID;
		m_nBulletID			= INVALID_ID;	
		m_nSendLogicCount	= -1;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}

	virtual VOID Reset( VOID )
	{
		SObject_EffectInit::Reset();

		m_nSendID			= INVALID_ID;
		m_nTargetID			= INVALID_ID;
		m_nBulletID			= INVALID_ID;
		m_nSendLogicCount	= -1;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}
};


struct _DBC_BULLET_DATA;

class CObject_Bullet : public CObject_Effect
{
public:

	CObject_Bullet();
	virtual ~CObject_Bullet();

public:

	virtual	VOID Initial( VOID *pInit );
	//初始化的简化版，不会读取子弹列表
	//virtual VOID InitialEx( VOID *pInit, const CHAR* pEffectName, const CHAR* hitLocator );
	virtual VOID Tick( VOID );
	
	// 清空累计时间，从新计时
	void CumulateTimeClear()								{ m_fCumulateTime = 0; }
	// 设置 目标
	void SetTargetID(int nID )								{ m_idTarget = nID; }
	// 设置 飞行轨迹
	void SetContrailType( int nType );
	// 设置 飞行起始速度
	void SetSpeed( float fSpeed )							{ m_fSpeed = fSpeed; }								
	// 设置 加速度
	void SetAcceleration( float fAcceleration )				{ m_fAcceleration = fAcceleration; }
	// 设置 角度
	void SetAngle( float fAngle )							{ m_fBulletAngle = fAngle; }
	// 设置 特效
	void SetEffect(const CHAR *pszEffectName, BOOL bLoop)	{ ChangEffect(pszEffectName, bLoop); }
	// 设置 人物受击点名称
	void SetHitEffectLocator( const CHAR *szLocatorName )	{ m_strHitEffectLocator = szLocatorName; }
	//设置 起点ID
	void SetSenderID( UINT nID )							{ m_idSend = nID; }
	//设置 目标ID
	void SetTargetID( UINT nID )							{ m_idTarget = nID; }

	//设置 再次活动, 到达目的地时可切换相应设置的状态
	void SetNextInitData( const SObject_BulletInit& initData, bool bActionAgain = true );	
	

protected:
	
	// 已经击中了
	VOID AlreadyHit( VOID );

	//运算子弹位置，并根据 当前子弹位置 判断是否到达目的地
	bool IsArrive( fVector3 fvTargetPos );
	
	//获取目标坐标
	void GetTargetPos();	

	//旋转算法
	fVector3 Rotate(const fVector3& pos, const fVector3& axis, float angle );
	
	//曲线运算
	void Curve( fVector3 fvTargetPos, float fAngle );

	
	POINT HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);
	//bezier曲线算法,基于hermite算法
	inline POINT BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);


	inline float TransformAngle(float nAngle);

protected:	
	
	ObjID_t						m_idSend;				// 发射者ID
	ObjID_t						m_idTarget;				// 目标ID	

	INT							m_nContrailType;		// 飞行轨迹
	INT							m_nBulletID;
	UINT						m_nSendLogicCount;		// 发起者的逻辑计数	
	
	FLOAT						m_fCumulateTime;		// 子弹飞行累计时间
	FLOAT						m_fCumulateDistance;	// 子弹飞行累计距离
	FLOAT						m_fAcceleration;		// 子弹的加速度	
	FLOAT						m_fBulletAngle;			// 子弹角度
	FLOAT						m_fSpeed;				// 起始速度

	FLOAT						m_fCurvePoint1X;		// 曲线参考点相对比例
	FLOAT						m_fCurvePoint1Y;
	FLOAT						m_fCurvePoint2X;
	FLOAT						m_fCurvePoint2Y;
	
	BOOL						m_bAlreadyHit;			// 已经击中	
	BOOL						m_bActionAgain;
	
	fVector3					m_fvTargetPos;			// 目标位置
	fVector3					m_fvStartPos;			// 起始的坐标
	fVector3					m_fvBulletCurrentPos;	// 当前子弹逻辑坐标	

	
	SObject_BulletInit*         m_pNextInitData;		// 下一个子弹状态
	const _DBC_BULLET_DATA*		m_pBulletData;			// 子弹数据

	std::string					m_strHitEffectLocator;	// 受击绑点名称

protected:

	KL_DECLARE_DYNAMIC(CObject_Bullet);

};


#endif // __OBJ_BULLET_H__
