/************************************************\
*												*
*												*
*				ʹ�ö��߳�����					*
*												*
*								by jinchao		*
*												*
\************************************************/
#pragma once

#include "AutoDownload.h"

namespace DOWNANT { class CDownManager; }

class CAutoDownload_DownAnt : public CAutoDownload
{
	/**********************************************************
		���нӿ�
	***********************************************************/
public:
	/*!
	\brief
		��������

	\return 
		Autodow����
	*/
	virtual AUTODOWN_TYPE getType(void) const { return AT_ANT; }

	/*!
	\brief
		ȡ�õ�ǰ״̬
	*/
	virtual AUTODOWN_STATE getState(void) const;

	/*!
	\brief	
		������������[�첽]

	\prama szDownURL
		���ص�ַ(http��ftpЭ��)

	\param szFileName
		���غ󱾵ش洢λ��
	
	\param hWnd
		���ع�������Ϣ֪ͨ���ڣ�֪ͨ��Ϣ���ֲ���AUTODOWN_WNDMSG

	\param hWnd
		����֪ͨ��Ϣʱ��OwnerID, ͨ��wParam���ظ�������

	\return
		��ʼ���Ƿ�ɹ�
	*/
	virtual bool init(const char* szDownURL, const char* szFileName, HWND hWnd=0, int nOwnerID=0);

	/*!
	\brief	
		���ù���״̬[ͬ��]
			ֹͣһ�й������ָ�����ʼ��״̬
	*/
	virtual void reset(void);

	/*!
	\brief	
		��ʼ����[�첽]
			���Ե���waitStopȷ�����ؽ���
	*/
	virtual void begin(void);

	/*!
	\brief	
		ǿ��ֹͣ����[�첽]
			���Ե���waitStopȷ����ȫֹͣ
	*/
	virtual void stop(void);

	/*!
	\brief	
		�ȴ���������
	\param	bBlock
		�Ƿ�ͬ��
	\return
		�Ƿ��Ѿ�������ֻ��bBlockΪtrueʱ��Ч
	*/
	virtual bool waitStop(bool bBlock);

	/*!
	\brief
		ȡ�õ�ǰ�������ص��ļ��Ĵ�С

	\param nFileSize[out]
		�ļ���С

	\param nReadSize[out]
		�Ѿ����ص��ļ���С

	\return
		�Ƿ�ȡ���ļ���С
	*/
	virtual bool getFileSize(__int64& nFileSize, __int64& nReadSize) const;

	/*!
	\brief
		ȡ�õ�ǰ�����ٶ������Ϣ
	
	\param pTimeSpend
		�Ѿ����ѵ�ʱ��(s)

	\param pTimeRemain
		����ʣ��ʱ��

	\return
		����ƽ���ٶ�(byte/s)
	*/
	virtual int getSpeedInfo(int* pTimeSpend=0, int* pTimeRemain=0) const;

	/**********************************************************
		�ڲ�����
	***********************************************************/
protected:
	DOWNANT::CDownManager* m_pDownManager;

	/*!
	\brief	
		Downant �ص�����
	*/
	static void WINAPI _downManStatusCallback(VOID* pOwner, DWORD dwCode, WPARAM wParam, LPARAM lParam);

	/**********************************************************
		����/����
	***********************************************************/
	friend CAutoDownload* CAutoDownload::createInstance(AUTODOWN_TYPE type);
	friend void CAutoDownload::destroyInsance(CAutoDownload* pDown);

protected:
	CAutoDownload_DownAnt(void);
	virtual ~CAutoDownload_DownAnt(void);
};