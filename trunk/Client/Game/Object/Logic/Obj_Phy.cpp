
/** �����obj
 *	������������ײ�Ĵ���tick
 *	��ʱ��һ����������ʵ��
 */

#include "StdAfx.h"
#include <math.h>
#include "Obj_Phy.h"
#include "Global.h"
#include "GITimeSystem.h"
#include "World\WorldManager.h"
#include "Procedure\GameProcedure.h"
#include "GIException.h"
#include <limits>


KL_IMPLEMENT_DYNAMIC(CObject_Phy, GETCLASS(CObject));
 
#define PHY_MACRO_GRAVITY					(9.8f)
#define PHY_MACRO_MSECONDS_PER_FRAME		(10)

static	const FLOAT	SF_Factor = (FLOAT)((FLOAT)PHY_MACRO_MSECONDS_PER_FRAME/(FLOAT)1000);



CObject_Phy::CObject_Phy()
{
	m_fvLinearSpeed =	fVector3(0.0f, 0.0f, 0.0f);
	m_fvRotSpeed	=	fVector3(0.0f, 0.0f, 0.0f);
	m_bIsEnable		=	FALSE;
	m_nLastTickTime	=	0;
	m_bIsInAir		=	FALSE;
}

CObject_Phy::~CObject_Phy()
{
}

VOID CObject_Phy::Initial(VOID* pParma)
{
	for(UINT i =0; i<MAX_REGISTER_EVENTS_NUM; i++)
	{
		m_aEventList[i] = PE_NONE;
	}
	m_nEventListNum = 0;

	CObject::Initial(pParma);
	return;
}

VOID CObject_Phy::AddLinearSpeed(fVector3 vSpeed)
{
	if(m_bIsInAir == TRUE)
		return;
	m_fvLinearSpeed = m_fvLinearSpeed+vSpeed;
}

VOID CObject_Phy::PhyEnable(BOOL bFlag)
{
	if(bFlag == FALSE)
	{
		m_fvLinearSpeed =	fVector3(0.0f, 0.0f, 0.0f);
		m_fvRotSpeed	=	fVector3(0.0f, 0.0f, 0.0f);
	}
	m_nLastTickTime	=	0;
	m_bIsEnable		=	bFlag;
}


VOID CObject_Phy::Tick(VOID)
{
	//��������ϵͳ������λ�ý��н���
	if(m_bIsEnable == FALSE)
	{
		return;
	}

	//��ǰλ��
	fVector3	fvTempPos	= CObject::GetPosition();
	fVector3	fvCurPos	= fvTempPos;
	//����ÿ�� 50����
	UINT	nCurTime	=	CGameProcedure::s_pTimeSystem->GetTimeNow();

	//��һ�β���tick
	if(m_nLastTickTime == 0)
	{
		//��¼�ϴ�ʱ���
		m_nLastTickTime	=	nCurTime;
		return;
	}

	UINT	nDeltaTime	=	CGameProcedure::s_pTimeSystem->CalSubTime(m_nLastTickTime, nCurTime);
	if(nDeltaTime<PHY_MACRO_MSECONDS_PER_FRAME)
	{
		return;
	}

	UINT	nStridTimes	= (UINT)(nDeltaTime/PHY_MACRO_MSECONDS_PER_FRAME);
	FLOAT	fUsedGravity	=	PHY_MACRO_GRAVITY;

	while(nStridTimes != 0)
	{
		//�����ٶ�Ҫ�и���������ʵ�����Ե�����̫���ˣ�
		if(m_fvLinearSpeed.y > 0)
		{
			fUsedGravity = PHY_MACRO_GRAVITY + 25.0f;
		}
		else
		{
			fUsedGravity = PHY_MACRO_GRAVITY + 70.0f;
		}

		//���㴹ֱ���ٶȱ仯
		m_fvLinearSpeed.y -= fUsedGravity*SF_Factor;

		//����λ��
		fvCurPos = fvCurPos+m_fvLinearSpeed*SF_Factor;

		//
		nStridTimes--;
	}
	
	//��¼�ϴ�ʱ���
	m_nLastTickTime	=	nCurTime;

	//����λ��
	SetPosition(fvCurPos);

	//����λ��
	SetMapPosition(fVector2(fvCurPos.x, fvCurPos.z));

	CObject::Tick();
}

VOID CObject_Phy::SetMapPosition(const fVector2& fvPosition)
{
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	if(pActiveScene==0)
    {
        //������δ���أ�һ�������ڵ�½����ʱAvatar����ʾ��ֱ�Ӵ���
		SetPosition(fVector3(fvPosition.x, 0.0, fvPosition.y));		//����������
		return;
    }

	//��ǰλ��
	fVector3	fvCurObjPos	=	CObject::GetPosition();
	FLOAT		fInAirHeight=	fvCurObjPos.y;

	//---------------------------------------------------
	//����ȡ���ڵ����ϵĸ߶�
	fVector3 fvAtTerrain;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		tGfxSystem::AX_PLAN, fVector3(fvPosition.x, 0.0f, fvPosition.y),
		tGfxSystem::AX_GAME, fvAtTerrain);

	//---------------------------------------------------
	//ȡ�����������ϸ߶�
	FLOAT fHeight = -FLT_MAX;
	{
		if(!(pActiveScene->GetMapHeight(fvPosition, fHeight)))
		{
			fHeight = -FLT_MAX;
		}
	}

	FLOAT fRealHeight = (fHeight > fvAtTerrain.y) ? fHeight:fvAtTerrain.y;

	if(IsPhyEnable() == TRUE )
	{
		if(m_fvLinearSpeed.y < 0)
		{//�½�����
			//---------------------------------------------------
			if(fRealHeight>fInAirHeight)
			{//�Ѿ������
				SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//����������
				DispatchPhyEvent(PE_COLLISION_WITH_GROUND);
				m_bIsInAir = FALSE;
			}
			else
			{//�ڿ������ÿ��еĸ߶�
				SetPosition(fVector3(fvPosition.x, fInAirHeight, fvPosition.y));	//�ڿ���
				m_bIsInAir = TRUE;
			}
		}
		else
		{//�������̣�һ���ڿ���
			if(fRealHeight>fInAirHeight)
			{
				SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));	//�ڿ���
			}
			else
			{
				SetPosition(fVector3(fvPosition.x, fInAirHeight, fvPosition.y));	//�ڿ���
			}
			m_bIsInAir = TRUE;
		}
	}
	else
	{
		SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//����������
	}
	SetFootPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//���µر�λ�ã����������׼
}

//ע��һ�������¼�
VOID CObject_Phy::RegisterPhyEvent(PHY_EVENT_ID eventid)
{
	if( m_nEventListNum == MAX_REGISTER_EVENTS_NUM )
	{
		return;
	}
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
			return;
	}
	m_aEventList[m_nEventListNum++] = eventid;
}

//ע��һ�������¼�
VOID CObject_Phy::UnRegisterPhyEvent(PHY_EVENT_ID eventid)
{
	if( m_nEventListNum==0)
	{
		return;
	}
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
		{
			m_aEventList[i] = m_aEventList[m_nEventListNum-1];
			m_nEventListNum--;
			break;
		}
	}
}

//֪ͨһ�������¼�����
VOID CObject_Phy::NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	return;
}

//�ַ�һ�������¼�
VOID CObject_Phy::DispatchPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
		{//�ѱ�ע��
			NotifyPhyEvent(eventid, pParam);
			return;
		}
	}
}
