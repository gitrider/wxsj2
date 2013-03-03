
/** �ӵ��㷨
 */

#ifndef __OBJ_BULLET_H__
#define __OBJ_BULLET_H__


#include "Type.h"
#include "Obj_Effect.h"



struct SObject_BulletInit : public SObject_EffectInit
{
	INT			m_nSendID;			// ������ID
	INT			m_nTargetID;		// Ŀ��ID
	INT			m_nBulletID;		// �ӵ�ID����������	
	INT			m_nSendLogicCount;	// ��Ϣ�߼�����
	fVector3	m_fvTargetPos;		// Ŀ�����꣬Ŀ��ID��Чʱ����

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
	//��ʼ���ļ򻯰棬�����ȡ�ӵ��б�
	//virtual VOID InitialEx( VOID *pInit, const CHAR* pEffectName, const CHAR* hitLocator );
	virtual VOID Tick( VOID );
	
	// ����ۼ�ʱ�䣬���¼�ʱ
	void CumulateTimeClear()								{ m_fCumulateTime = 0; }
	// ���� Ŀ��
	void SetTargetID(int nID )								{ m_idTarget = nID; }
	// ���� ���й켣
	void SetContrailType( int nType );
	// ���� ������ʼ�ٶ�
	void SetSpeed( float fSpeed )							{ m_fSpeed = fSpeed; }								
	// ���� ���ٶ�
	void SetAcceleration( float fAcceleration )				{ m_fAcceleration = fAcceleration; }
	// ���� �Ƕ�
	void SetAngle( float fAngle )							{ m_fBulletAngle = fAngle; }
	// ���� ��Ч
	void SetEffect(const CHAR *pszEffectName, BOOL bLoop)	{ ChangEffect(pszEffectName, bLoop); }
	// ���� �����ܻ�������
	void SetHitEffectLocator( const CHAR *szLocatorName )	{ m_strHitEffectLocator = szLocatorName; }
	//���� ���ID
	void SetSenderID( UINT nID )							{ m_idSend = nID; }
	//���� Ŀ��ID
	void SetTargetID( UINT nID )							{ m_idTarget = nID; }

	//���� �ٴλ, ����Ŀ�ĵ�ʱ���л���Ӧ���õ�״̬
	void SetNextInitData( const SObject_BulletInit& initData, bool bActionAgain = true );	
	

protected:
	
	// �Ѿ�������
	VOID AlreadyHit( VOID );

	//�����ӵ�λ�ã������� ��ǰ�ӵ�λ�� �ж��Ƿ񵽴�Ŀ�ĵ�
	bool IsArrive( fVector3 fvTargetPos );
	
	//��ȡĿ������
	void GetTargetPos();	

	//��ת�㷨
	fVector3 Rotate(const fVector3& pos, const fVector3& axis, float angle );
	
	//��������
	void Curve( fVector3 fvTargetPos, float fAngle );

	
	POINT HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);
	//bezier�����㷨,����hermite�㷨
	inline POINT BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);


	inline float TransformAngle(float nAngle);

protected:	
	
	ObjID_t						m_idSend;				// ������ID
	ObjID_t						m_idTarget;				// Ŀ��ID	

	INT							m_nContrailType;		// ���й켣
	INT							m_nBulletID;
	UINT						m_nSendLogicCount;		// �����ߵ��߼�����	
	
	FLOAT						m_fCumulateTime;		// �ӵ������ۼ�ʱ��
	FLOAT						m_fCumulateDistance;	// �ӵ������ۼƾ���
	FLOAT						m_fAcceleration;		// �ӵ��ļ��ٶ�	
	FLOAT						m_fBulletAngle;			// �ӵ��Ƕ�
	FLOAT						m_fSpeed;				// ��ʼ�ٶ�

	FLOAT						m_fCurvePoint1X;		// ���߲ο�����Ա���
	FLOAT						m_fCurvePoint1Y;
	FLOAT						m_fCurvePoint2X;
	FLOAT						m_fCurvePoint2Y;
	
	BOOL						m_bAlreadyHit;			// �Ѿ�����	
	BOOL						m_bActionAgain;
	
	fVector3					m_fvTargetPos;			// Ŀ��λ��
	fVector3					m_fvStartPos;			// ��ʼ������
	fVector3					m_fvBulletCurrentPos;	// ��ǰ�ӵ��߼�����	

	
	SObject_BulletInit*         m_pNextInitData;		// ��һ���ӵ�״̬
	const _DBC_BULLET_DATA*		m_pBulletData;			// �ӵ�����

	std::string					m_strHitEffectLocator;	// �ܻ��������

protected:

	KL_DECLARE_DYNAMIC(CObject_Bullet);

};


#endif // __OBJ_BULLET_H__
