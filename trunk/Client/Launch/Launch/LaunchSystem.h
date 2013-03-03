#pragma once

class CAutoDownload;
class UpdateSystem;
//class FixSystem;

const unsigned long MAX_NEED_SPACE = 200*1024*1024;		// ����������Ҫ�Ĵ��̿ռ䣬
														// �ⲿ�ֿռ����Patch�ļ�������ʱ���ļ�
														// Ŀǰ����Ҫ600M

class LaunchSystem
{
	/***************************************************
		��������
	****************************************************/
public:
	//��������
	enum PROCEDURE	
	{
		PRO_NONE,						//!< ��

		PRO_DOWNLOAD_PATCH_INFO,		//!< 0. �������°汾��Ϣ
		PRO_DOWNLOAD_PATCH,				//!< 1. �������°�
		PRO_DOWNLOAD_SERVER_INFO,		//!< 2. ���ط�����������Ϣ
		PRO_INSTALL_PATCH,				//!< 3. ��װ���°�
	};

	//֪ͨ��Ϣ
	enum NOTIFY_MSG
	{
		NM_STATUS = (WM_USER+0X5000),	//!< ��ǰ״̬��������	wParam-��������
		NM_FILE,						//!< ��ǰ���ڴ�����ļ� wParam-��������
		NM_PROCESS1,					//!< ������1����		wParam-(0-100)
		NM_PROCESS2,					//!< ������2����		wParam-(0-100)
		NM_FAULT,						//!< ��������			wParam-��������
		NM_ENABLE_CANCEL,				//!< ����ȡ��
		NM_DISABLE_CANCEL,				//!< ��ֹȡ��
		NM_OK,							//!< Launch�������
	};

	/***************************************************
		���нӿ�
	****************************************************/
public:
	/*!
	\brief
		ϵͳ��ʼ��

	\param hNotifyWnd
		֪ͨ����

	\param szLaunchIniFile
		�����ļ�

	\return
		None
	*/
	void init(HWND hWnd, const char* szLaunchIniFile = "Launch.ini");

	/*!
	\brief
		�ͻ���fix������������init��ִ��
	\return
		None
	*/
	//void fix();

	/*!
	\brief
		�õ���Ϣ�ļ���ַ

	\return
		��Ϣ�ļ���ַ
	*/
	const std::string&	getHelpURL(void) const { return m_strHelpURL; }

	/*!
	\brief
		�õ���Ϸϵͳ�����ļ�

	\return
		��Ϣ�ļ���ַ
	*/
	const std::string&	getGameConfig(void) const { return m_strGameConfig; }

	/*!
	\brief
		ȡ�õ�ǰ���ڹ������첽����

	\return
		������
	*/
	PROCEDURE getCurrentProcedure(void) const { return m_theProcedure; }

	/*!
	\brief
		������Ϸ

	\return
		None
	*/
	void runGame(void);

	BOOL IsTestVersion()
	{
		return m_bTestVersion;
	}

	/***************************************************
		�ڲ�����
	****************************************************/
private:
	/*!
	\brief
		����ĳһ��ָ������

	\param procedure
		������

	\return
		None
	*/
	//ϵ�к���
	bool enterProcedure_CheckInternetState(void);	//!< �������״��
	bool enterProcedure_CheckDiskState(void);		//!< �����̿ռ�
	bool enterProcedure_DownloadPatchInfo(void);	//!< �������°汾��Ϣ
	bool enterProcedure_DownloadPatch(void);		//!< �������°汾
	bool enterProcedure_DownloadServerInfo(void);	//!< �������·�����������Ϣ
	bool enterProcedure_Install(void);				//!< ��װ���°汾

	//bool enterProcedure_ParseFiles(void);			//!< ������ǰ�汾
	/*!
	\brief 
		�����̺߳���
	*/
	static unsigned int __stdcall _workThread(void* lpParam);
	void workThread(void);

	/*!
	\brief 
		�����̺߳���
	*/
	//static unsigned int __stdcall _fixThread(void* lpParam);
	//void fixThread(void);

	/*!
	\brief 
		���ش�����Ϣ���ܴ���
	*/
	static LRESULT CALLBACK downloadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void FormatErrorMessage( LPSTR lpMessage );
	/***************************************************
		�ڲ�����
	****************************************************/
private:
	BOOL			m_bTestVersion;		//�Ƿ�Ϊ�ڲ����԰�,Ĭ�ϲ���
	std::string		m_strPatchDir;		//!< ����Ŀ¼
	HWND			m_hNotifyWnd;		//!< ֪ͨ����
	std::string		m_strIniFile;		//!< Launch�������ļ�

	std::string		m_strHelpURL;		//!< ��Ϣ��ʾURL
	std::string		m_strGameExe;		//!< ��Ϸexe
	std::string		m_strGameConfig;	//!< ��Ϸ�����ļ�
	std::string		m_strFairyConfig;	//!< FairyResources�����ļ�

	HANDLE			m_hWorkThread;		//!< �����߳̾��
	//HANDLE			m_hFixThread;		//!< �����߳̾��
	PROCEDURE		m_theProcedure;		//!< ȡ�õ�ǰ���ڹ������첽����

	CAutoDownload*	m_pAutodownload;	//!< ���ع���
	HWND			m_hDownloadWnd;		//!< ���ؿ��ƴ���

	typedef std::pair< std::string, std::string> FilePair;

	FilePair		m_strPatchFile;		//!< ���°汾��Ϣ�ļ�
	FilePair		m_strServerInfo;


	UpdateSystem*	m_pUpdateSystem;	//!< ����ϵͳ

	//FixSystem*		m_pFixSystem;		//!< FIXϵͳ

	DWORD			m_dwInternetState;
	int				m_nUpdateExit;		//!< �����߳��˳���
										//!< -1. �߳�û������
										//!< -2. �߳���������������δ֪����û����������
										//!< -3. �߳������������������ش��������
										//!< -4. �߳��������������ڰ�װ���������
										//!< -5. �߳��������������ڴ��̿ռ䲻����������
	
	/***************************************************
		����/����
	****************************************************/
public:
	LaunchSystem();
	~LaunchSystem();
	
	/*!
	\brief
		�õ�����

	\return
		����ָ��
	*/
	static LaunchSystem*	GetMe(void) { return s_pMe; }

private:
	static LaunchSystem* s_pMe;	//!< ���ڱ��������ָ��
};