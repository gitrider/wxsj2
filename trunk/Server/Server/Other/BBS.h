/*
���԰�������˽ṹ
*/

#ifndef _BBS_H_
#define _BBS_H_

#include "Type.h"
class Obj_Human;

/*
�������Խṹ
*/
struct MessageEntry_t
{
	CHAR	szAuthorName[MAX_BBS_MESSAGE_AUTHORLENGTH];				//����������
	UINT	nID;											//����ID
	BYTE	nHour;											//����ʱ�䣨Сʱ��
	BYTE	nMin;											//����ʱ�䣨���ӣ�
	BYTE	bHasReply;										//�Ƿ��лظ�
	UINT	nMsgLength;										//���ӳ���
	CHAR	szMessage[MAX_BBS_MESSAGE_LENGTH];				//��������
	BYTE	nReHour;										//�ظ�ʱ�䣨Сʱ��
	BYTE	nReMin;											//�ظ�ʱ�䣨���ӣ�
	UINT	nReplyMsgLength;								//�ظ�����
	CHAR	szReplyMessage[MAX_BBS_MESSAGE_LENGTH];			//�ظ�����


	MessageEntry_t()
	{
		memset(szAuthorName, 0, MAX_BBS_MESSAGE_AUTHORLENGTH);
		nID			=	0;
		nHour		=	0;
		nMin		=	0;
		bHasReply	=	0;
		nMsgLength	=	0;
		memset(szMessage, 0, MAX_BBS_MESSAGE_LENGTH);
		nReHour		=	0;
		nReMin		=	0;
		nReplyMsgLength	=	0;
		memset(szReplyMessage, 0, MAX_BBS_MESSAGE_LENGTH);
	}
};

/*
���԰�ṹ
*/
class ServerBBS
{
public:
	ServerBBS();
	~ServerBBS();

	BOOL			Init( ) ;
	VOID			CleanUp( ) ;

	/*
	Message
	*/
	BOOL			AddNewMessageByID(UINT nID, CHAR*	szMessage, INT nLength, const CHAR* szAuthorName);
	BOOL			ReplyMessageByID(UINT nID, CHAR*	szReplyMessage, INT	nReplyLength);
	BOOL			DeleteMessageByID(UINT nID);
	MessageEntry_t*	GetMessageByIndex(UINT nIndex);

	/*
	ID
	*/
	UINT			NewMessageID()	
	{
		FinalID++;
		if(FinalID == 0)
			return ++FinalID;
		else
			return FinalID;
	}

	/*
	title
	*/
	BOOL			SetBBSTitle(CHAR*	szMessage, INT nLength)
	{
		memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);
		m_nTitleLength	=	nLength;
		memcpy(m_szTitle, szMessage, nLength);
		IncSerial();
		return TRUE;
	}
	CHAR*			GetBBSTitle(INT& nLength)
	{
		nLength	=	m_nTitleLength;
		return	m_szTitle;
	}

	/*
	TempBuff
	*/
	VOID			SaveTitle()
	{
		SetTempBuff(m_szTitle, m_nTitleLength);
	}

	VOID			LoadTitle()
	{
		INT titleLength = 0;
		CHAR*	pszTitle = GetTempBuff(titleLength);
		if(titleLength!=0)
			SetBBSTitle(pszTitle, titleLength);
		/*else
			memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);*/
	}

	/*
	Serial change
	*/
	UINT			GetSerial() {return m_Serial;}
	VOID			IncSerial()
	{
		m_Serial += m_SerialIncStep;
	}
	VOID			SetSerialIncStep(UINT SerialIncStep) {m_SerialIncStep = SerialIncStep;}
	UINT			GetSerialIncStep() {return m_SerialIncStep;}

	UINT			GetFinalIndex()const{return m_FinalIndex;}

private:
	BOOL			AddNewMessage(MessageEntry_t* pMessage);
	INT				GetMessageIndexByID(UINT nID);
	MessageEntry_t*	GetMessageByID(UINT nID);

	BOOL			SetTempBuff(CHAR*	szMessage, INT nLength)
	{
		if(nLength<MAX_BBS_MESSAGE_LENGTH)
		{
			memset(m_szTempBuffer, 0, MAX_BBS_MESSAGE_LENGTH);
			memcpy(m_szTempBuffer, szMessage, nLength);
		}
		return TRUE;
	}
	CHAR*			GetTempBuff(INT& nLength)
	{
		nLength	=	(INT)strlen(m_szTempBuffer);
		return	m_szTempBuffer;
	}


private:
	UINT			m_SerialIncStep;
	UINT			FinalID;
    UINT			m_Serial;
	UINT			m_nTitleLength;
	BYTE			m_FinalIndex;
	CHAR			m_szTitle[MAX_BBS_MESSAGE_LENGTH];
	BYTE			m_IsValid[MAX_BBS_MESSAGE_NUM];
	MessageEntry_t	m_MessageData[MAX_BBS_MESSAGE_NUM];			
	CHAR			m_szTempBuffer[MAX_BBS_MESSAGE_LENGTH];//�洢��ʱ�����̯λ�����
};

#endif