// ImageSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UIEditor.h"
#include "ImageSetDlg.h"


// CImageSetDlg �Ի���

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


// CImageSetDlg ��Ϣ�������

TCHAR* CImageSetDlg::GetImageSetString()
{
	return NULL;
}