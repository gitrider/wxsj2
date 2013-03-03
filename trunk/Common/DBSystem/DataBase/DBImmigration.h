#ifndef _DB_IMMIGRATION_H_
#define _DB_IMMIGRATION_H_

#include "Type.h"
#include "ODBCBase.h"

class DBImmigration:public ODBCBase
{
	CHAR			m_Account[MAX_ACCOUNT+1];		//�û�����
	GUID_t			m_CharGuid;						//�û�GUID
	UINT			m_CreateTime;					//��ɫ����ʱ��
	CHAR			m_CharName[MAX_CHARACTER_NAME+1];	//��ɫ����
public:
	DBImmigration(ODBCInterface* pInterface);

	virtual BOOL	Load();							//�Ƴ�ǰ���Ȼ��������ʺ�
	virtual	BOOL	Save(VOID* pResult);			//����������������ʺŵ�isvalid��־����Ϊ1
	virtual BOOL	AddNew();						//����ǰ���ȴ������������Ϣ�����봴��ʱ��
	virtual BOOL	ParseResult(VOID* pResult);

	BOOL			IsCharNameOK();					//�жϿ����Ƿ�����ͬ����������
	BOOL			UpdateNewCharName();			//�������ݿ�����������
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