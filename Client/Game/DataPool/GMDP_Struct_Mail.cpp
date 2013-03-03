
/** GMDP_Struct_Mail.cpp
 *	
 *	功能：	
 *
 *	修改记录：
 *			080410	修改新邮件的通知方式
 *			
 */

#include "StdAfx.h"
#include "GMDP_Struct_Mail.h"
#include "../event/gmeventsystem.h"
#include "GMDataPool.h"



VOID SMail::CleanUp()
{
	m_uFlag = 0;
	memset( (void*)m_szCreateTime, 0, sizeof(m_szCreateTime) );
	memset( (void*)m_szSourName, 0, sizeof(m_szSourName) );
	m_nPortrait = -1;
	memset( (void*)m_szDestName, 0, sizeof(m_szDestName) );
	memset( (void*)m_szContex, 0, sizeof(m_szContex) );
}

CMailPool::CMailPool()
{
	m_nMailLeft = 0;
}

CMailPool::~CMailPool()
{
	INT nCount;
	
	nCount = (INT)m_MailPool.size();
	if ( nCount > 0 )
	{
		for( INT i = 0; i < nCount; ++i )
		{
			SAFE_DELETE(m_MailPool[i]);
		}

		m_MailPool.clear();
	}
}

// 得到邮件池中的邮件数量，好像没有多大意义
INT CMailPool::GetMailCount()
{
	return (INT)m_MailPool.size();
}

// 得到剩余未收的邮件数量
INT CMailPool::GetMailLeftCount()
{
	return m_nMailLeft;
}

// 设置剩余未收的邮件数量
VOID CMailPool::SetMailLeftCount( INT nCount )
{
	if ( nCount < 0 )
	{
		Assert( nCount >= 0 );
		return;
	}

	m_nMailLeft = nCount;
}

// 加入一封邮件，并将加入结果的在数据池中的索引返回，如果加入失败，则返回 -1
INT CMailPool::AddMail( const MAIL* pMail )
{
	if ( pMail == NULL )
	{
		Assert( pMail == NULL );
		return -1;
	}

	SMail* pSMail = new SMail;

	pSMail->m_uFlag = pMail->m_uFlag;

	struct tm* ttm;
	ttm = localtime( &(pMail->m_uCreateTime) );
	if ( ttm != NULL )
	{
		strftime(pSMail->m_szCreateTime, sizeof(pSMail->m_szCreateTime), "%a %y/%m/%d %H:%M:%S", ttm );
	}

	pSMail->m_SourGUID = pMail->m_GUID;
	strncpy( pSMail->m_szSourName, pMail->m_szSourName, pMail->m_SourSize );
	pSMail->m_nPortrait = pMail->m_nPortrait;
	strncpy( pSMail->m_szDestName, pMail->m_szDestName, pMail->m_DestSize );
	strncpy( pSMail->m_szContex, pMail->m_szContex, pMail->m_ContexSize );

	m_MailPool.push_back( pSMail );

	return ((INT)(m_MailPool.size() - 1));
}

// 根据编号取得邮件
const SMail* CMailPool::GetMail( UINT uIndex )
{
	return m_MailPool.at( uIndex );
}

// 通知 UI 有未收邮件
VOID CMailPool::InformUI()
{
	CDataPool::GetMe()->SystemTip_AddEventInfo( STT_SYSTEMTIP, "有新邮件。" );
	// 更新界面显示
	CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );


	// 如果界面打开了， 更新
	CEventSystem::GetMe()->PushEvent( GE_HAVE_MAIL );
}

// 让 UI 显示最后一封邮件
VOID CMailPool::ShowLastMail()
{
	// 就是把最后一封邮件的位置用事件通知 UI
	// m_MailPool.size() - 1
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_EMAIL, GetMailCount() - 1 );
	CEventSystem::GetMe()->PushEvent( GE_HAVE_MAIL );
}
