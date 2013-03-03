/************************************************\
*												*
*												*
*					AtuoDownload				*
*			----------------------------		*
*	    		0. ʹ��Sogou p2p��������		*
*				1. ʹ�ö��߳�����				*
*												*
*								by jinchao		*
*												*
\************************************************/

#pragma once

#include <windows.h>
#include <string>

class CAutoDownload
{
	/**********************************************************
		��������
	***********************************************************/
public:
	/*!
	\brief
		��̨����
	*/
	enum AUTODOWN_TYPE
	{
		AT_ANT,		//!< ��ͨ���߳�����
		AT_SODA,	//!< Sogou toolbar �ṩ��p2p���ط���
	};

	/*!
	\brief
		����״̬�����
	*/
	enum AUTODOWN_STATE_CATEGORY
	{
		AC_IDLE				= 0,			//!< ����
		AC_BUSY				= 1,			//!< ����
		AC_FINISH_SUCCESS	= 2,			//!< �ɹ�����
		AC_FINISH_FAILED	= 3,			//!< ʧ�ܽ���
	};

	/*!
	\brief
		����״̬
	*/
	enum AUTODOWN_STATE
	{
		AS_IDLE_NOTINIT			= 0x00000000,			//!< ����
		AS_IDLE_INIT_SUCCESS	= 0x00000001,			//!< ����_��ʼ���ɹ�
		AS_IDLE_INIT_FAILED		= 0x00000002,			//!< ����_��ʼ��ʧ��

		AS_BUSY_QUERY			= 0x00010000,			//!< ����_��ѯ�ļ���Ϣ
		AS_BUSY_DOWNBEGIN		= 0x00010001,			//!< ����_��ʼ����
		AS_BUSY_UNIONBEGIN		= 0x00010002,			//!< ����_��ʼ�ϲ�

		AS_FINISH_SUCCESS		= 0x00020000,			//!< �ɹ�����

		AS_FINISH_FAILED_ERROR	= 0x00030000,			//!< ʧ�ܽ���_��������
		AS_FINISH_FAILED_CANCEL	= 0x00030001,			//!< ʧ�ܽ���_�û���ֹ
		AS_FINISH_FAILED_PAUSE	= 0x00030002,			//!< ʧ�ܽ���_�û���ͣ
		AS_FINISH_FAILED_LOCAL	= 0x00030003,			//!< ʧ�ܽ���_�����ļ�ʧ��

	};

	/*!
	\brief
		���ع�������Ϣ֪ͨ��wParamΪOwnerID, lParam������Ϣ��ͬ�в�ͬ����
	*/
	enum AUTODOWN_WNDMSG
	{
		AM_BEGIN = WM_USER+0X3000,		//!< ���ؿ�ʼ
		AM_STATUSUPDATE,				//!< ״̬�����ı� lParam = Ŀǰ״̬ enum AUTODOWN_STATE
		AM_SIZEUPDATE,					//!< �ߴ����
		AM_END,							//!< ���ؽ���
										/*
										|  lParam 
										|		0: �ɹ�
										|		1: ���ر�Cancel
										|		2: ���ر���ͣ
										|		3: ��ͨ����ʱԭվ����ʧ��
										|		4: ����д�ļ�����
										|		5: ���ع����е�ʧ��
										|		6: ��������ʧ�ܣ�*��
										|		10: ��������ʱgetfiplʧ��[P2P]
										|		11: ��������ʱProbeʧ��[P2P]
										*/
	};

	/**********************************************************
		���нӿ�
	***********************************************************/
public:
	/*!
	\brief
		����ʵ������������û�а�װSogou toolbarʱ�����ָ��Ҫ����SODA���ͣ���ʧ��

	\param type
		����

	\return 
		Autodowʵ��ָ��
	*/
	static CAutoDownload* createInstance(AUTODOWN_TYPE type);

	/*!
	\brief
		����ʵ��

	\return 
		None
	*/
	static void destroyInsance(CAutoDownload* pDown);

public:

	/*!
	\brief
		��������

	\return 
		Autodow����
	*/
	virtual AUTODOWN_TYPE getType(void) const = 0;

	/*!
	\brief
		ȡ�õ�ǰ״̬
	*/
	virtual AUTODOWN_STATE getState(void) const = 0;

	/*!
	\brief
		ȡ�õ�ǰ״̬���
	*/
	AUTODOWN_STATE_CATEGORY getStateCategory(void) const { return (AUTODOWN_STATE_CATEGORY)(getState()>>16);}

	/*!
	\brief
		ȡ�õ�ǰ�������ص��ļ����ļ���
	*/
	std::string getUrlFileName(void) const;

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
	virtual bool getFileSize(__int64& nFileSize, __int64& nReadSize) const = 0;

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
	virtual int getSpeedInfo(int* pTimeSpend=0, int* pTimeRemain=0) const = 0;

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
	virtual bool init(const char* szDownURL, const char* szFileName, HWND hWnd=0, int nOwnerID=0) = 0;

	/*!
	\brief	
		���ù���״̬[ͬ��]
			ֹͣһ�й������ָ�����ʼ��״̬
	*/
	virtual void reset(void) = 0;

	/*!
	\brief	
		��ʼ����[�첽]
			���Ե���waitStopȷ�����ؽ���
	*/
	virtual void begin(void) = 0;

	/*!
	\brief	
		ǿ��ֹͣ����[�첽]
			���Ե���waitStopȷ����ȫֹͣ
	*/
	virtual void stop(void) = 0;

	/*!
	\brief	
		�ȴ���������
	\param	bBlock
		�Ƿ�ͬ��
	\return
		�Ƿ��Ѿ�������ֻ��bBlockΪtrueʱ��Ч
	*/
	virtual bool waitStop(bool bBlock) = 0;

	/**********************************************************
		���ߺ���
	***********************************************************/
public:
	/*!
	\brief
		���ߺ��������ļ���Сת��Ϊ�ַ�������
		(  -1024)	xxx
		[1K-1024K)	xxx.xK
		[1M-1024M)	xxx.xM
		[1G-1024G)  xxx.xG
		[1T-...)	xxx.xT

	\param nSize
		�ļ���С(byte)
	
	\return 
		ת������ַ�������
	*/
	static std::string fileSize2String(__int64 nSize);

	/**********************************************************
		�ڲ��ӿ�
	***********************************************************/
protected:
	/*!
	\brief	
		����״̬����֪ͨ��ش���
	\param	state
		�µ�״̬
	\return
		None
	*/
	void setState(AUTODOWN_STATE state);

	/**********************************************************
		����/����
	***********************************************************/
protected:
	CAutoDownload(void);
	virtual ~CAutoDownload(void);

	/**********************************************************
		�ڲ�����
	***********************************************************/
	/*!
	\brief	
		ȡ�ù������ݣ��ڲ����ã�
	*/
	struct AUTODOWN_IMPL;
	AUTODOWN_IMPL* getImpl(void) { return m_pDownImpl; }
	const AUTODOWN_IMPL* getConstImpl(void) const{ return m_pDownImpl; }

protected:
	//��������
	AUTODOWN_IMPL*		m_pDownImpl;
};