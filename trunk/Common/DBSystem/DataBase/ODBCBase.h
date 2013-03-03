#ifndef _ODBC_BASE_H_
#define _ODBC_BASE_H_
#include "Type.h"
#include "DBTypes.h"
#include "ODBCInterface.h"


class ODBCBase
{
public:
	enum	DBOP_TYPE
	{
		DB_LOAD,
		DB_SAVE,
		DB_ADDNEW,
		DB_DELETE,
		DB_INIT_EMPTY,	//��ʼ״̬
	};

#ifdef __WINDOWS__
	enum	ODBC_ERROR
	{
		DB_SAME_PRI_KEY = 2601, //�ظ��ؼ��ֶ�
	};
#else 
	enum	ODBC_ERROR
	{
		DB_SAME_PRI_KEY = 1062, //�ظ��ؼ��ֶ�
	};
#endif
protected:
	INT				mResultCount;
	BOOL			mResult;
	DB_NAMES		mDBName;
	DBOP_TYPE		mOPType;
	ODBCInterface*	mInterface;
	DB_QUERY*		GetInternalQuery();

	LONG_DB_QUERY*	GetLongInterQuery();


	INT				GetInternalAffectCount();

	BOOL			IsPrepare();

	BOOL			CheckDBConnect(const CHAR* pInfo);		//���DB�������Ƿ�������������5������

	BOOL			LongLoad();
	BOOL			LongSave(VOID* pSource);

public:


	ODBCBase();
	VOID			SetDBName(DB_NAMES dbName);
	UINT			GetResultCount();


	virtual BOOL	Load();
	virtual BOOL	AddNew();
	virtual BOOL	Delete();
	virtual BOOL	Save(VOID* pSource);






	virtual BOOL	ParseResult(VOID* pResult)	=	0;






	INT				GetErrorCode();
	CHAR*			GetErrorMessage();
};

#endif