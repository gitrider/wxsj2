#include "stdafx.h"
#include "ODBCInterface.h"

ODBCInterface::ODBCInterface()
{
	__ENTER_FUNCTION

	mConnected		= FALSE;
	mAffectCount	=-1;
	mResult			=	SQL_SUCCESS;
	hEnv			=	0;
	hStmt			=	0;
	hDbc			=	0;
	memset(mErrorMsg,0,MAX_ERROR_MSG_LENGHT);
	m_Query.Clear();

	__LEAVE_FUNCTION

}

ODBCInterface::~ODBCInterface()
{
	__ENTER_FUNCTION	

	if (hStmt)	SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
	//if (hDbc)	SQLEndTran(SQL_HANDLE_DBC,hDbc,SQL_COMMIT);
	if (hDbc)	SQLDisconnect(hDbc);
	if (hDbc)	SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
	if (hEnv)	SQLFreeHandle(SQL_HANDLE_ENV,hEnv);

	__LEAVE_FUNCTION
}

BOOL ODBCInterface::Connect(const CHAR *ConnectStr, 
							const CHAR *UserName/* =NULL */,
							const CHAR *Password/* =NULL */)
{
	__ENTER_FUNCTION

	Close();

	strncpy(mConnectStr, ConnectStr,DATABASE_STR_LEN);
	strncpy(mUserName, UserName,DB_USE_STR_LEN);
	strncpy(mPassWord, Password,DB_PASSWORD_STR_LEN);

	SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
	SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);
	mResult=SQLConnect(hDbc,(SQLCHAR *)mConnectStr,SQL_NTS,(SQLCHAR *)mUserName,SQL_NTS,
		(SQLCHAR *)mPassWord,SQL_NTS);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) 
	{
		CHAR LogBuff[512];
		memset(LogBuff,0,512);
		sprintf(LogBuff,"Connect string: %s",mConnectStr);
		sprintf(LogBuff,"Connect string: %s",mUserName);
		DiagState();
		return FALSE;
	}
	//SQLSetConnectAttr(hDbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_OFF,0);

	mResult=	SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) 
	{
		hStmt=0;
		return FALSE;
	}

	mConnected = TRUE;
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ODBCInterface::Connect()
{
	__ENTER_FUNCTION

	Close();
#ifdef MUST_CLOSE_HENV_HANDLE
	SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
	SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
#endif
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);

	mResult=SQLConnect(hDbc,(SQLCHAR *)mConnectStr,SQL_NTS,(SQLCHAR *)mUserName,SQL_NTS,
		(SQLCHAR *)mPassWord,SQL_NTS);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) 
	{
		CHAR LogBuff[512];
		memset(LogBuff,0,512);
		sprintf(LogBuff,"Connect string: %s",mConnectStr);
		sprintf(LogBuff,"Connect string: %s",mUserName);
		DiagState();
		return FALSE;
	}
	//SQLSetConnectAttr(hDbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_OFF,0);

	mResult=	SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) {
		hStmt=0;
		return FALSE;
	}

	mConnected = TRUE;
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ODBCInterface::Close()
{
	__ENTER_FUNCTION

	if (hStmt)	
	{
			_MY_TRY
			{
		SQLCloseCursor(hStmt);
		SQLFreeStmt(hStmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
		hStmt = NULL;
	}
			_MY_CATCH
			{
				hStmt = NULL;
			}
		}
	if (hDbc)
	{
			//SQLEndTran(SQL_HANDLE_DBC,hDbc,SQL_COMMIT);
			_MY_TRY
			{
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
		hDbc = NULL;
	}
			_MY_CATCH
			{
				hDbc = NULL;
			}
		}
#ifdef MUST_CLOSE_HENV_HANDLE
	if (hEnv)	
	{
			_MY_TRY
			{
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		hEnv = NULL;
	}
			_MY_CATCH
			{
				hEnv = NULL;
			}
		}
#endif
	mConnected = FALSE;						//设置连接状态

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ODBCInterface::Execute()
{
	_MY_TRY
	{
		INT ColIndex;
 		mResult=SQLExecDirect(hStmt,(SQLCHAR *)m_Query.m_SqlStr,SQL_NTS);
		if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO) && (mResult != SQL_NO_DATA)) 
		{
			DiagState();

			return FALSE;
		}

			// Update, Delete, Insert 影响的结果
		SQLRowCount(hStmt,&mAffectCount);
		SQLNumResultCols(hStmt,&mColCount);
		if (mColCount > MAXCOL) 
		{
				return FALSE;
		}

		if (mColCount <= 0 && mAffectCount <= 0)
		{
				Clear();
				return TRUE;
		}

		for (ColIndex=0;ColIndex<mColCount;ColIndex++) 
		{
			//SQLBindCol(hStmt,ColIndex+1,SQL_C_CHAR,Col[ColIndex],MAX_COLUMN_BUFFER,&mCollocate[ColIndex]);
			//SQLDescribeCol(hStmt,ColIndex+1,ColName[ColIndex],MAX_COLUMN_NAME,NULL,NULL,NULL,NULL,NULL);
		}
		return TRUE;

	}
	_MY_CATCH
	{
		SaveErrorLog("Huge Error occur:");
		SaveErrorLog((const CHAR*)m_Query.m_SqlStr);//保存错误的Sql语句
	}
	return FALSE;
}

BOOL ODBCInterface::LongExecute()
{
	_MY_TRY
	{
		INT ColIndex;
		mResult=SQLExecDirect(hStmt,(SQLCHAR *)m_LongQuery.m_SqlStr,SQL_NTS);
		if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO) && (mResult != SQL_NO_DATA)) 
		{
			DiagStateEx();
			return FALSE;
		}
		// Update, Delete, Insert 影响的结果
		SQLRowCount(hStmt,&mAffectCount);
		SQLNumResultCols(hStmt,&mColCount);
		if (mColCount > MAXCOL) 
		{
			return FALSE;
		}

		if (mColCount <= 0 && mAffectCount <= 0)
		{
			Clear();
			return TRUE;
		}

		for (ColIndex=0;ColIndex<mColCount;ColIndex++) 
		{
			//SQLBindCol(hStmt,ColIndex+1,SQL_C_CHAR,Col[ColIndex],MAX_COLUMN_BUFFER,&mCollocate[ColIndex]);
			//SQLDescribeCol(hStmt,ColIndex+1,ColName[ColIndex],MAX_COLUMN_NAME,NULL,NULL,NULL,NULL,NULL);
		}
		return TRUE;

	}
	_MY_CATCH
	{
		SaveErrorLog("Huge Error occur:");
		SaveErrorLog((const CHAR*)m_LongQuery.m_SqlStr);//保存错误的Sql语句
	}
	return FALSE;
}

VOID ODBCInterface::Clear_NoCommit()
{
	__ENTER_FUNCTION
		SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
	__LEAVE_FUNCTION
}

VOID ODBCInterface::Clear()
{
	__ENTER_FUNCTION
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
	//SQLEndTran(SQL_HANDLE_DBC,hDbc,SQL_COMMIT);
	__LEAVE_FUNCTION
}


BOOL	ODBCInterface::Fetch()
{
	__ENTER_FUNCTION

	mResult	=	SQLFetch(hStmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO) && (mResult != SQL_NO_DATA)) 
	{
		DiagState();
		return FALSE;
	}

	if(mResult == SQL_NO_DATA)
	{
		return FALSE;
	}

	for (int ColIndex=0;ColIndex<mColCount;ColIndex++)
	{
		int				TotalGet = 0;
		SQLINTEGER		LenData = 0;
		int				Ret		= 0;
		while(Ret=SQLGetData(hStmt, ColIndex+1, SQL_C_CHAR,Col[ColIndex]+TotalGet,MAX_COLUMN_BUFFER,&LenData)!=SQL_NO_DATA)
		{
			break;
		}
		//printf("sql length=%d ret=%d,LenData=%d\r\n",TotalGet,Ret,LenData);
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ODBCInterface::LongFetch()
{
	__ENTER_FUNCTION

		mResult	=	SQLFetch(hStmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO) && (mResult != SQL_NO_DATA)) 
	{
		DiagState();
		return FALSE;
	}

	if(mResult == SQL_NO_DATA)
	{
		return FALSE;
	}

	for (int ColIndex=0;ColIndex<mColCount;ColIndex++)
	{
		int				TotalGet = 0;
		SQLINTEGER		LenData  = 0;
		int				Ret		 = 0;
		/* 请注意，这里有一个潜规则！！！[3/2/2007]
		* Col[ColIndex]的大小是MAX_COLUMN_BUFFER 2049
		* 而这里读取的大小是MAX_LONG_COLUMN_BUFFER	200000
		* 所以当使用LongFetch读取超过2049的列的返回值时，一定要保证这列是最后一列
		* 严重注意！！！严重注意！！！严重注意！！！严重注意！！！
		*/
		while(Ret=SQLGetData(hStmt, ColIndex+1, SQL_C_CHAR,Col[ColIndex]+TotalGet,MAX_LONG_COLUMN_BUFFER,&LenData)!=SQL_NO_DATA)
		{
			break;
		}
		//printf("sql length=%d ret=%d,LenData=%d\r\n",TotalGet,Ret,LenData);
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


int ODBCInterface::GetInt(INT ColIndex,INT& ErrorCode)
{
	__ENTER_FUNCTION

	if (ColIndex > mColCount)
	{
		ErrorCode = QUERYNOCOL;
		Assert(FALSE);
		return QUERYNOCOL;
	}

	if (mCollocate[ColIndex-1]==SQL_NULL_DATA)
	{
		ErrorCode = QUERYNULL;
		Assert(FALSE);
		return QUERYNULL;
	} 
	else 
	{
		ErrorCode = QUERYOK;
		return atoi(Col[ColIndex-1]);
	}

 __LEAVE_FUNCTION

	 return QUERYNULL;
}

UINT ODBCInterface::GetUInt(INT ColIndex,INT& ErrorCode)
{
	__ENTER_FUNCTION

	if (ColIndex > mColCount)
	{
		ErrorCode = QUERYNOCOL;
		Assert(FALSE);
		return QUERYNOCOL;
	}

	if (mCollocate[ColIndex-1]==SQL_NULL_DATA)
	{
		ErrorCode = QUERYNULL;
		Assert(FALSE);
		return QUERYNULL;
	} 
	else 
	{
		ErrorCode =	QUERYOK;
        return (UINT)atoi(Col[ColIndex-1]);
	}

	__LEAVE_FUNCTION

		return QUERYNULL;
}

FLOAT ODBCInterface::GetFloat(INT ColIndex,INT& ErrorCode)
{
	__ENTER_FUNCTION

	if (ColIndex > mColCount)
	{
		ErrorCode =  QUERYNOCOL;
		Assert(FALSE);
		return QUERYNOCOL;
	}

	if (mCollocate[ColIndex-1]==SQL_NULL_DATA) 
	{
		ErrorCode = QUERYNULL;
		Assert(FALSE);
		return QUERYNULL;

	}
	else 
	{
		ErrorCode = QUERYOK;
		return (FLOAT)atof(Col[ColIndex-1]);
	}

	__LEAVE_FUNCTION

		return QUERYNULL;
}


VOID ODBCInterface::GetString(INT ColIndex, CHAR * buf,INT BufLen,INT& ErrorCode)
{
	__ENTER_FUNCTION

	if (ColIndex > mColCount)
	{
		ErrorCode = QUERYNOCOL;
		buf[0] = '\0';
		Assert(FALSE);
		return;
	}

	if (mCollocate[ColIndex-1]==SQL_NULL_DATA) 
	{
		ErrorCode = QUERYNULL;
		buf[0] = '\0';
		Assert(FALSE);
	} 
	else 
	{
		if(BufLen<MAX_COLUMN_BUFFER)
			strncpy(buf,Col[ColIndex-1],BufLen);
		else
		{
			strncpy(buf,Col[ColIndex-1],MAX_COLUMN_BUFFER);
		}
		ErrorCode = QUERYOK;
	}

	__LEAVE_FUNCTION
}

VOID ODBCInterface::GetField(INT ColIndex, CHAR * buf,INT BufLen,INT& ErrorCode)
{
	__ENTER_FUNCTION	

	if (ColIndex > mColCount)
	{
		ErrorCode = QUERYNOCOL;
		buf[0] = '\0';
		Assert(FALSE);
		return;
	}

	if (mCollocate[ColIndex-1]==SQL_NULL_DATA) 
	{
		ErrorCode = QUERYNULL;
		buf[0] = '\0';
		Assert(FALSE);
	} 
	else 
	{
		if(BufLen<MAX_COLUMN_BUFFER)
		{
			UINT OutLength = 0;
			DBStr2Binary(Col[ColIndex-1],MAX_COLUMN_BUFFER,buf,BufLen,OutLength);
			Assert((INT)OutLength<=BufLen);
		}
		else
		{
			memcpy(buf,Col[ColIndex-1],MAX_COLUMN_BUFFER);
			Assert(FALSE);
		}
		ErrorCode = QUERYOK;
	}

	__LEAVE_FUNCTION
}

VOID ODBCInterface::GetLongField(INT ColIndex, CHAR * buf,INT BufLen,INT& ErrorCode)
{
	__ENTER_FUNCTION	

		if (ColIndex > mColCount)
		{
			ErrorCode = QUERYNOCOL;
			buf[0] = '\0';
			Assert(FALSE);
			return;
		}

		if (mCollocate[ColIndex-1]==SQL_NULL_DATA) 
		{
			ErrorCode = QUERYNULL;
			buf[0] = '\0';
			Assert(FALSE);
		} 
		else 
		{
			if(BufLen<MAX_LONG_COLUMN_BUFFER)
			{
				UINT OutLength = 0;
				DBStr2Binary(Col[ColIndex-1],MAX_LONG_COLUMN_BUFFER,buf,BufLen,OutLength);
				Assert((INT)OutLength<=BufLen);
			}
			else
			{
				memcpy(buf,Col[ColIndex-1],MAX_LONG_COLUMN_BUFFER);
				Assert(FALSE);
			}
			ErrorCode = QUERYOK;
		}

		__LEAVE_FUNCTION
}




VOID	ODBCInterface::DiagState()
{
	__ENTER_FUNCTION

	int				ii=1;
	SQLINTEGER		NativeError;
	SQLCHAR			SqlState[6];
	SQLSMALLINT		MsgLen;
	memset(mErrorMsg,0,MAX_ERROR_MSG_LENGHT);	
	while (mResult=SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, ii, SqlState, &NativeError, 
		mErrorMsg, sizeof(mErrorMsg), &MsgLen) !=SQL_NO_DATA) 
	{
		ii++;

	}

	mErrorMsg[MAX_ERROR_MSG_LENGHT-1] = '\0'; //防止错误消息溢出

	if(strlen((const char*)mErrorMsg) == 0)
	{//自定义数据库错误

					mResult	=	SQLError(hEnv,hDbc,hStmt,SqlState,
						&NativeError,mErrorMsg,sizeof(mErrorMsg),&MsgLen);
	}
	mErrorCode =  NativeError;

	switch(mErrorCode) 
	{
	case 2601: //角色重名不需要关闭连接
		break;
	case 1062: //角色重复
		break;
	default:
		{
			Close();
		}
	}

	CHAR ErrorBuffer[512] = {0};
	sprintf(ErrorBuffer,"ErrorCode = %d,ErrorMessage = %s,ErrorSql",mErrorCode,mErrorMsg);
	SaveErrorLog(ErrorBuffer);
	SaveErrorLog((const CHAR*)m_Query.m_SqlStr);//保存错误的Sql语句
	__LEAVE_FUNCTION
}

VOID	ODBCInterface::DiagStateEx()
{
	__ENTER_FUNCTION

		int				ii=1;
	SQLINTEGER		NativeError;
	SQLCHAR			SqlState[6];
	SQLSMALLINT		MsgLen;
	memset(mErrorMsg,0,MAX_ERROR_MSG_LENGHT);	
	while (mResult=SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, ii, SqlState, &NativeError, 
		mErrorMsg, sizeof(mErrorMsg), &MsgLen) !=SQL_NO_DATA) 
	{
		ii++;

	}

	mErrorMsg[MAX_ERROR_MSG_LENGHT-1] = '\0'; //防止错误消息溢出

	if(strlen((const char*)mErrorMsg) == 0)
	{//自定义数据库错误

		mResult	=	SQLError(hEnv,hDbc,hStmt,SqlState,
			&NativeError,mErrorMsg,sizeof(mErrorMsg),&MsgLen);
	}
	mErrorCode =  NativeError;

	switch(mErrorCode) 
	{
	case 2601: //角色重名
		break;
	case 1062: //角色重名
		break;
	default:
		{
			Close();
		}
	}

	CHAR ErrorBuffer[512] = {0};
	sprintf(ErrorBuffer,"ErrorCode = %d,ErrorMessage = %s,ErrorSql",mErrorCode,mErrorMsg);
	SaveErrorLog(ErrorBuffer);
	SaveErrorLog((const CHAR*)m_LongQuery.m_SqlStr);//保存错误的Sql语句
	__LEAVE_FUNCTION
}

VOID ODBCInterface::SaveErrorLog(const CHAR* pLog)
{
	__ENTER_FUNCTION
	if(strlen(pLog)==0)
		return;
	FILE* f = fopen( "./Log/dberror.log", "a" ) ;
	if(f)
	{
		fwrite( pLog, 1, strlen(pLog), f ) ;
		fwrite( "\r\n", 1, 2, f ) ;
		fclose(f) ;
	}
	__LEAVE_FUNCTION
}

VOID ODBCInterface::SaveWarrning(const CHAR* pLog)
{
	__ENTER_FUNCTION
		if(strlen(pLog)==0)
			return;
	FILE* f = fopen( "./Log/dbwarrning.log", "a" ) ;
	if(f)
	{
	fwrite( pLog, 1, strlen(pLog), f ) ;
	fwrite( "\r\n", 1, 2, f ) ;
	fclose(f) ;
	}
	__LEAVE_FUNCTION
}

VOID ODBCInterface::CleanEnv()
{
	if (hStmt)	
	{
		SQLCloseCursor(hStmt);
		SQLFreeStmt(hStmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
		hStmt = NULL;
	}
	if (hDbc)
	{
		//SQLEndTran(SQL_HANDLE_DBC,hDbc,SQL_COMMIT);
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
		hDbc = NULL;
	}
#ifdef MUST_CLOSE_HENV_HANDLE
	if (hEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		hEnv = NULL;
	}
#endif
}

VOID ODBCInterface::Dump(INT col)
{
	FILE* f = fopen( "./Log/field", "a" ) ;
	if(f)
	{
		fwrite( "begin", 1, 5, f ) ;
		fwrite(Col[col-1],1,MAX_COLUMN_BUFFER,f);
		fclose(f) ;
	}
}