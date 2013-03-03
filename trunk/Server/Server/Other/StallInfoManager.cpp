/*
̯λ��Ϣ��������ÿ��������һ����
����̯λ��Ϣ�ļ��Ķ�ȡ��
̯λ���õĹ���������һ�ΰ�̯��̯λ�ѣ�ÿ�ν��׵Ľ���˰���˵��Ƿ���԰�̯��
13:20 2006-2-7 sunyu
*/

#include "stdafx.h"
#include "StallInfoManager.h"
#define MAX_STALL_MAP_SIZE	(512)

#include "Log.h"

StallInfoManager::StallInfoManager()
{
	__ENTER_FUNCTION
	m_StallMap		=	NULL;
	m_nMapHeight	=	0;
	m_nMapWidth		=	0;
	__LEAVE_FUNCTION
}

StallInfoManager::~StallInfoManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

BOOL		StallInfoManager::Init(UINT uMapWidth, UINT uMapHeight)
{
	__ENTER_FUNCTION

	if( uMapWidth>MAX_STALL_MAP_SIZE || uMapHeight>MAX_STALL_MAP_SIZE )
		return FALSE;

	//������0�����������
    if(uMapWidth == 0 || uMapHeight == 0)
	{
		Log::SaveLog( SERVER_LOGFILE, "StallInfoManager::Init error uMapWidth = %d, uMapHeight = %d!\n",
			uMapWidth, 
			uMapHeight 
			);
		return TRUE;
	}

	//���㣬new�ڴ�
	m_nMapHeight	=	uMapHeight;
	m_nMapWidth		=	uMapWidth;

	m_StallMap = new StallInfoData_t*[uMapHeight];
	if( m_StallMap == NULL )
	{
		Assert(0);
		return FALSE;
	}

	for(UINT i = 0; i<uMapHeight; i++)
	{
		m_StallMap[i] = new StallInfoData_t[uMapWidth];
		if ( m_StallMap[i] == NULL )
		{
			Assert(0);
			return FALSE;
		}
		memset(m_StallMap[i], 0, uMapWidth*sizeof(StallInfoData_t));
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

VOID		StallInfoManager::CleanUp()
{
	__ENTER_FUNCTION
	
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return;

	for(UINT i =0; i<m_nMapHeight; i++)
	{
		SAFE_DELETE_ARRAY(m_StallMap[i])
	}
	SAFE_DELETE_ARRAY(m_StallMap);
	m_nMapHeight = 0;
	m_nMapWidth  = 0;

	__LEAVE_FUNCTION
}

BOOL		StallInfoManager::Load( CHAR* filename ) 
{
	__ENTER_FUNCTION

	UINT	CurVersion = 1;
	FILE*	pFile = NULL;

	UINT	dwVersion = 0;
	UINT	dwStallWidth = 0;
	UINT	dwStallHeight = 0;

	pFile = ::fopen(filename, "rb");
	if(NULL == pFile)
	{
		return FALSE;
	}

	// ��ȡ�汾��
	::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
	// �汾����
	if(dwVersion!=CurVersion)
	{
		::fclose(pFile);
		return FALSE;
	}

	// ̯λ�Ĵ�С
	::fread(&dwStallWidth,  sizeof(dwStallWidth),  1, pFile);
	::fread(&dwStallHeight, sizeof(dwStallHeight), 1, pFile);

	if(dwStallWidth != m_nMapWidth || dwStallHeight != m_nMapHeight)
	{//̯λ�ļ��ĵ�ͼ��С��ʵ�ʵ�ͼ��С��һ����
		Assert(0);
		::fclose(pFile);
		return FALSE;
	}

    //��ͼû�б���ʼ��
	if(NULL == m_StallMap)
	{
		::fclose(pFile);
		return FALSE;
	}

	for(UINT i = 0; i < dwStallHeight; i++)
	{
		for(UINT j = 0; j < dwStallWidth; j++)
		{
			StallInfoData_t&	stallInfo =  m_StallMap[i][j];
			::fread(&(stallInfo.m_bCanStall),  sizeof(stallInfo.m_bCanStall),  1, pFile);
			::fread(&(stallInfo.m_iTradeTax),  sizeof(stallInfo.m_iTradeTax),  1, pFile);
			::fread(&(stallInfo.m_dwPosTax),   sizeof(stallInfo.m_dwPosTax),   1, pFile);
			::fread(&(stallInfo.m_iExtraInfo), sizeof(stallInfo.m_iExtraInfo), 1, pFile);
		}
	}
	::fclose(pFile);
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL		StallInfoManager::CanStall(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION

	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return FALSE;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_bCanStall;
	}
	__LEAVE_FUNCTION
	return FALSE;

}

BYTE		StallInfoManager::StallExchangeTax(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION

	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_iTradeTax;
	}
	__LEAVE_FUNCTION
	return 0;
}

UINT		StallInfoManager::StallPosPayment(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_dwPosTax;
	}
	__LEAVE_FUNCTION
	return 0;
}

BYTE		StallInfoManager::StallExtraInfo(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_iExtraInfo;
	}
	__LEAVE_FUNCTION
	return 0;
}

VOID		StallInfoManager::SetCanStall(UINT pos_x, UINT pos_y, BOOL bIsCanStall)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		m_StallMap[pos_x][pos_y].m_bCanStall = bIsCanStall;
	}
	__LEAVE_FUNCTION

}