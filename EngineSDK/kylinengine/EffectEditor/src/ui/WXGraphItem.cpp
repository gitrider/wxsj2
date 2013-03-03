
#include "WXGraphItem.h"
#include <wx/gdicmn.h>
#include <wx/event.h>
#include <wx/msw/dcclient.h>
///////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC_CLASS(wxGraphItem, wxControl)
BEGIN_EVENT_TABLE( wxGraphItem, wxControl )
EVT_PAINT( wxGraphItem::OnPaint )

END_EVENT_TABLE()
wxGraphItem::wxGraphItem( wxWindow* parent,
						 wxWindowID id,
						 const wxString& label,
						 const wxPoint& pos, 
						 const wxSize& size, 
						 long style ) : wxControl( parent, id, pos, size, style )
{
	Create(parent,id,label,pos,size,style);
	
}
bool wxGraphItem::Create(wxWindow* parent,
						 wxWindowID id,
						 const wxString& label,
						 const wxPoint& pos,
						 const wxSize& size, 
						 long style,
						 const wxValidator& validator)
{
	//this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	m_ItemcheckBox = new wxCheckBox( this, wxID_ANY, label, wxDefaultPosition, wxSize(80,-1), 0|wxNO_BORDER );
	return true;
}

wxGraphItem::~wxGraphItem()
{
}

wxSize wxGraphItem::DoGetBestSize() const
{
	wxSize size = GetSize();
	wxSize best =wxSize(size.GetWidth(),size.GetHeight());
	CacheBestSize(best);
	return best;
}

void wxGraphItem::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxControl::OnPaint(event);

}