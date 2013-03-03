/*
�Լ�¼���в����Ĺ����࣬�Լ��������κ���Ϣֻ��һЩ�����ļ���
*/

#ifndef _RECORD_OPT_H_
#define _RECORD_OPT_H_

#include "Type.h"
#include "Record.h"	
enum OPT_RECORD
{
	REC_EXCHANGEITEM = 0,			//���׼�¼������Ʒ
	REC_EXCHANGEPET,				//���׼�¼�������
	REC_ADDITEM,					//�ϻ�
	REC_DELITEM,					//�»�
	REC_ADDPET,						//�ϻ�
	REC_DELPET,						//�»�
	REC_ONSALEITEM,					//�ϼ�
	REC_OFFSALEITEM,				//�¼�
	REC_ONSALEPET,					//�ϼ�
	REC_OFFSALEPET,					//�¼�
	REC_OPEN,						//����
	REC_CLOSE,						//����
	REC_INPUT_BASE,					//���뱾��
	REC_INPUT_PROFIT,				//����ӯ����



	MAX_RECORD,
};

namespace RecordOpt
{
	//-------------------------------------------------------------------------------------------------
	//��ʼ����������
	BOOL		InitRecordContainer(RecordContainer_t* pContainer, INT iSize);

	//-------------------------------------------------------------------------------------------------
	//��õ�ǰ��¼�����еļ�¼����
	INT			GetRecordNum(RecordContainer_t* pContainer);

	//-------------------------------------------------------------------------------------------------
	//��������
	BOOL		Excute( OPT_RECORD Opt, RecordContainer_t* pContainer, ... );

	//-------------------------------------------------------------------------------------------------
	//ö�������е���Ŀ
	VOID		BeginEnumEntry(RecordContainer_t* pContainer);
	CHAR*		EnumEntry( RecordContainer_t* pContainer );
	VOID		EndEnumEntry(RecordContainer_t* pContainer);
};

#endif