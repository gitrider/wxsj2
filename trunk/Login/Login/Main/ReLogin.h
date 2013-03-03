/********************************************************************
	created:	2007-12-12   11:09
	filename: 	Login\Login\Main\ReLogin.h
	author:		YangJun
	
	purpose:	��ҪΪ������ͬһ�ʺ�ֻ�ܼ��20���¼
				��ҪΪ������ͬһ�ʺ�ÿ��Сʱֻ�ܵ�¼20��
*********************************************************************/

#ifndef _RELOGIN_H_
#define _RELOGIN_H_

#include "Type.h"
#include "StrTable.h"

#define RELOGIN_CLEAR_INTERVAL			2000		//ÿ2���Ӽ��

#define MIN_RELOGIN_TIME_GAP			20000		//ÿ20�������֤һ������
#define RESET_RELOGIN_COUNT_TIME		3600000		//ÿСʱ���һ���ʺŵĿ���֤����
#define MAX_RELOGIN_COUNT_PERHOUR		30			//ÿСʱ������֤���������
#define MAX_RELOGIN_ACCNUM				51200		//���������5����ʺ���

typedef struct tag_ReLoginUnit
{
	UINT		m_LoginTime;			//�ϴ���֤�����ʱ��
	INT			m_LoginCount;			//ʣ��Ŀ���֤����

	tag_ReLoginUnit();
	VOID		CleanUp();
}ReLoginUnit;

class ReLoginManager
{
public:
	ReLoginManager(){}
	virtual ~ReLoginManager(){}

	BOOL		Init();
	BOOL		IsAccCanReLogin(const CHAR* szAccName);

	BOOL		HeartBeat();
private:
	StrTable	m_LoginAccTable;

	ReLoginUnit	m_LoginUnit[MAX_RELOGIN_ACCNUM];
	INT			m_CurPos;

	CMyTimer	m_CheckOldTimer;			//��ʱ�Ƴ��Ѿ�����20��ĵ�¼��ɫ
	CMyTimer	m_RestCountTimer;			//��ʱ���ÿ�����֤���������
};

extern ReLoginManager*					g_pReLoginManager;

#endif