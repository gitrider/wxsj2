/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/checkbox.h
// Purpose:     wxCheckBox class

#ifndef _WX_GRAPHITEM_H_
#define _WX_GRAPHITEM_H_

#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/msw/control.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GraphItem
///////////////////////////////////////////////////////////////////////////////
class wxGraphItem : public wxControl
{
private:

protected:
	wxCheckBox* m_ItemcheckBox;

public:
	wxGraphItem(){}
	wxGraphItem( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& label = wxT("GraphItem"),const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,12), long style = 0|wxNO_BORDER );
	~wxGraphItem();

	bool Create(wxWindow* parent, wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSUNKEN_BORDER,
		const wxValidator& validator = wxDefaultValidator);

	virtual wxSize DoGetBestSize() const;

	void OnPaint(wxPaintEvent& event);

	DECLARE_DYNAMIC_CLASS(wxGraphItem)
	DECLARE_EVENT_TABLE()
	DECLARE_NO_COPY_CLASS(wxGraphItem)

};

#endif
// _WX_CHECKBOX_H_
