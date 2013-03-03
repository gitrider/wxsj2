//-----------------------------------------------------------------------------
// �ļ��� : MisBuf.h
// ģ��	:	Script
// ����	 :	����ϵͳ�õĳ��������ڵ����ݿ�
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------
#ifndef __MISBUF__H_
#define __MISBUF__H_


typedef struct _MisBuf
{
	enum 
	{
		MISSIONBUFLEN = 32,
		MISSIONCHARBUFLEN = MISSIONBUFLEN * 4 * 8,//��Ϊ�ͻ��˵�bug,���ԷŴ�buf����
	};

	typedef struct _MisItemBuf
	{
		//���buf�洢�˷��͸��ͻ��˵��ַ���Ϣ��,�ͻ�����Ҫע���ַ���������
		CHAR buf[MISSIONCHARBUFLEN];
		enum BufType
		{
			BufType_Int,
			BufType_Str,
			BufType_IntStr,
			BufType_Money,
			BufType_Item,
			BufType_RandItem,
			BufType_RadioItem,
		}mType;	
	}MisItemBuf[MISSIONBUFLEN];

	MisItemBuf Buff;
	INT mUseNum;

	VOID ResetMisBuf()
	{
		//��Ϊ����Q_strncyz,���Բ�������0
		//memset(&Buff,0,sizeof(Buff));

		mUseNum = 0;
	}
}MisBuf;

#endif