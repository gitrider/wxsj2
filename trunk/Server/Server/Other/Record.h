/*
������¼��������¼���е��û��������û���ѯ��
�������κνṹ��ֻ��һ���򵥵Ĵ洢�ṹ��������RecordOpt����
*/

#ifndef _RECORD_H_
#define _RECORD_H_

//-------------------------------------------------------------------------------------------------
//ÿ����¼
struct RecordEntry_t
{
	CHAR	m_szRecord[MAX_RECORD_LEN_ENTRY];
	INT		m_iTime;
	RecordEntry_t()
	{
		CleanUp();
	}
	VOID	CleanUp()
	{
		memset(m_szRecord, 0, MAX_RECORD_LEN_ENTRY);
		m_iTime = 0;
	}
};

//-------------------------------------------------------------------------------------------------
//��¼��������
struct	RecordContainer_t
{
	RecordEntry_t	m_szRecordContent[MAX_RECORD_NUM_ENTRY];	
	INT				m_iMaxSize;
	INT				m_iCurEntry;
	INT				m_iOldestEntry;
	INT				m_iTempEntry;

	VOID	CleanUp()
	{
		memset(this, 0, sizeof(RecordContainer_t));
	}
};
#endif