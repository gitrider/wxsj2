#if !defined(AFX_BDIALOG_H__A303185C_4254_4F5F_B5A8_462531A59B83__INCLUDED_)
#define AFX_BDIALOG_H__A303185C_4254_4F5F_B5A8_462531A59B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define StyleTile 0
#define StyleCenter 1
#define StyleStretch 2

class CBDialog : public CDialog
{
// Construction
public:
	int SetBitmap(UINT nIDResource);
	void SetBitmapStyle(int style);

	CBDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBDialog)	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBDialog)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bitmap;
	int m_style;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDIALOG_H__A303185C_4254_4F5F_B5A8_462531A59B83__INCLUDED_)
