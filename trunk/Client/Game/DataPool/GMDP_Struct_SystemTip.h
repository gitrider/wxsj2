
/**	GMDP_Struct_SystemTip.h
 *
 *	���ܣ�	ϵͳ��ʾ��ص�����
 *
 *	�޸ļ�¼��	
 *					
 */

#pragma once


#include "Type.h"



// �¼�����
enum SYSTIP_TYPE
{
	STT_INVALID	= -1,	// ��Ч

	STT_LEVELUP,		// ����
	STT_NEWTIP,			// ������ʾ
	STT_SKILL,			// ����ѧϰ
	STT_TEAMINVITE,		// ��������
	STT_EXCHANGEINVITE,	// ��������
	STT_FAMILYINVITE,	// ��������
	STT_GUILDINVITE,	// �������
	STT_SYSTEMTIP,		// ϵͳ��ʾ
	STT_MISSTIP,		// ������ʾ
	STT_FRIENDCHAT,		// ����������Ϣ
	STT_EQUIPTIP,		// �;���ʾ������10��5 �����0��

	STT_NUM,			// ����
};

// ÿ���¼�������
struct SystemTipEvent
{
	SYSTIP_TYPE	m_eType;		// �¼�������
	INT			m_nIndex;		// �ڽ����е�����
	INT			m_nLevelIndex;	// ������ڶ���¼���ͬһͼ���ϣ� ��ǰ���¼����ڵĲ㼶
	STRING		m_strTipDesc;	// ��ʾ����ʾ��Ϣ
	BOOL		m_bDispose;		// �Ƿ��Ѵ��� �����˾Ͳ�����ʾ
	
	INT			m_nParameter;	// ������ �����ʹ��


	SystemTipEvent()
	{
		m_eType			= STT_INVALID;
		m_nIndex		= -1;
		m_nLevelIndex	= -1;
		m_strTipDesc	= "";
		m_bDispose		= FALSE;

		m_nParameter	= -1;
	};
};


