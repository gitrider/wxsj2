#pragma once


// CImageSetDlg �Ի���

class CImageSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageSetDlg)

public:
	CImageSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageSetDlg();
	TCHAR* GetImageSetString();

// �Ի�������
	enum { IDD = IDD_IMAGESET_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
