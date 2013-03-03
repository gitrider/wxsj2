/********************************************************************
	created:	2007-12-12   11:09
	filename: 	Login\Login\Main\ReLogin.h
	author:		YangJun
	
	purpose:	主要为了限制同一帐号只能间隔20秒登录
				主要为了限制同一帐号每个小时只能登录20次
*********************************************************************/

#ifndef _RELOGIN_H_
#define _RELOGIN_H_

#include "Type.h"
#include "StrTable.h"

#define RELOGIN_CLEAR_INTERVAL			2000		//每2秒钟检测

#define MIN_RELOGIN_TIME_GAP			20000		//每20秒可以验证一次密码
#define RESET_RELOGIN_COUNT_TIME		3600000		//每小时清除一次帐号的可验证次数
#define MAX_RELOGIN_COUNT_PERHOUR		30			//每小时可以验证的密码次数
#define MAX_RELOGIN_ACCNUM				51200		//保留最近的5万个帐号名

typedef struct tag_ReLoginUnit
{
	UINT		m_LoginTime;			//上次验证密码的时间
	INT			m_LoginCount;			//剩余的可验证次数

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

	CMyTimer	m_CheckOldTimer;			//定时移除已经超过20秒的登录角色
	CMyTimer	m_RestCountTimer;			//定时重置可以验证的密码次数
};

extern ReLoginManager*					g_pReLoginManager;

#endif