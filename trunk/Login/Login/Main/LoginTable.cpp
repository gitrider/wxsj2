#include "stdafx.h"
#include "LoginTable.h"
#include "WXSJ_DBC.h"
#include "Log.h"

using namespace DBC;

#define FILE_CHAR_DEFAULT					"./Config/PlayerTemplate.tab"

_DEFAULT_CHARDATA_TBL g_DefaultCharDataTbl ;

VOID	_DEFAULT_CHARDATA_TBL::Init()
{
	__ENTER_FUNCTION

	enum DefaultRoleTBData
	{	
		TBEnterScence		=0, //起始场景ID
		TBBornX             ,   //出生X坐标
		TBBornY             ,   //出生Y坐标
		TBCamp              ,   //初始阵营

		TBIniLevel          ,   //初始等级	
		TBIniExp            ,   //初始经验
		TBIniHP             ,   //初始生命
		TBIniRage           ,   //初始怒气

		TBIniPneuma			,	//初始元气
		TBIniMaxVigor		,	//初始活力上限
		TBIniSkill1         ,   //初始战斗技能1	
		TBIniSkill2         ,   //初始战斗技能2	

		TBIniSkill3         ,   //初始战斗技能3	
		TBIniSkill4         ,   //初始战斗技能4	
		TBIniSkill5         ,   //初始战斗技能5	
		TBIniAbility1		,   //初始生活技能1	

		TBIniAbility2		,   //初始生活技能2	
		TBIniAbility3		,   //初始生活技能3	
		TBIniAbility4		,   //初始生活技能4	
		TBIniAbility5		,   //初始生活技能5	

		TBMoney				,
		TBIniNum			,
	};

	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_CHAR_DEFAULT );
	INT iTableCount = DBFile.GetRecordsNum();
	for (INT n=0; n<MAX_COUNTRY_NUM; ++n)
	{
		for(INT i = 0;	i<MAX_PREFESSION_NUM;i++)
		{
			for(INT j = 0;j<TBIniNum; j++)
			{
				INT iValue = DBFile.Search_Posistion(i+n*MAX_PREFESSION_NUM, j+3)->iValue;
				Set(n, i,j,iValue);
			}
		}
	}


	Log::SaveLog( LOGIN_LOGFILE, "Init_DefaultChar()  Ok!") ;

	__LEAVE_FUNCTION
}