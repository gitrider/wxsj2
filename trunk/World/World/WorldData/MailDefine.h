
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����MailDefine.h
//�����ߣ�
//���������������ʼ�����ģ������ݽṹ
//�޸ļ�¼��2005-12-01 ����
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __MAILDEFINE_H__
#define __MAILDEFINE_H__

#include "Type.h"
#include "Chain.h"
#include "StrTable.h"

struct MailSMU;

//�ʼ��ڵ�
class MailNode : public CChainItem
{
public :
	MailNode( ) ;
	virtual ~MailNode( ) ;

	VOID			CleanUp( ) ;

	const MAIL*		GetMail( ){ return &m_Mail ; } ;
	VOID			SetMail( const MAIL* pMail ) ;
	VOID			ValidateShareMem();

public :
	MAIL			m_Mail ;
	MailSMU*		m_pMailSMU;
	INT				m_nPoolIndex ;
	INT				m_nUserNodePoolIndex ;//�ҽ��ڴ��û��ڵ�����
};

//�û��ڵ�
class UserNode : public CChainItem
{
public :
	UserNode( ) ;
	virtual ~UserNode( ) ;

	VOID			CleanUp( ) ;

	INT				GetMailCount( ) ;//���ش��û����ʼ�����
	INT				GetScriptMailCount( ) ;//���ش��û����ʼ�����


public :
	CChainHeader	m_ChainMail ;
	CChainHeader	m_ChainScriptMail ;
	INT				m_nPoolIndex ;
	UINT			m_uTickCount ;
	CHAR			m_szName[MAX_CHARACTER_NAME] ;

};

//�û�����
class UserChain
{
public :
	UserChain( ) ;
	~UserChain( ) ;

	BOOL			Init( ) ;
	VOID			CleanUp( ) ;

	INT				GetUserCount( ) ;//���ش��û����ʼ�����

	UserNode*		GetHeadNode( ) ;
	UserNode*		GetPrevNode( ) ;
	UserNode*		GetNextNode( ) ;
	UserNode*		GetTailNode( ) ;

public :
	CChainHeader	m_ChainUser ;

};

//�û�����(GUID)���û��ڵ��Hash��
class UserHashTable
{
public :
	UserHashTable( ) ;
	~UserHashTable( ) ;

	VOID			CleanUp( ) ;
	BOOL			Init( ) ;

	BOOL			Add( CHAR* szName, UserNode* pNode ) ;
	VOID			Del( CHAR* szName ) ;
	UserNode*		Get( CHAR* szName ) ;

protected :
	StrTable		m_NameTable ;

};




#endif
