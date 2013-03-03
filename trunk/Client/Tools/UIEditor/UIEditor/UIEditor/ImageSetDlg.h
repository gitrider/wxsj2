#pragma once


// CImageSetDlg 对话框

class CImageSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageSetDlg)

public:
	CImageSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageSetDlg();
	TCHAR* GetImageSetString();

// 对话框数据
	enum { IDD = IDD_IMAGESET_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
