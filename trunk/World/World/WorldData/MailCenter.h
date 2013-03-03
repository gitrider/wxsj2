
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����MailCenter.h
//�����ߣ�
//���������������������ʼ�����
//�޸ļ�¼��2005-11-30 ����
//���ݽṹ˵����
//
// UserNode->MailNode->MailNode->MailNode......			MAIL_Pool ->MailNode[0]
// |         |_MAIL	   |_MAIL    |_MAIL								MailNode[1]
// |																MailNode[2]
// UserNode->MailNode->MailNode->MailNode......						...
// |         |_MAIL	   |_MAIL    |_MAIL
// |
// UserChain
//
// UserHashTable->Name(GUID) to UserNode[?]
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __MAILCENTER_H__
#define __MAILCENTER_H__

#include "Type.h"
#include "MailDefine.h"


class USER ;

class MailCenter
{
public :
	MailCenter( ) ;
	~MailCenter( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	BOOL				HeartBeat( UINT uTime=0 ) ;


public :
//�ʼ�����ӿ�
	
	//ϵͳ����ʱע���ʼ��ڵ�
	UINT				RegisterMailNode(MailNode* pMailNode);

	//����mail��Ŀ�꣨Ŀ����Ϣ��mail�����������ֵΪ�û���ǰ�ú���ʼ�����
	UINT				MailTo( const MAIL* pMail ) ;
	//�������ֽ�szName��ҵ��ʼ���Ϣ��pList�����ʼ��б�, ����ֵΪ�ʼ�����
	UINT				AskMail( const CHAR* szName, MAIL_LIST* pListOut ) ;
	//������ֽ�szName����Ƿ����ʼ�
	UINT				CheckMail( const CHAR* szName ) ;
	//�������ֽ�szName��ҵ��ʼ���Ϣ��pList�����ʼ��б�, ����ֵΪ�ʼ�����
	UINT				AskScriptMail( const CHAR* szName, MAIL_LIST* pListOut ) ;

public :
//�ʼ��¼���Ӧ�ӿ�
	//��ҽ��յ����ʼ�
	VOID				OnUserRecvMail( const MAIL_LIST* pList ) ;
	//��ҷ������ʼ�
	VOID				OnUserSendMail( const MAIL* pMail ) ;

	// ������ͨ�ʼ�
	VOID				SendNormalMail(const CHAR* szReceiverName, const CHAR* szContent);

	// ���Ϳ�ִ���ʼ�
	VOID				SendScriptMail(const CHAR* szReceiverName, UINT uParam0 = 0, UINT uParam1 = 0, UINT uParam2 = 0, UINT uParam3 = 0 );

protected :
//�ڲ����ܽӿ�
	//UINT				MailToOnlineUser( MAIL* pMail, USER* pSender, USER* pRecver ) ;
	//UINT				MailToOfflineUser( MAIL* pMail, USER* pSender, USER* pRecver ) ;

	// �� SendNormalMail��SendScriptMail ����
	VOID				SendMail(const MAIL* pMail);

protected :
	UserHashTable		m_UserHashTable ;
	UserChain			m_UserChain ;
	CMyTimer			m_LogicTime ;

};

extern MailCenter*	g_pMailCenter ;


#endif
