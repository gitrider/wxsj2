
/********************************************************************************
*	�ļ�����	GMDP_Struct_Daytips.h
*	�����ˣ�	Nick
*	����ʱ�䣺	2008 �� 4 �� 4 �� 17:00
*
*	����˵����	����daytips.tab�������
*	�޸ļ�¼��
*********************************************************************************/

#pragma once

#include "DB_Struct.h"
#include "GIDBC_Struct.h"
#include "DataBase\WXSJ_DBC.h"




class SplitStringLevelOne;
class SplitStringLevelTwo;



//ÿ����ʾ����
struct _DAY_TIPS_ATTR_TBL
{
	_DBC_DAY_TIPS		m_TableExt[MAX_DAY_TIPS_NUM] ;

	uint						m_Count ;
	uint						m_MaxType;

	_DAY_TIPS_ATTR_TBL()
	{
		memset( m_TableExt, 0, sizeof(DBC_DAY_TIPS)*MAX_DAY_TIPS_NUM ) ;
		m_Count = 0 ;
	};

	INT		Count()		{ return m_Count ; } ;
};

class DaytipsDataMgr
{
protected:
	static DaytipsDataMgr* s_pMe;

public:
	static DaytipsDataMgr* GetMe(void) { return s_pMe; }


public:
	DaytipsDataMgr( VOID );
	~DaytipsDataMgr( VOID );

	BOOL Init( const DBC::DBCFile *pDataBase );

	_DAY_TIPS_ATTR_TBL* GetDayTipsTBL() { return &m_DayTips; }

protected:
	VOID	InitDapTipsAttrTable( const DBC::DBCFile *pDataBase );

	VOID	ReadDayTipsData( SplitStringLevelOne* pSplitL1, 
							 SplitStringLevelTwo* pSplitL2, 
							 const CHAR* pSplitString, INT* pVal,INT MaxCnt );

	VOID	ReadDayTipsValidWeek( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
								  const CHAR* pSplitString,INT pVal[MONTH_WEEK_NUM][WEEK_DAY_NUM] );

	VOID	ReadDayTipsBeginAndEndTime(SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
		const CHAR* pSplitString, VALID_TIME_SLICE* pVal,INT MaxCnt );

	_DAY_TIPS_ATTR_TBL m_DayTips;
};
