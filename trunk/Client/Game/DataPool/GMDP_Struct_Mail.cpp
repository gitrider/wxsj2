
/** GMDP_Struct_Mail.cpp
 *	
 *	���ܣ�	
 *
 *	�޸ļ�¼��
 *			080410	�޸����ʼ���֪ͨ��ʽ
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

// �õ��ʼ����е��ʼ�����������û�ж������
INT CMailPool::GetMailCount()
{
	return (INT)m_MailPool.size();
}

// �õ�ʣ��δ�յ��ʼ�����
INT CMailPool::GetMailLeftCount()
{
	return m_nMailLeft;
}

// ����ʣ��δ�յ��ʼ�����
VOID CMailPool::SetMailLeftCount( INT nCount )
{
	if ( nCount < 0 )
	{
		Assert( nCount >= 0 );
		return;
	}

	m_nMailLeft = nCount;
}

// ����һ���ʼ��������������������ݳ��е��������أ��������ʧ�ܣ��򷵻� -1
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

// ���ݱ��ȡ���ʼ�
const SMail* CMailPool::GetMail( UINT uIndex )
{
	return m_MailPool.at( uIndex );
}

// ֪ͨ UI ��δ���ʼ�
VOID CMailPool::InformUI()
{
	CDataPool::GetMe()->SystemTip_AddEventInfo( STT_SYSTEMTIP, "�����ʼ���" );
	// ���½�����ʾ
	CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );


	// ���������ˣ� ����
	CEventSystem::GetMe()->PushEvent( GE_HAVE_MAIL );
}

// �� UI ��ʾ���һ���ʼ�
VOID CMailPool::ShowLastMail()
{
	// ���ǰ����һ���ʼ���λ�����¼�֪ͨ UI
	// m_MailPool.size() - 1
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_EMAIL, GetMailCount() - 1 );
	CEventSystem::GetMe()->PushEvent( GE_HAVE_MAIL );
}
