// UIEditorDoc.cpp : CUIEditorDoc 类的实现
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


// CUIEditorDoc 构造/析构

CUIEditorDoc::CUIEditorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CUIEditorDoc::~CUIEditorDoc()
{
}

BOOL CUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CUIEditorDoc 序列化

void CUIEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CUIEditorDoc 诊断

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


// CUIEditorDoc 命令
