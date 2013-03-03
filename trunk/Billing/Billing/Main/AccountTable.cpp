#include "stdafx.h"
#include "AccountTable.h"
#include "WXSJ_DBC.h"

using namespace DBC;
AccountTable g_Account;

BOOL	AccountTable::Init()	
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT("./Config/Account.txt");
	if(!ret)
		return FALSE;

	INT iTableCount		= ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	if(iTableCount<=0)
		return FALSE;

	mPW = new PASSWORD[iTableCount];
	mTable.InitTable(iTableCount,MAX_ACCOUNT);

	for(INT i=0;i<iTableCount;i++)
	{
		strncpy(mPW[i].PW,ThirdFile.Search_Posistion(i,1)->pString,MAX_PASSWORD);
		mTable.Add(ThirdFile.Search_Posistion(i,0)->pString,
			       mPW[i].PW);
	}
	
	__LEAVE_FUNCTION

	return TRUE;
}	

BOOL	AccountTable::Check(const CHAR* UserName,const CHAR* PassWord)
{
	Assert(UserName);

	CHAR* tPW = 	(CHAR*)mTable.Get(UserName);
	if(!tPW)
		return FALSE;

	Assert(PassWord);

	return !strcmp(PassWord,tPW);



}