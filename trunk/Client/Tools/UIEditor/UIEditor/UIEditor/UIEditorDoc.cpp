// UIEditorDoc.cpp : CUIEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "UIEditor.h"

#include "UIEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIEditorDoc

IMPLEMENT_DYNCREATE(CUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIEditorDoc, CDocument)
END_MESSAGE_MAP()


// CUIEditorDoc ����/����

CUIEditorDoc::CUIEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CUIEditorDoc::~CUIEditorDoc()
{
}

BOOL CUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CUIEditorDoc ���л�

void CUIEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CUIEditorDoc ���

#ifdef _DEBUG
void CUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIEditorDoc ����
