
#include "stdafx.h"
#include "Ini.h"
#include "Scene.h"
#include "Patrol.h"
#include "SceneManager.h"

VOID PatrolPathMgr::Init( Scene* pScene )
{
	m_pScene = pScene;
}

VOID PatrolPathMgr::CleanUp( VOID )
{
	m_PatrolPathList=NULL;
	m_nPatrolPathCount = 0;
}

VOID PatrolPathMgr::LoadPatrolPoint( const CHAR* filename )
{
__ENTER_FUNCTION
	// ��ָ���ļ���ȡ��Ӧ��Ѳ��·����Ϣ
	PATROL_FILE* pPatrolFile = g_pSceneManager->FindPatrolFile( filename ) ;
	if( pPatrolFile==NULL )
	{//û�ж�ȡ�����������ļ����ȡ
		pPatrolFile = g_pSceneManager->FindEmptyPatrolFile() ;

		CHAR szSection[_MAX_PATH],
			szName[_MAX_PATH],
			szRet[_MAX_PATH];
		Ini ini( filename ) ;

		INT nPatrolNum = ini.ReadInt( "INFO", "PATROLNUMBER" ) ;
		// ��ʼ���ó��������е�Ѳ��·�߸���
		if (nPatrolNum <= 0)
		{
			return;
		}
		pPatrolFile->m_PatrolPathCount = nPatrolNum;
		pPatrolFile->m_PatrolPathList = new PatrolPath[nPatrolNum];
		if ( !pPatrolFile->m_PatrolPathList )
		{
			Assert( FALSE && "m_PatrolPathList = new PatrolPath[nPatrolNum]...fail!" );
			return;
		}

		for(INT i = 0; i < pPatrolFile->m_PatrolPathCount; ++i)
		{
			memset(szSection,0,_MAX_PATH);
			memset(szName,0,_MAX_PATH);
			
			sprintf( szSection, "PATROL%d", i ) ;
			memset(szRet,0,_MAX_PATH);

			ini.ReadText( szSection, "PATROLPOINTNUM", szRet, _MAX_PATH ) ;
			INT nPatrolPointNum = atoi(szRet);
			if (nPatrolPointNum <= 0)
			{
				continue;
			}
			// ��ʼ����Ѳ��·�ߵ�Ѳ�ߵ����
			pPatrolFile->m_PatrolPathList[i].m_nPatrolPointCount = nPatrolPointNum;
            pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList = new PatrolPath::PatrolUnit[nPatrolPointNum];
			if ( !pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList)
			{
				Assert( FALSE && "m_PatrolPathList[i].m_PatrolUnitList = new PatrolPath::PatrolUnit[nPatrolPointNum]...fail!" );
				return ;
			}

			for(INT j = 0; nPatrolPointNum > j; ++j )
			{
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "scriptid%d", j);
				if (ini.ReadTextIfExist(szSection, szName, szRet, sizeof(szRet)) )
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_ScriptID = (INT)atoi(szRet);
				}
				else
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_ScriptID = -1;
				}
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "settletime%d", j);
				if (ini.ReadTextIfExist(szSection, szName, szRet, sizeof(szRet)) )
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_nSettleTime = (INT)atoi(szRet);
				}
				else
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_nSettleTime = 0;
				}
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "POSX%d", j ) ;
				ini.ReadText( szSection, szName, szRet, _MAX_PATH ) ;

				FLOAT fX = (FLOAT)atof(szRet);
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "POSZ%d", j ) ;
				ini.ReadText( szSection, szName, szRet, _MAX_PATH ) ;

				FLOAT fZ = (FLOAT)atof(szRet);

				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "POSY%d", j ) ;
				ini.ReadText( szSection, szName, szRet, _MAX_PATH ) ;

				FLOAT fY = (FLOAT)atof(szRet);
				// ��ʼ����Ѳ��·�ߵ��ض���
				pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_PatrolPoint.m_fX = fX;
				pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_PatrolPoint.m_fZ = fZ;
				pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_fHeight = fY;
			}
		}
	
		strncpy( pPatrolFile->m_szFileName, filename, _MAX_PATH-1 ) ;
	}

	m_nPatrolPathCount = pPatrolFile->m_PatrolPathCount ;
	m_PatrolPathList = pPatrolFile->m_PatrolPathList;

__LEAVE_FUNCTION

}

BOOL PatrolPathMgr::FindPatrolID(INT PatrolID) const
{
	if (PatrolID < 0 || PatrolID > m_nPatrolPathCount-1)
	{
        return FALSE;
	}
	return TRUE;
}

const PatrolPath *PatrolPathMgr::GetPatrolPath(INT nPatrolID)const
{
	if(nPatrolID >= 0 && nPatrolID < m_nPatrolPathCount)
	{
		return &m_PatrolPathList[nPatrolID];
	}
	return NULL;
}

INT PatrolPathMgr::GetPatrolPoint(INT patrolPathIndex, INT patrolPointIndex, BOOL& baHead, WORLD_POS& rTar, INT& rSettleTime, ScriptID_t& rScriptID)
{
__ENTER_FUNCTION
	if ( patrolPathIndex < 0 || patrolPathIndex > m_nPatrolPathCount-1 )
	{
		char szTemp[128];
		tsnprintf(szTemp, 128, "patrolPathIndex=%d...patrolPathIndex < 0 || patrolPathIndex > m_nPatrolPathCount-1", patrolPathIndex);
		Assert(FALSE && szTemp);
		return NULL;
	}

	if ( patrolPointIndex < 0 || patrolPointIndex > m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount-1 )
	{
		char szTemp[128];
		tsnprintf(szTemp, 128, "PatrolPointIndex=%d...patrolPointIndex < 0 || patrolPointIndex > m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount-1", patrolPointIndex);
		Assert(FALSE && szTemp);
		return NULL;
	}

	INT nCount = m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount;
    if ( baHead )
	{// �������������...
		if ( patrolPointIndex+1 >= nCount )
		{// �Ѿ������������ͷ��,���ͷ�ڵ�==ĩ�ڵ㣬��һֱ��������
			WORLD_POS* pCurPos = &(m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_PatrolPoint);
			WORLD_POS* pStartPos = &(m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[0].m_PatrolPoint);
			if ((*pCurPos) == (*pStartPos))
			{// ֱ�ӵ�ͷ�ڵ����һ��λ��
				patrolPointIndex = 1;
			}
			else 
			{
				--patrolPointIndex;
				baHead = FALSE;
			}
		}
		else
		{
			++patrolPointIndex;
		}
	}
	else
	{
		if ( patrolPointIndex-1 < 0 )
		{// �Ѿ����ﷴ�����ͷ��
			patrolPointIndex = 1;
			baHead = TRUE;
		}
		else
		{
			--patrolPointIndex;
		}
	}
	// ��patrolPointIndex�ı߽����ǿ������
	{
		patrolPointIndex < 0 ? patrolPointIndex = 0 : NULL;
		patrolPointIndex+1 > nCount ? patrolPointIndex = nCount-1 : NULL;
	}

	rScriptID = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_ScriptID;
	rSettleTime = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_nSettleTime;
	rTar = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_PatrolPoint;
	return patrolPointIndex;

__LEAVE_FUNCTION
	return NULL;
}