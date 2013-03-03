/////////////////////////////////////////////////////////////////////////////
// Name:        CDlgChangeMonsterInfo.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __CDlgChangeMonsterInfo_H__
#define __CDlgChangeMonsterInfo_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "CDlgChangeMonsterInfo."
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/ChangeMonsgerInfo_wdr.h"
// WDR: class declarations

#include <wx/dialog.h>
#include <wx/image.h>

class CDlgChangeMosterInfo : public wxDialog
{

public:
	enum CHANGE_TYPE
	{
		
		NONE = 0,
		RADIUS,
		TIME,
		BOTH,

	};

	 DECLARE_DYNAMIC_CLASS(CDlgChangeMosterInfo)
	 
public:
    CDlgChangeMosterInfo() 
		 : m_iChangeType(NONE)
		 , m_iTime(1000)
		 , m_fRadius(2.0f)
		 , m_pTimeBn(NULL)
		 , m_pRadiusBn(NULL)
		 , m_pBothBn(NULL)
		 , m_pNoneBn(NULL)
		 , m_pAreaText(NULL)
		 , m_pTimeText(NULL)
	{
	}
 
    // full ctor
    CDlgChangeMosterInfo(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name)
		, m_iChangeType(NONE)
		, m_iTime(1000)
		, m_fRadius(2.0f)
		, m_pTimeBn(NULL)
		, m_pRadiusBn(NULL)
		, m_pBothBn(NULL)
		, m_pNoneBn(NULL)
		, m_pAreaText(NULL)
		, m_pTimeText(NULL)

    {
    }

protected:

    DECLARE_EVENT_TABLE()

    void OnInitDialog(wxInitDialogEvent &e);


public:

	int		m_iChangeType;
	int		m_iTime;
	float	m_fRadius;

	// ʱ�䰴ť.
	wxButton*  m_pTimeBn;

	// �뾶��ť.
	wxButton*  m_pRadiusBn;

	// �������޸İ�ť.
	wxButton*  m_pBothBn;

	// ���޸İ�ť.
	wxButton*  m_pNoneBn;



	// ��뾶�ı���
	wxTextCtrl* m_pAreaText;

	// ʱ��뾶�ı���
	wxTextCtrl* m_pTimeText;

	// �����Ի���
	void OnEndModifyDlg( wxCommandEvent &event );


};



#endif
