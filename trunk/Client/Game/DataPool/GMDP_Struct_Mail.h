
/** �ʼ��ͻ������ݳ�
 */

#pragma once


#include "Type.h"
#include "GameStruct.h"

#define TIME_STRING_LENGTH				24


struct SMail
{
	UINT								m_uFlag;								// �ʼ���־ enum MAIL_TYPE
	GUID_t								m_SourGUID;								// �����˵� GUID
	CHAR								m_szCreateTime[TIME_STRING_LENGTH];		// �ʼ�����ʱ��
	CHAR								m_szSourName[MAX_CHARACTER_NAME];		// ������
	INT									m_nPortrait;							// ������ͷ��
	CHAR								m_szDestName[MAX_CHARACTER_NAME];		// ������
	CHAR								m_szContex[MAX_MAIL_CONTEX];			// ����

	SMail()								{ CleanUp(); }
	VOID								CleanUp();
};
typedef std::vector< SMail* >			SMailList;

class CMailPool
{
public:
	CMailPool();
	~CMailPool();

	// �õ��ʼ����е��ʼ�����������û�ж������
	INT									GetMailCount();

	// �õ�ʣ��δ�յ��ʼ�����
	INT									GetMailLeftCount();

	// ����ʣ��δ�յ��ʼ�����
	VOID								SetMailLeftCount( INT nCount );

	// ����һ���ʼ��������������������ݳ��е��������أ��������ʧ�ܣ��򷵻� -1
	INT									AddMail( const MAIL* pMail );

	// ���ݱ��ȡ���ʼ�
	const SMail*						GetMail( UINT uIndex );

	// ֪ͨ UI ��δ���ʼ�
	VOID								InformUI();

	// �� UI ��ʾ���һ���ʼ�
	VOID								ShowLastMail();

private:
	INT									m_nMailLeft;							// ���ж��ٷ�û��������
	SMailList							m_MailPool;								// �ʼ���
};
