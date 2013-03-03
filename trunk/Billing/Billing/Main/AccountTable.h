#ifndef		_ACCOUNT_TABLE_H_
#define		_ACCOUNT_TABLE_H_
#include "Type.h"
#include "StrTable.h"


class AccountTable
{
	struct PASSWORD
	{
		CHAR PW[MAX_PASSWORD];
	};

public:
	AccountTable(){};
	~AccountTable(){};
	

	BOOL	Init();

	BOOL	Check(const CHAR* UserName,const CHAR* PassWord);
private:

	StrTable	mTable;
	PASSWORD*	mPW;
	INT			AccountCount;
};

extern AccountTable	g_Account;

#endif