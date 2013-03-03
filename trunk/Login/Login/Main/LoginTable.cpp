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
		TBEnterScence		=0, //��ʼ����ID
		TBBornX             ,   //����X����
		TBBornY             ,   //����Y����
		TBCamp              ,   //��ʼ��Ӫ

		TBIniLevel          ,   //��ʼ�ȼ�	
		TBIniExp            ,   //��ʼ����
		TBIniHP             ,   //��ʼ����
		TBIniRage           ,   //��ʼŭ��

		TBIniPneuma			,	//��ʼԪ��
		TBIniMaxVigor		,	//��ʼ��������
		TBIniSkill1         ,   //��ʼս������1	
		TBIniSkill2         ,   //��ʼս������2	

		TBIniSkill3         ,   //��ʼս������3	
		TBIniSkill4         ,   //��ʼս������4	
		TBIniSkill5         ,   //��ʼս������5	
		TBIniAbility1		,   //��ʼ�����1	

		TBIniAbility2		,   //��ʼ�����2	
		TBIniAbility3		,   //��ʼ�����3	
		TBIniAbility4		,   //��ʼ�����4	
		TBIniAbility5		,   //��ʼ�����5	

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