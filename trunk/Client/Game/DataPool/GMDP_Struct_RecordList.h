
#pragma once

#include "Type.h"
//------------
// ���׼�¼�����߲�����¼
//------------
//�������İ���ְλ
struct RecordEntry_t
{
	STRING	szContent; 
	RecordEntry_t()
	{
		szContent = "";
	}
};

struct RecordList_t
{
	INT								m_nRecordNum;
	std::vector< RecordEntry_t >	m_RecordEntryList;

	RecordList_t()
	{
		CleanUp();
	}
	VOID	CleanUp()
	{
		m_nRecordNum = 0;
		RecordEntry_t	Recordinital;
		m_RecordEntryList.resize(MAX_RECORD_NUM_PER_PAGE, Recordinital);			//���׼�¼�����߲�����¼
	}
};