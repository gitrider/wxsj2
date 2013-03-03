
/** Obj_Bullet.cpp
 */ 

#include "StdAfx.h"

#include "GameStruct_Skill.h"
#include "GIException.h"
#include "GIUtil.h"
#include "GITimeSystem.h"
#include "GIDBC_Struct.h"
#include "..\..\Manager\ObjectManager.h"
#include "Procedure\GameProcedure.h"
#include "Dbc\GMDataBase.h"
#include "..\Character\Obj_Character.h"

#include "Obj_Bullet.h"

#include <assert.h>

KL_IMPLEMENT_DYNAMIC(CObject_Bullet, GETCLASS(CObject_Effect));


static const float BULLET_ACCELERATION = 0.3f;   //ÿ����ٶ�
static const int   BULLET_TOTALTIME = 10000;
static const float BULLET_ANGLE = 60;
static const int   BULLET_NUM = 4;

static const float BULLET_ANGLE_RATE = 90.0f; //���ٶ�  �Ƕ�/ÿ��
static const float BULLET_ROTATE_RADIUS = 1.0f; //Χ��������ת�İ뾶


static const int Iterative  = 150; // ���߷��������(������150���ڣ� �����㷨����) 
static const int Iterative2 = Iterative*Iterative;	
static const int Iterative3 = Iterative2*Iterative;

static const float  PI = 3.14159265358979323846f;


float Q_rsqrt( float number );


/*--------------------------------------------------
--------------------------------------------------*/
CObject_Bullet::CObject_Bullet()
{
	m_idSend			= INVALID_ID;
	m_idTarget			= INVALID_ID;

	m_nContrailType		= BULLET_CONTRAIL_TYPE_INVALID;
	m_nBulletID			= 0;
	m_nSendLogicCount	= -1;

	m_fCumulateTime		= 0.0f;
	m_fCumulateDistance = 0.0f;
	m_fAcceleration		= 0.0f;
	m_fBulletAngle		= 0.0f;
	m_fSpeed			= 0.0f;

	m_bAlreadyHit		= FALSE;
	m_bActionAgain		= FALSE;	

	m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	m_fvStartPos		= fVector3( -1.f, -1.f, -1.f );
	m_fvBulletCurrentPos= fVector3( -1.f, -1.f, -1.f );		
	
	m_pNextInitData		= NULL;
	m_pBulletData		= NULL;	
}

/*--------------------------------------------------
--------------------------------------------------*/
CObject_Bullet::~CObject_Bullet()
{
	if (m_pNextInitData)
		delete m_pNextInitData;
}

/*--------------------------------------------------
//--------------------------------------------------*/
//VOID CObject_Bullet::InitialEx( VOID *pInit, const CHAR* pEffectName, const CHAR* hitLocator )
//{	
//	CObject_Effect::Initial(pInit);
//
//	SObject_BulletInit *pBulletInit = (SObject_BulletInit*)(pInit);
//	if( pBulletInit != NULL )
//	{
//		//�ӳ�ʼ����Ϣ����ȡ����
//		m_idSend = pBulletInit->m_idSend;
//
//		m_nBulletID				= pBulletInit->m_nBulletID;
//		m_nSendLogicCount		= pBulletInit->m_nSendLogicCount;		
//		m_bSingleTarget			= pBulletInit->m_bSingleTarget;
//		m_idTarget				= pBulletInit->m_idTarget;
//		m_fvTargetPos			= pBulletInit->m_fvTargetPos;
//		m_fBulletAngle			= pBulletInit->m_fBulletAngle;		
//		m_fAcceleration			= pBulletInit->m_fAcceleration;	
//		m_fSpeed				= pBulletInit->m_fSpeed;
//		m_nContrailType			= pBulletInit->m_nContrailType;
//		m_strHitEffectLocator	= hitLocator;
//
//	}	
//
//	
//	switch(m_nContrailType)
//	{
//	case BULLET_CONTRAIL_TYPE_BEELINE:	// ֱ��
//	case BULLET_CONTRAIL_TYPE_PARABOLA:// ������
//	case BULLET_CONTRAIL_TYPE_ROTATE:
//	case 100:
//	case 101:
//	case BULLET_CONTRAIL_TYPE_EXP:
//		{
//			m_fvStartPos		 = GetPosition();
//			m_fvBulletCurrentPos = GetPosition();
//
//			if( strlen( pEffectName ) > 0 )
//			{
//				ChangEffect( pEffectName, FALSE );
//			}
//		}
//		break;
//	// �޹켣��ֱ�ӱ�ը
//	case BULLET_CONTRAIL_TYPE_NONE:
//	default:
//		{
//			m_fvBulletCurrentPos	= m_fvTargetPos;
//			m_fvStartPos			= m_fvTargetPos;
//			SetPosition(m_fvStartPos);
//		}
//		break;
//	}
//	
//	m_bAlreadyHit		= FALSE;
//	m_fCumulateTime		= 0.0f;
//	m_fCumulateDistance = 0.0f;
//
//
//	//����������ʽ
//	SetContrailType(m_nContrailType);
//}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_Bullet::Initial( VOID *pInit )
{
	CObject_Effect::Initial(pInit);

	SObject_BulletInit *pBulletInit= (SObject_BulletInit*)(pInit);
	if( pBulletInit != NULL )
	{
		m_nBulletID = pBulletInit->m_nBulletID;

		if( pBulletInit->m_nBulletID != -1 )
		{
			//����
			DBC_DEFINEHANDLE(s_pBulletDataDBC, DBC_BULLET_DATA);
			m_pBulletData = (const _DBC_BULLET_DATA*)(s_pBulletDataDBC->Search_Index_EQU( pBulletInit->m_nBulletID ));
			if (m_pBulletData)
			{
				m_nContrailType			= m_pBulletData->m_nContrailType;
				m_fSpeed				= m_pBulletData->m_fSpeed;
				m_fAcceleration			= m_pBulletData->m_fAcceleration;				
				m_strHitEffectLocator	= m_pBulletData->m_szHitEffectLocator;
			}			
		}
		else
		{
			m_pBulletData = NULL;
		}

		//�ӳ�ʼ����Ϣ����ȡ����
		m_idSend			= pBulletInit->m_nSendID;
		m_idTarget			= pBulletInit->m_nTargetID;
		m_nSendLogicCount	= pBulletInit->m_nSendLogicCount;		
		m_fvTargetPos		= pBulletInit->m_fvTargetPos;

		switch(m_nContrailType)
		{
		case BULLET_CONTRAIL_TYPE_BEELINE:	// ֱ��
		case BULLET_CONTRAIL_TYPE_PARABOLA:// ������
		case BULLET_CONTRAIL_TYPE_ROTATE:
		case 100:
		case 101:
		case BULLET_CONTRAIL_TYPE_EXP:
			{
				m_fvStartPos		 = GetPosition();
				m_fvBulletCurrentPos = GetPosition();

				if (m_pBulletData && strlen( m_pBulletData->m_szFlyEffect ) > 0)
				{
					ChangEffect( m_pBulletData->m_szFlyEffect, FALSE );
				}
			}
			break;
		// �޹켣��ֱ�ӱ�ը
		case BULLET_CONTRAIL_TYPE_NONE:
		default:
			{
				m_fvBulletCurrentPos	= m_fvTargetPos;
				m_fvStartPos			= m_fvTargetPos;

				SetPosition(m_fvStartPos);
			}
			break;
		}
		
		m_bAlreadyHit		= FALSE;
		m_fCumulateTime		= 0.0f;
		m_fCumulateDistance = 0.0f;

		//����������ʽ
		SetContrailType(m_nContrailType);
	}
	else
	{
		m_idSend			= INVALID_ID;
		m_idTarget			= INVALID_ID;
		m_pBulletData		= NULL;
		m_nSendLogicCount	= -1;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_Bullet::SetContrailType(int nType)
{
	m_nContrailType = nType;

	switch(m_nContrailType)
	{
	case BULLET_CONTRAIL_TYPE_PARABOLA://������
		{
			m_fCurvePoint1X = 0.0f;		//	1						 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		break;
	case 100:
		{
			m_fCurvePoint1X = 0.0f;		//	1		   2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 0.5f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		break;
	case 101:
		{
			m_fCurvePoint1X = 0.5f;		//	            1			 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		break;
	case BULLET_CONTRAIL_TYPE_EXP:
		{
			m_fCurvePoint1X = 0.0f;		//	1
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 0.5f;		//
			m_fCurvePoint2Y = 0.0f;		//  x			2			 x		
		}
		break;
	default:
		{
			m_fCurvePoint1X = 0.0f;		//	1						 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		break;
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_Bullet::Tick( VOID )
{
	// �Ѿ�����
	if( m_bAlreadyHit )
	{
		CObject_Effect::Tick();
	}
	else
	{
		//��ȡĿ������
		GetTargetPos();

		//��¼��ʱ��
		m_fCumulateTime += CGameProcedure::s_pTimeSystem->GetDeltaTime();		


		//�ӵ��켣
		switch (m_nContrailType)
		{
			//����ֱ��
		case BULLET_CONTRAIL_TYPE_BEELINE:
			{
				// �ӽ�Ŀ���
				if (IsArrive(m_fvTargetPos))
				{
					SetPosition( m_fvTargetPos );					

					AlreadyHit();
				}
				// ���о���
				else
				{
					SetPosition( m_fvBulletCurrentPos );
				}
			}
			break;
			//����������
		case BULLET_CONTRAIL_TYPE_PARABOLA:
		case 100:
		case 101:
		case BULLET_CONTRAIL_TYPE_EXP:
			{
				// �ӽ�Ŀ���
				if (IsArrive(m_fvTargetPos))
				{
					SetPosition( m_fvTargetPos );					
					
					AlreadyHit();
				}
				// ���о���
				else
				{
					Curve (m_fvTargetPos, TransformAngle(m_fBulletAngle));
				}
			}
			break;
		case BULLET_CONTRAIL_TYPE_ROTATE://Χ��������ת
			{
				float fCurrentAngle = m_fCumulateTime * 0.001f * BULLET_ANGLE_RATE + m_fBulletAngle;//��ǰ�Ƕ�
				int   nCurrentAngle = (int)fCurrentAngle % 360;
				float fAngle		= PI / 180.0f * (float)nCurrentAngle;
				
				m_fvBulletCurrentPos.y = m_fvTargetPos.y;
				m_fvBulletCurrentPos.x = m_fvTargetPos.x + sin(fAngle) * BULLET_ROTATE_RADIUS;
				m_fvBulletCurrentPos.z = m_fvTargetPos.z + cos(fAngle) * BULLET_ROTATE_RADIUS;

				FLOAT fDir = KLU_GetYAngle( fVector2( m_fvBulletCurrentPos.x, m_fvBulletCurrentPos.z ),
											fVector2( m_fvTargetPos.x, m_fvTargetPos.z ) );	
				SetFaceDir( fDir );
				SetPosition( m_fvBulletCurrentPos );										
			}
			break;
			//��ʱ
		case BULLET_CONTRAIL_TYPE_NONE:
		default:
			AlreadyHit();
			break;
		}
	}
}

/*--------------------------------------------------
�����ӵ�λ�ã������� ��ǰ�ӵ�λ�� �ж��Ƿ񵽴�Ŀ�ĵ�
--------------------------------------------------*/
bool CObject_Bullet::IsArrive( fVector3 fvTargetPos )
{
	float t = CGameProcedure::s_pTimeSystem->GetDeltaTime() / 1000.0f;
	//vt=v0+at
	//ǰһ������ٶ�
	float fSpeed = m_fSpeed + m_fAcceleration * (m_fCumulateTime / 1000.0f - t);

	//S=V0t+1/2at2	
	float fCurTickFlyDist = fSpeed * t + m_fAcceleration * t * t / 2;//���������еľ���

	m_fCumulateDistance += fCurTickFlyDist; //�ۼƷ��о���

	fVector3 vDir = fvTargetPos - m_fvBulletCurrentPos;
	vDir.normalise();
	fVector3 vFlyLength;
	vFlyLength.x = vDir.x * fCurTickFlyDist;
	vFlyLength.y = vDir.y * fCurTickFlyDist;
	vFlyLength.z = vDir.z * fCurTickFlyDist;
	m_fvBulletCurrentPos = vFlyLength + m_fvBulletCurrentPos;	

	FLOAT fDir = KLU_GetYAngle( fVector2( m_fvBulletCurrentPos.x, m_fvBulletCurrentPos.z ), 
								fVector2( fvTargetPos.x, fvTargetPos.z ) );
	SetFaceDir( fDir );	

	FLOAT fCurTickFlyDistSq = fCurTickFlyDist * fCurTickFlyDist;
	FLOAT fDistSq = KLU_GetDistSq( fvTargetPos, m_fvBulletCurrentPos );//��ǰλ�õ�Ŀ���ֱ�߾���

	// �ӽ�Ŀ���
	if( fDistSq <= fCurTickFlyDistSq )	
		return true;	
	else	
		return false;
	
	return false;
}

/*--------------------------------------------------
����
--------------------------------------------------*/
void CObject_Bullet::Curve( fVector3 fvTargetPos, float fAngle )
{

	float fDist = KLU_GetDist(m_fvBulletCurrentPos, fvTargetPos);//�߼��ӵ� �� Ŀ��ľ���
	fDist += m_fCumulateDistance;	//����ʼ�㵽Ŀ�����ܷ��о���

	//S=V0t+1/2at^2
	float t = m_fCumulateTime / 1000.0f;
	float fS = m_fSpeed * t + m_fAcceleration * t * t / 2;	
	if (fS > fDist)
	{
		fS = fDist;
	}
	//���ƶ����뻻��ɶ���
	float iter = fS / fDist * (float)Iterative;
	

	int nDist = (int)(fDist * 10.0f);//������룬��������
	//�õ������������
	float x1, x2, y1, y2;
	x1 = (float)nDist * m_fCurvePoint1X;
	y1 = (float)nDist * m_fCurvePoint1Y;
	x2 = (float)nDist * m_fCurvePoint2X;
	y2 = (float)nDist * m_fCurvePoint2Y;

	POINT point = BezierCurve(  0,          0,			//1
								(int)nDist, 0,			//2		
								(int)x1,    (int)y1 ,	//3		
								(int)x2,    (int)y2,	//4		
								(int)iter );	


	fVector3 vDir = m_fvTargetPos - m_fvBulletCurrentPos;
	vDir.normalise();//��λʸ��		
	

	//2d-3d�任	
	float pointX = (float)point.x / 10.0f;
	float pointY = (float)point.y / 10.0f;
	float pointZ = 0.0f;

	//��Y��ת
	float angleY = KLU_GetYAngle( fVector2(vDir.x, vDir.z), fVector2(0.0f, 0.0f));
	angleY = angleY + PI * 0.5f;

	float fx1 = pointZ * sin(angleY) + pointX * cos(angleY);
	float fy1 = pointY;
	float fz1 = pointZ * cos(angleY) - pointX * sin(angleY);

	//��Z����ת
	float angleZ = KLU_GetYAngle( fVector2(vDir.y, vDir.x), fVector2(0.0f, 0.0f));
	if (vDir.x > KLU_MINFLOAT)
	{
		if (vDir.y > KLU_MINFLOAT)
			angleZ = abs(PI - angleZ);
		else
			angleZ = PI + angleZ;
	}	

	float fx2 = fx1 * cos(angleZ) - fy1 * sin(angleZ);
	float fy2 = fx1 * sin(angleZ) + fy1 * cos(angleZ);
	float fz2 = fz1;

	//�Ƕ���ת�任
	fVector3 fvPos = Rotate(fVector3(fx2, fy2, fz2), m_fvTargetPos - m_fvBulletCurrentPos, fAngle);

	//���������ʼ��	
	fVector3 fvStartPos;
	fvStartPos.x = m_fvTargetPos.x - fDist * vDir.x;
	fvStartPos.y = m_fvTargetPos.y - fDist * vDir.y;
	fvStartPos.z = m_fvTargetPos.z - fDist * vDir.z;
	//ƽ�Ƶ���������
	fVector3 fvCurrentPos;	
	fvCurrentPos.x = fvStartPos.x + fvPos.x;
	fvCurrentPos.y = fvStartPos.y + fvPos.y;
	fvCurrentPos.z = fvStartPos.z + fvPos.z;
	
	//��λ
	SetPosition( fvCurrentPos );

	FLOAT fDir = KLU_GetYAngle( fVector2( fvCurrentPos.x, fvCurrentPos.z ), 
								fVector2( fvTargetPos.x, fvTargetPos.z ) );	
	SetFaceDir( fDir );
	

		
}
/*--------------------------------------------------
--------------------------------------------------*/
void CObject_Bullet::GetTargetPos()
{
	fVector3 fvTargetPos = m_fvTargetPos;

	
	
	//�õ�Ŀ������
	CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject( m_idTarget ));
	if( pObj != NULL )
	{
		// ������ڴ���㣬 ȡ��λ��
		if( pObj->GetRenderInterface() != NULL && m_strHitEffectLocator.size() > 0 )
		{
			pObj->GetRenderInterface()->Actor_GetLocator( m_strHitEffectLocator.c_str(), fvTargetPos );
		}
		// ֱ��ʹ��Ŀ������λ��
		else
		{
			fvTargetPos = pObj->GetPosition();
		}
	}		
	

	m_fvTargetPos = fvTargetPos;
}


/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_Bullet::AlreadyHit( VOID )
{
	//����һ��״̬
	if (m_bActionAgain)
	{
		m_bActionAgain = false;

		if (m_pNextInitData)
		{
			//����������ʼ��

			tObject* obj = CObjectManager::GetMe()->FindServerObject(m_pNextInitData->m_nSendID);
			obj->GetRenderInterface()->Actor_GetLocator(GetCharaLocatorName(LOCATOR_CHAR_ATTACK), m_pNextInitData->m_fvPos);	// "���������ܻ���"


			//���³�ʼ��
			Initial(m_pNextInitData);
		}
	}
	//û����һ��״̬
	else
	{
		if(m_idTarget != INVALID_ID)
		{
			CObject_Character *pChar = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_idTarget));
			if(pChar != NULL)
			{
				pChar->ShowLogicEvent(m_idSend, m_nSendLogicCount, TRUE);
			}
		}

		m_bAlreadyHit = TRUE;

		if( m_pBulletData != NULL && strlen( m_pBulletData->m_szHitEffect ) > 0 )
		{
			SetPosition( m_fvTargetPos );
			ChangEffect( m_pBulletData->m_szHitEffect, FALSE );		
		}
		else
		{
			if(m_pRenderInterface != NULL)
				m_pRenderInterface->Detach_Effect();
		}
	}
}


/*--------------------------------------------------
	x1,y1,x2,y2     =   ���߶˵㣬���������1000����
	xr1,yr1,xr2,yr2 =   �������ο�����, ���������1000����	
	currIter = λ�����ڶ���
--------------------------------------------------*/ 
POINT CObject_Bullet::HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)   
{
	if (currIter>Iterative)
	{
		assert(!"�ӵ����߶�ֵ����");
		currIter = Iterative;
	}

	long	oldx	= x1,
			oldy	= y1,
			m1		= Iterative3,
			m2		= 0,
			m3		= 0,
			m4		= 0,
			k1		= 0,
			k2		= 0;
	POINT point;
	
	for (int i=0; i<Iterative; ++i)
	{
		k1   =   (i   <<   1)   +   1;
		k2   =   (k1+i)*i   +   k1;  
		m4   +=  (k2   -=   (k1   *=   Iterative));   
		m3   +=  (k1   =   k2   -   k1)   +   Iterative2;   
		m2   -=  (k2   +=   k1);
		m1   +=   k2;

		point.x	= (int) (((long)x1*m1 + (long)x2*m2 + (long)xr1*m3 + (long)xr2*m4) / Iterative3);
		point.y = (int) (((long)y1*m1 + (long)y2*m2 + (long)yr1*m3 + (long)yr2*m4) / Iterative3);    

		if (i >= currIter)
			return point;
	}

	return point;   
}

/*--------------------------------------------------
ͬHermiteCurve
--------------------------------------------------*/
POINT CObject_Bullet::BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)
{    
	return HermiteCurve(x1,y1,x2,y2,3*(xr1-x1),3*(yr1-y1),3*(x2-xr2),3*(y2-yr2), currIter);   
}

/*--------------------------------------------------
pos		Ҫ��ת�ĵ�
axis	����
angle	�Ƕ�
--------------------------------------------------*/
fVector3 CObject_Bullet::Rotate(const fVector3& pos, const fVector3& axis, float angle )
{
	const fVector3			vector		= pos;	
	fVector3				unitAxis	= axis;
	unitAxis.normalise();
	const float				halfAngle	= angle/float(2);
	const float				s			= sin(halfAngle);
	const float				c			= cos(halfAngle);
	const float				x			= unitAxis.x * s;
	const float				y			= unitAxis.y * s;
	const float				z			= unitAxis.z * s;
	const float				w			= c;
	const float				xx			= x*x;
	const float				xy 			= y*x;
	const float				xz			= z*x;
	const float				yy			= y*y;
	const float				yz			= z*y;
	const float				zz			= z*z;
	const float				wx			= w*x;
	const float				wy			= w*y;
	const float				wz			= w*z;
	const float				M[3][3]		=
	{
		{float(1)-float(2)*(yy+zz),		float(2)*(xy-wz),				float(2)*(xz+wy)},
		{float(2)*(xy+wz),				float(1)-float(2)*(xx+zz),      float(2)*(yz-wx)},
		{float(2)*(xz-wy),				float(2)*(yz+wx),				float(1)-float(2)*(xx+yy)},
	};

	return fVector3( 
		vector.x*M[0][0] + vector.y*M[0][1] + vector.z*M[0][2],
		vector.x*M[1][0] + vector.y*M[1][1] + vector.z*M[1][2],
		vector.x*M[2][0] + vector.y*M[2][1] + vector.z*M[2][2] );
}

/*--------------------------------------------------
--------------------------------------------------*/
float CObject_Bullet::TransformAngle(float nAngle)
{
	return PI / 180.0f * nAngle;
}



/*--------------------------------------------------
--------------------------------------------------*/
void CObject_Bullet::SetNextInitData( const SObject_BulletInit& initData, bool bActionAgain )
{
	if (NULL == m_pNextInitData)
		m_pNextInitData = new SObject_BulletInit;

	memcpy(m_pNextInitData, &initData, sizeof(SObject_BulletInit));

	m_bActionAgain = bActionAgain;
}

/*--------------------------------------------------
--------------------------------------------------*/
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
 //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	return y;
}
