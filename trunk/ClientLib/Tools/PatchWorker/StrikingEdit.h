#pragma once


// CStrikingEdit

class CStrikingEdit : public CEdit
{
	DECLARE_DYNAMIC(CStrikingEdit)

public:
	CStrikingEdit();
	virtual ~CStrikingEdit();

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


