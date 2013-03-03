
#include "stdafx.h"
#include "RecordOpt.h"
#include "TimeManager.h"//g_pTimeManager
#include "Log.h"
using namespace RecordOpt;
//-------------------------------------------------------------------------------------------------
//��ʼ����������
BOOL RecordOpt::InitRecordContainer(RecordContainer_t* pContainer, INT iSize)
{
	if(iSize>MAX_RECORD_NUM_ENTRY)
		return FALSE;
	pContainer->CleanUp();
	pContainer->m_iMaxSize		= iSize;
	pContainer->m_iCurEntry		= 0;
	pContainer->m_iOldestEntry	= 0;
	pContainer->m_iTempEntry	= 0;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
//��õ�ǰ��¼�����еļ�¼����
INT RecordOpt::GetRecordNum(RecordContainer_t* pContainer)
{
	if(pContainer->m_iCurEntry==(pContainer->m_iMaxSize - 1) && pContainer->m_iOldestEntry == 0)
	{
		return pContainer->m_iMaxSize;
	}

	if(pContainer->m_iCurEntry>=pContainer->m_iOldestEntry)
	{
		return pContainer->m_iCurEntry - pContainer->m_iOldestEntry;
	}
	else
	{
		return pContainer->m_iMaxSize;
	}
}

//-------------------------------------------------------------------------------------------------
//��������
/*
��������Ϊ��ʽ��
�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
ȡ�أ�AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
�¼ܣ�AAA�¼���BBBC����ԭ���Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
���ţ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
���ȣ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
���뱾��AAA�����̱����г��루���У���������ͭ��AAAΪ����������
����Ӯ���ʽ�AAA������Ӯ���ʽ��г��루���У���������ͭ��AAAΪ����������

*/
BOOL RecordOpt::Excute( OPT_RECORD Opt, RecordContainer_t* pContainer, ... )
{
	__ENTER_FUNCTION
	if(Opt<0||Opt>=MAX_RECORD)
		return FALSE;
	if(pContainer == NULL)
		return FALSE;

    va_list ParamList;
	va_start(ParamList, pContainer);

	pContainer->m_szRecordContent[pContainer->m_iCurEntry].CleanUp();
	g_pTimeManager->SetTime();
	UINT	iCurTime = pContainer->m_szRecordContent[pContainer->m_iCurEntry].m_iTime = g_pTimeManager->Time2DWORD();
	CHAR*	szInputBuffer = static_cast<CHAR*>(pContainer->m_szRecordContent[pContainer->m_iCurEntry].m_szRecord);
	
	switch(Opt)
	{
	case REC_EXCHANGEITEM:
		{
			//�۳�AA����ã�������ͭ����AAΪ��Ʒ����
			INT iItemType = va_arg( ParamList, INT);
			INT iItemNum  = va_arg( ParamList, INT);
			INT	iCost	  = va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: �۳�[#{_ITEM%d}]X%d, ���#{_MONEY%d}", iCurTime, iItemType, iItemNum, iCost);
		}
		break;
	case REC_EXCHANGEPET:
		{
			//�۳�AA����ã�������ͭ����AAΪ��Ʒ����
			CHAR*	pszPetName	= va_arg( ParamList, CHAR*);
			INT		iItemNum	= va_arg( ParamList, INT);
			INT		iCost		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: �۳�[%s]X%d, ���#{_MONEY%d}", iCurTime, pszPetName, iItemNum, iCost);
		}
		break;
	case REC_ADDITEM:
		{
			//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϻ���[#{_ITEM%d}]X%d��", iCurTime, pszOperatorName, iItemType, iItemNum);
		}
		break;
	case REC_DELITEM:
		{
			//ȡ�أ�AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %sȡ����[#{_ITEM%d}]X%d��", iCurTime, pszOperatorName, iItemType, iItemNum);
		}
		break;
	case REC_ADDPET:
		{
			//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϻ���[%s]X%d��", iCurTime, pszOperatorName, pszPetName, iItemNum);
		}
		break;
	case REC_DELPET:
		{
			//ȡ�أ�AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %sȡ����[%s]X%d��", iCurTime, pszOperatorName, pszPetName, iItemNum);
		}
		break;
	case REC_ONSALEITEM:
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϼ���[#{_ITEM%d}]X%d��,���Ϊ#r #{_MONEY%d}", iCurTime, pszOperatorName, iItemType, iItemNum, iPrice);
		}
		break;
	case REC_OFFSALEITEM:
		{
			//�¼ܣ�AAA�¼���BBBC����ԭ���Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϼ���[#{_ITEM%d}]X%d��,ԭ���Ϊ#r #{_MONEY%d}", iCurTime, pszOperatorName, iItemType, iItemNum, iPrice);
		}
		break;
	case REC_ONSALEPET:
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϼ���[%s]X%d��,���Ϊ#r #{_MONEY%d}", iCurTime, pszOperatorName, pszPetName, iItemNum, iPrice);
		}
		break;
	case REC_OFFSALEPET:
		{
			//�¼ܣ�AAA�¼���BBBC����ԭ���Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�ϼ���[%s]X%d��,ԭ���Ϊ#r #{_MONEY%d}", iCurTime, pszOperatorName, pszPetName, iItemNum, iPrice);
		}
		break;
	case REC_OPEN:
		{
			//���ţ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iStallIndex		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�����˵�%d���̨", iCurTime, pszOperatorName, iStallIndex);
		}
		break;
	case REC_CLOSE:
		{
			//���ȣ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iStallIndex		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�����˵�%d���̨", iCurTime, pszOperatorName, iStallIndex);
		}
		break;
	case REC_INPUT_BASE:
		{
			//���뱾��AAA�����̱����г��루���У���������ͭ��AAAΪ����������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iMoney			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s�����̱����г���#r #{_MONEY%d}", iCurTime, pszOperatorName, iMoney);
		}
		break;
	case REC_INPUT_PROFIT:
		{
			//����Ӯ���ʽ�AAA������Ӯ���ʽ��г��루���У���������ͭ��AAAΪ����������
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iMoney			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s������Ӯ���ʽ��г���#r #{_MONEY%d}", iCurTime, pszOperatorName, iMoney);
		}
		break;

	default:
		{
			va_end( ParamList );  
			g_pLog->FastSaveLog( LOG_FILE_1, "��¼����" ) ;
			return FALSE;
		}
		break;
	}

	pContainer->m_iCurEntry++;
	if(pContainer->m_iCurEntry == pContainer->m_iMaxSize)
	{
		pContainer->m_iCurEntry = 0;
	}
	if(pContainer->m_iCurEntry == pContainer->m_iOldestEntry)
	{
		pContainer->m_iOldestEntry++;
		if(pContainer->m_iOldestEntry == pContainer->m_iMaxSize)
		{
			pContainer->m_iOldestEntry = 0;
		}
	}
	va_end( ParamList );  
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

//-------------------------------------------------------------------------------------------------
//ö�������е���Ŀ
VOID RecordOpt::BeginEnumEntry(RecordContainer_t* pContainer)
{
	pContainer->m_iTempEntry = pContainer->m_iOldestEntry;
}

CHAR* RecordOpt::EnumEntry( RecordContainer_t* pContainer )
{
	__ENTER_FUNCTION

	if(pContainer->m_iTempEntry == pContainer->m_iCurEntry)
	{
		return NULL;
	}

	INT iReturnIndex = pContainer->m_iTempEntry++;
	if(pContainer->m_iTempEntry == pContainer->m_iMaxSize)
	{
		pContainer->m_iTempEntry = 0;
	}
	return pContainer->m_szRecordContent[iReturnIndex].m_szRecord;

	__LEAVE_FUNCTION
	return NULL;

}

VOID RecordOpt::EndEnumEntry(RecordContainer_t* pContainer)
{
	pContainer->m_iTempEntry = pContainer->m_iCurEntry;
}
