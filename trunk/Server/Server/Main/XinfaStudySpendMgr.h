//
// �ķ�ѧϰ�����ģ���Ǯ�;���ֵ���Ĺ���
// ��¼�ķ�ÿ������ѧϰ��ʱ����Ҫ���ʵĽ�Ǯ�;���
// ���������ķ���������������ͬ��
//--------------------------------------------------
#ifndef __XINFASTUDYSPENDMGR_H__
#define __XINFASTUDYSPENDMGR_H__

#include "Type.h"
#include "GameDefine.h"

class XinfaStudySpendMgr
{
public :
	struct StudySpend
	{
		UINT			m_uMoney;
		UINT			m_uExperience;

		StudySpend()
		{
			m_uMoney			= 0;
			m_uExperience		= 0;
		};
	};

	XinfaStudySpendMgr();
	~XinfaStudySpendMgr();

	StudySpend		GetStudySpend(INT nXinfaLevel){
		return m_pXinfaLevel[nXinfaLevel];
	}

	BOOL			Init();

private :
	INT				m_nMaxLevel;
	StudySpend		m_pXinfaLevel[MAX_XINFA_LEVEL_NUM + 1];
	
};

extern XinfaStudySpendMgr* g_pXinfaStudySpendMgr;

#endif
