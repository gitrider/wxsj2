#ifndef _DB_IMMIGRATION_H_
#define _DB_IMMIGRATION_H_

#include "Type.h"
#include "ODBCBase.h"

class DBImmigration:public ODBCBase
{
	CHAR			m_Account[MAX_ACCOUNT+1];		//用户名称
	GUID_t			m_CharGuid;						//用户GUID
	UINT			m_CreateTime;					//角色创建时间
	CHAR			m_CharName[MAX_CHARACTER_NAME+1];	//角色名称
public:
	DBImmigration(ODBCInterface* pInterface);

	virtual BOOL	Load();							//移出前，先获得人物的帐号
	virtual	BOOL	Save(VOID* pResult);			//移入结束，将人物帐号的isvalid标志设置为1
	virtual BOOL	AddNew();						//移入前，先创建人物基本信息并填入创建时间
	virtual BOOL	ParseResult(VOID* pResult);

	BOOL			IsCharNameOK();					//判断库里是否有相同的人物名称
	BOOL			UpdateNewCharName();			//更新数据库里人物名称
public:
	const	CHAR*	GetAccount()	const;
	VOID			SetAccount(const CHAR*	pAccount);

	const	CHAR*	GetCharName()	const;
	VOID			SetCharName(const CHAR*	pName);

	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();

	VOID			SetCharCreateTime(UINT createTime);
	UINT			GetCharCreateTime();
};


#endif