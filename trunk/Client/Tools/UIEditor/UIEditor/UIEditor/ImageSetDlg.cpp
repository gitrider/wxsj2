// ImageSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "ImageSetDlg.h"


// CImageSetDlg 对话框

IMPLEMENT_DYNAMIC(CImageSetDlg, CDialog)

CImageSetDlg::CImageSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageSetDlg::IDD, pParent)
{

}

CImageSetDlg::~CImageSetDlg()
{
}

void CImageSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageSetDlg, CDialog)
END_MESSAGE_MAP()


// CImageSetDlg 消息处理程序

TCHAR* CImageSetDlg::GetImageSetString()
{
	return NULL;
}