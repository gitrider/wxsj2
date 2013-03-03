// StrikingEdit.cpp : implementation file
//

#include "stdafx.h"
#include "PatchWorker.h"
#include "StrikingEdit.h"
#include ".\strikingedit.h"


// CStrikingEdit

IMPLEMENT_DYNAMIC(CStrikingEdit, CEdit)
CStrikingEdit::CStrikingEdit()
{
}

CStrikingEdit::~CStrikingEdit()
{
}


BEGIN_MESSAGE_MAP(CStrikingEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CStrikingEdit message handlers


void CStrikingEdit::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here
}

void CStrikingEdit::OnEnKillfocus()
{
	// TODO: Add your control notification handler code here
}

void CStrikingEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	SetReadOnly(TRUE);
	Invalidate();

	OutputDebugString("KILL_FOCUS\n");
}

void CStrikingEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	SetReadOnly(FALSE);

	Invalidate();

	OutputDebugString("SET_FOCUS\n");
}
