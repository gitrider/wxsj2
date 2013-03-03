/////////////////////////////////////////////////////////////////////////////
// Name:        manager.cpp
// Purpose:     wxPropertyGridManager
// Author:      Jaakko Salli
// Modified by:
// Created:     Jan-14-2005
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "manager.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/object.h"
    #include "wx/hash.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/event.h"
    #include "wx/window.h"
    #include "wx/panel.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    //#include "wx/dcmemory.h"
    #include "wx/button.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/cursor.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/choice.h"
    #include "wx/stattext.h"
    #include "wx/textctrl.h"
    #include "wx/scrolwin.h"
    #include "wx/dirdlg.h"
    //#include "wx/listbox.h"
    #include "wx/combobox.h"
    #include "wx/layout.h"
    #include "wx/sizer.h"
    #include "wx/textdlg.h"
    #include "wx/filedlg.h"
    #include "wx/statusbr.h"
    #include "wx/toolbar.h"
    #include "wx/intl.h"
#endif

// This define is necessary to prevent macro clearing
#define __wxPG_SOURCE_FILE__

#include <wx/propgrid/propgrid.h>

#include <wx/propgrid/manager.h>

// -----------------------------------------------------------------------

// For wxMSW cursor consistency, we must do mouse capturing even
// when using custom controls

# define BEGIN_MOUSE_CAPTURE \
    if ( !(m_iFlags & wxPG_FL_MOUSE_CAPTURED) ) \
    { \
        CaptureMouse(); \
        m_iFlags |= wxPG_FL_MOUSE_CAPTURED; \
    }

# define END_MOUSE_CAPTURE \
    if ( m_iFlags & wxPG_FL_MOUSE_CAPTURED ) \
    { \
        ReleaseMouse(); \
        m_iFlags &= ~(wxPG_FL_MOUSE_CAPTURED); \
    }

// -----------------------------------------------------------------------
// wxPropertyGridManager
// -----------------------------------------------------------------------

const wxChar *wxPropertyGridManagerNameStr = wxT("wxPropertyGridManager");

/* Categoric Mode Icon */
static char* gs_xpm_catmode[] = {
"16 16 5 1",
". c none",
"B c black",
"D c #868686",
"L c #CACACA",
"W c #FFFFFF",
".DDD............",
".DLD.BBBBBB.....",
".DDD............",
".....DDDDD.DDD..",
"................",
".....DDDDD.DDD..",
"................",
".....DDDDD.DDD..",
"................",
".....DDDDD.DDD..",
"................",
".DDD............",
".DLD.BBBBBB.....",
".DDD............",
".....DDDDD.DDD..",
"................"
};

/* Alphabetic Mode Icon */
static char* gs_xpm_noncatmode[] = {
"16 16 5 1",
". c none",
"B c black",
"D c #868686",
"L c #000080",
"W c #FFFFFF",
"..DBD...DDD.DDD.",
".DB.BD..........",
".BBBBB..DDD.DDD.",
".B...B..........",
"...L....DDD.DDD.",
"...L............",
".L.L.L..DDD.DDD.",
"..LLL...........",
"...L....DDD.DDD.",
"................",
".BBBBB..DDD.DDD.",
"....BD..........",
"...BD...DDD.DDD.",
"..BD............",
".BBBBB..DDD.DDD.",
"................"
};

/* Default Page Icon. */
static char* gs_xpm_defpage[] = {
"16 16 5 1",
". c none",
"B c black",
"D c #868686",
"L c #000080",
"W c #FFFFFF",
"................",
"................",
"..BBBBBBBBBBBB..",
"..B..........B..",
"..B.BB.LLLLL.B..",
"..B..........B..",
"..B.BB.LLLLL.B..",
"..B..........B..",
"..B.BB.LLLLL.B..",
"..B..........B..",
"..B.BB.LLLLL.B..",
"..B..........B..",
"..BBBBBBBBBBBB..",
"................",
"................",
"................"
};

// -----------------------------------------------------------------------

/** \class wxPropertyGridPageData
    \ingroup classes
    \brief
    Simple holder of propertygrid page information.
*/
class wxPropertyGridPageData
{
public:

    wxPropertyGridPageData() { }
    ~wxPropertyGridPageData() { }

    wxString                m_label;
    wxPropertyGridState     m_state;
    int                     m_id;
};

// -----------------------------------------------------------------------

// Final default splitter y is client height minus this.
#define wxPGMAN_DEFAULT_NEGATIVE_SPLITTER_Y         100

// -----------------------------------------------------------------------

IMPLEMENT_CLASS(wxPropertyGridManager, wxPanel)

#define ID_ADVTOOLBAR_OFFSET        1
#define ID_ADVHELPCAPTION_OFFSET    2
#define ID_ADVHELPCONTENT_OFFSET    3
#define ID_ADVBUTTON_OFFSET         4
#define ID_ADVTBITEMSBASE_OFFSET    5   // Must be last.

// -----------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxPropertyGridManager, wxPanel)
  EVT_MOTION(wxPropertyGridManager::OnMouseMove)
  EVT_SIZE(wxPropertyGridManager::OnResize)
  EVT_PAINT(wxPropertyGridManager::OnPaint)
  EVT_LEFT_DOWN(wxPropertyGridManager::OnMouseClick)
  EVT_LEFT_UP(wxPropertyGridManager::OnMouseUp)
  EVT_LEAVE_WINDOW(wxPropertyGridManager::OnMouseEntry)
  //EVT_ENTER_WINDOW(wxPropertyGridManager::OnMouseEntry)
END_EVENT_TABLE()

// -----------------------------------------------------------------------

wxPropertyGridManager::wxPropertyGridManager()
    : wxPanel()
{
    Init1();
}

// -----------------------------------------------------------------------

wxPropertyGridManager::wxPropertyGridManager (wxWindow *parent, wxWindowID id,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxChar* name )
    : wxPanel( parent, id, pos, size, (style&0xFFFF0000)|wxWANTS_CHARS, name )
{
    Init1();
    Init2(style);
}

// -----------------------------------------------------------------------

bool wxPropertyGridManager::Create ( wxWindow *parent, wxWindowID id ,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxChar* name )
{

    bool res = wxPanel::Create ( parent, id, pos, size, (style&0xFFFF0000)|wxWANTS_CHARS, name );
    Init2(style);

    return res;
}

// -----------------------------------------------------------------------

//
// Initialize values to defaults
//
void wxPropertyGridManager::Init1()
{

#if wxUSE_TOOLBAR
    m_pToolbar = (wxToolBar*) NULL;
#endif
    m_pTxtHelpCaption = (wxStaticText*) NULL;
    m_pTxtHelpContent = (wxStaticText*) NULL;
    m_pButCompactor = (wxButton*) NULL;

    m_targetState = (wxPropertyGridState*) NULL;

    m_selPage = -1;

    m_width = m_height = 0;

    m_splitterHeight = 5;

    m_splitterY = -1; // -1 causes default to be set.

    m_nextDescBoxSize = -1;

    m_extraHeight = 0;
    m_dragStatus = 0;
    m_onSplitter = 0;
    m_iFlags = 0;

}

// -----------------------------------------------------------------------

// These flags are always used in wxPropertyGrid integrated in xPropertyGridManager.
#define wxPG_MAN_PROPGRID_FORCED_FLAGS (wxPG_AUTO_SORT|wxSIMPLE_BORDER| \
                                        wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN)

// Which flags can be passed to underlying wxPropertyGrid.
#define wxPG_MAN_PASS_FLAGS_MASK       (0xFFF0|wxTAB_TRAVERSAL)

//
// Initialize after parent etc. set
//
void wxPropertyGridManager::Init2 ( int style )
{

    if ( m_iFlags & wxPG_FL_INITIALIZED )
        return;

    m_windowStyle |= (style&0x0000FFFF);

    wxSize csz = GetClientSize();

    m_cursorSizeNS = wxCursor(wxCURSOR_SIZENS);

    // Prepare the first page
    wxPropertyGridPageData* pd = new wxPropertyGridPageData();
    pd->m_state.m_pPropGrid = &m_propGrid;
    m_arrPages.Add( (void*)pd );
    m_propGrid.m_pState = &pd->m_state;
    m_targetState = &pd->m_state;

    wxWindowID base_id = GetId();

    // Create propertygrid.
    m_propGrid.Create(this,base_id,wxPoint(0,0),csz,
        (m_windowStyle&wxPG_MAN_PASS_FLAGS_MASK)
        |wxPG_MAN_PROPGRID_FORCED_FLAGS
      );

    m_propGrid.m_iFlags |= wxPG_FL_IN_MANAGER;

    m_pState = m_propGrid.m_pState;

    //m_propGrid.SetCursor ( *wxSTANDARD_CURSOR );
    //m_propGrid.SetCursor ( wxPG_DEFAULT_CURSOR );
    m_propGrid.SetExtraStyle(wxPG_EX_INIT_NOCAT);

    m_nextTbInd = base_id+ID_ADVTBITEMSBASE_OFFSET + 2;

    // Connect to property grid onselect event.
    Connect(base_id,
            wxEVT_PG_SELECTED,
            (wxObjectEventFunction) (wxEventFunction) (wxPropertyGridEventFunction)
            &wxPropertyGridManager::OnPropertyGridSelect );

    // Connect to compactor button event.
    Connect(base_id+ID_ADVBUTTON_OFFSET,
            wxEVT_COMMAND_BUTTON_CLICKED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &wxPropertyGridManager::OnCompactorClick );

    // Connect to toolbar button events.
    Connect(base_id+ID_ADVTBITEMSBASE_OFFSET,base_id+ID_ADVTBITEMSBASE_OFFSET+50,
            wxEVT_COMMAND_TOOL_CLICKED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &wxPropertyGridManager::OnToolbarClick );

    /*
    // Connect to wxPropertyGrid mouse entry.
    Connect(base_id,
            wxEVT_ENTER_WINDOW,
            (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction)
            &wxPropertyGridManager::OnMouseEntry );

    // Connect to button mouse entry.
    Connect(base_id+ID_ADVBUTTON_OFFSET,
            wxEVT_ENTER_WINDOW,
            (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction)
            &wxPropertyGridManager::OnMouseEntry );
    */

#if !wxUSE_TOOLBAR && defined(__WXDEBUG__)
    if ( m_windowStyle & wxPG_TOOLBAR )
        wxLogDebug(wxT("WARNING: wxPropertyGridManager ignores style %s with this library build."),wxT("wxPG_TOOLBAR"));
#endif

    // Optional initial controls.
    m_width = -12345;
    //RecreateControls();

    m_iFlags |= wxPG_FL_INITIALIZED;

}

// -----------------------------------------------------------------------

wxPropertyGridManager::~wxPropertyGridManager()
{
    END_MOUSE_CAPTURE

#if wxUSE_TOOLBAR
    /*if ( m_pBmpCatMode ) delete m_pBmpCatMode;
    if ( m_pBmpNonCatMode ) delete m_pBmpNonCatMode;*/
#endif

    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        // Do not delete selected, since its pointers etc. are copied
        // to propgrid, which automatically deletes it.
        //if ( i != (size_t)m_selPage )
        delete (wxPropertyGridPageData*)m_arrPages.Item(i);
    }

}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetId( wxWindowID winid )
{
    wxWindow::SetId(winid);

    // TODO: Reconnect propgrid event handler(s).

    m_propGrid.SetId(winid);
}

// -----------------------------------------------------------------------

wxSize wxPropertyGridManager::DoGetBestSize() const
{
    /*
    //wxSize sz = m_propGrid.DoGetBestSize();
    wxSize sz(60,m_propGrid.m_lineHeight);
    wxLogDebug(wxT("m_extraHeight: %i"),m_extraHeight);
    sz.y += m_extraHeight;
    wxLogDebug(wxT("sz.y: %i"),sz.y);
    //CacheBestSize(sz);
    return sz;
    */
    return wxSize(60,150);
}

// -----------------------------------------------------------------------

bool wxPropertyGridManager::SetFont ( const wxFont& font )
{
    bool res = wxWindow::SetFont(font);
    m_propGrid.SetFont(font);
    // TODO: Need to do caption recacalculations for other pages as well.
    return res;
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetExtraStyle ( long exStyle )
{
    wxWindow::SetExtraStyle ( exStyle );
    m_propGrid.SetExtraStyle ( exStyle & 0xFFFFF000 );
#if wxUSE_TOOLBAR
    if ( (exStyle & wxPG_EX_NO_FLAT_TOOLBAR) && m_pToolbar )
        RecreateControls();
#endif
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::Freeze()
{
    m_propGrid.Freeze();
    wxWindow::Freeze();
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::Thaw()
{
    wxWindow::Thaw();
    m_propGrid.Thaw();
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetWindowStyleFlag ( long style )
{
    wxWindow::SetWindowStyleFlag( style /*& 0xFFFF000F*/ );
    m_propGrid.SetWindowStyleFlag( (style & wxPG_MAN_PASS_FLAGS_MASK) | wxPG_MAN_PROPGRID_FORCED_FLAGS );
}

// -----------------------------------------------------------------------

// Actually shows given page.
void wxPropertyGridManager::SelectPage ( int index )
{
    wxASSERT ( m_selPage >= 0 );

    wxCHECK_RET( index >= 0 && index < (int)m_arrPages.GetCount(),
        wxT("invalid page index") );

    if ( m_selPage == index )
        return;

    wxPropertyGridPageData* pd_next = (wxPropertyGridPageData*)m_arrPages.Item(index);

    m_iFlags |= wxPG_FL_DESC_REFRESH_REQUIRED;

    m_propGrid.SwitchState ( &pd_next->m_state );

    m_targetState = &pd_next->m_state;
    m_pState = m_propGrid.m_pState;

    m_selPage = index;

#if wxUSE_TOOLBAR
    if ( m_pToolbar )
    {
        m_pToolbar->ToggleTool ( pd_next->m_id, TRUE );
    }
#endif

}

// -----------------------------------------------------------------------

int wxPropertyGridManager::GetPageByName ( const wxChar* name ) const
{
    wxASSERT ( name );

    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        if ( ((wxPropertyGridPageData*)m_arrPages.Item(i))->m_label == name )
            return i;
    }
    return wxNOT_FOUND;
}

// -----------------------------------------------------------------------

int wxPropertyGridManager::GetPageByState( wxPropertyGridState* pstate ) const
{
    wxASSERT ( pstate );

    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        if ( pstate == &((wxPropertyGridPageData*)m_arrPages.Item(i))->m_state )
            return i;
    }
    return wxNOT_FOUND;
}

// -----------------------------------------------------------------------

const wxString& wxPropertyGridManager::GetPageName ( int index ) const
{
    wxASSERT ( index >= 0 && index < (int)m_arrPages.GetCount() );
    return ((wxPropertyGridPageData*)m_arrPages.Item(index))->m_label;
}

// -----------------------------------------------------------------------

// Sets page for append, insert, etc. operations.
void wxPropertyGridManager::SetTargetPage ( int index )
{
    wxASSERT ( m_selPage >= 0 );
    wxASSERT ( index >= 0 );
    wxASSERT ( index < (int)m_arrPages.GetCount() );

    m_targetState = &((wxPropertyGridPageData*)m_arrPages.Item(index))->m_state;

}

// -----------------------------------------------------------------------

void wxPropertyGridManager::Compact ( bool compact )
{
    if ( m_pButCompactor )
    {
        if ( compact ) m_pButCompactor->SetLabel(wxT("Expand >>"));
        else m_pButCompactor->SetLabel(wxT("<< Compact"));
    }
    m_propGrid.Compact(compact);
}

// -----------------------------------------------------------------------

int wxPropertyGridManager::InsertPage ( int index, const wxString& label, const wxBitmap&
#if wxUSE_TOOLBAR
    bmp
#endif
)
{
    if ( index < 0 )
        index = m_arrPages.GetCount();

    //wxASSERT ( (size_t)index <= m_arrPages.GetCount() );
    wxCHECK_MSG ( (size_t)index == m_arrPages.GetCount(), -1,
        wxT("wxPropertyGridManager currently only supports appending pages (due to wxToolBar limitation)."));

    wxPropertyGridPageData* pd;
    if ( m_selPage >= 0 )
    {
        pd = new wxPropertyGridPageData();
        pd->m_state.m_pPropGrid = &m_propGrid;
        pd->m_state.InitNonCatMode();
        m_targetState = &pd->m_state;
    }
    else
        pd = (wxPropertyGridPageData*)m_arrPages.Item(0);

    pd->m_label = label;
    pd->m_id = m_nextTbInd;

    if ( m_selPage >= 0 )
        //m_arrPages.Insert( (void*)pd, index );
        m_arrPages.Add( (void*)pd );

#if wxUSE_TOOLBAR
    if ( m_windowStyle & wxPG_TOOLBAR )
    {
        if ( !m_pToolbar )
            RecreateControls();

        wxASSERT ( m_pToolbar );

        // Add separator before first page.
        if ( m_arrPages.GetCount() < 2 && !(GetExtraStyle()&wxPG_EX_NO_MODE_BUTTONS) )
            m_pToolbar->AddSeparator();

        if ( &bmp != &wxNullBitmap )
            m_pToolbar->AddTool(m_nextTbInd,label,bmp,label,wxITEM_RADIO);
            //m_pToolbar->InsertTool(index+3,m_nextTbInd,bmp);
        else
            m_pToolbar->AddTool(m_nextTbInd,label,wxBitmap( (const char**)gs_xpm_defpage ),
                label,wxITEM_RADIO);

        m_nextTbInd++;

        m_pToolbar->Realize();

    }
#endif

    // Need to increase selected index?
    if ( m_selPage >= 0 )
    {
        if ( m_selPage >= index )
        {
            m_selPage += 1;
        }
    }
    else
    {
        // Set reset it.
        m_selPage = 0;
    }

    return index;
}

// -----------------------------------------------------------------------

bool wxPropertyGridManager::IsAnyModified () const
{
    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        if ( ((wxPropertyGridPageData*)m_arrPages.Item(i))->m_state.m_anyModified )
            return TRUE;
    }
    return FALSE;
}

// -----------------------------------------------------------------------

bool wxPropertyGridManager::IsPageModified ( size_t index ) const
{
    if ( ((wxPropertyGridPageData*)m_arrPages.Item(index))->m_state.m_anyModified )
        return TRUE;
    return FALSE;
}
    
// -----------------------------------------------------------------------

void wxPropertyGridManager::RepaintSplitter ( int new_splittery, int new_width, int new_height,
                                              bool desc_too )
{
    wxClientDC dc(this);

    int use_hei = new_height;
    if ( m_pButCompactor )
        use_hei = m_pButCompactor->GetPosition().y;

    // Draw background
    wxColour bgcol = GetBackgroundColour();
    dc.SetBrush( bgcol );
    dc.SetPen( bgcol );
    int rect_hei = use_hei-new_splittery;
    if ( !desc_too )
        rect_hei = m_splitterHeight;
    dc.DrawRectangle(0,new_splittery,new_width,rect_hei);
    dc.SetPen ( wxSystemSettings::GetColour ( wxSYS_COLOUR_3DDKSHADOW ) );
    int splitter_bottom = new_splittery+m_splitterHeight - 1;
    int box_height = use_hei-splitter_bottom;
    if ( box_height > 1 )
        dc.DrawRectangle(0,splitter_bottom,new_width,box_height);
    else
        dc.DrawLine(0,splitter_bottom,new_width,splitter_bottom);
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::RefreshHelpBox ( int new_splittery, int new_width, int new_height )
{
    //if ( new_splittery == m_splitterY && new_width == m_width )
    //    return;

    int use_hei = new_height;
    if ( m_pButCompactor )
        use_hei = m_pButCompactor->GetPosition().y;
    use_hei--;

    //wxRendererNative::Get().DrawSplitterSash(this,dc,
        //wxSize(width,m_splitterHeight),new_splittery,wxHORIZONTAL);

    //wxRendererNative::Get().DrawSplitterBorder(this,dc,
    //    wxRect(0,new_splittery,new_width,m_splitterHeight));

    // Fix help control positions.
    int cap_hei = m_propGrid.m_fontHeight;
    int cap_y = new_splittery+m_splitterHeight+5;
    int cnt_y = cap_y+cap_hei+3;
    int sub_cap_hei = cap_y+cap_hei-use_hei;
    int cnt_hei = use_hei-cnt_y;
    if ( sub_cap_hei > 0 )
    {
        cap_hei -= sub_cap_hei;
        cnt_hei = 0;
    }
    if ( cap_hei <= 2 )
    {
        m_pTxtHelpCaption->Show( FALSE );
        m_pTxtHelpContent->Show( FALSE );
    }
    else
    {
        m_pTxtHelpCaption->SetSize(3,cap_y,new_width-6,cap_hei);
        m_pTxtHelpCaption->Show( TRUE );
        if ( cnt_hei <= 2 )
        {
            m_pTxtHelpContent->Show( FALSE );
        }
        else
        {
            m_pTxtHelpContent->SetSize(3,cnt_y,new_width-6,cnt_hei);
            m_pTxtHelpContent->Show( TRUE );
        }
    }

    RepaintSplitter ( new_splittery, new_width, new_height, TRUE );

    m_splitterY = new_splittery;

    m_iFlags &= ~(wxPG_FL_DESC_REFRESH_REQUIRED);
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::RecalculatePositions ( int width, int height )
{

    int propgrid_y = 0;
    int propgrid_bottom_y = height;
    //int button_top = height;

    // Toolbar at the top.
#if wxUSE_TOOLBAR
    if ( m_pToolbar )
    {
        m_pToolbar->SetSize(0,0,width,-1);
        propgrid_y = m_pToolbar->GetSize().y;
    }
#endif

    // Button at the bottom.
    if ( m_pButCompactor )
    {
        int but_hei = m_pButCompactor->GetSize().y;
        m_pButCompactor->SetSize(0,height-but_hei,width,but_hei);
        propgrid_bottom_y -= but_hei;
        //button_top -= but_hei;
    }

    // Help box.
    if ( m_pTxtHelpCaption )
    {
        int new_splittery = m_splitterY;

        // Move m_splitterY
        if ( ( m_splitterY >= 0 || m_nextDescBoxSize ) && m_height > 32 )
        {
            if ( m_nextDescBoxSize >= 0 )
            {
                new_splittery = m_height - m_nextDescBoxSize - m_splitterHeight;
                m_nextDescBoxSize = -1;
            }
            new_splittery += (height-m_height);
        }
        else
        {
            new_splittery = height - wxPGMAN_DEFAULT_NEGATIVE_SPLITTER_Y;
            if ( new_splittery < 32 )
                new_splittery = 32;
        }

        // Check if beyond minimum.
        int nspy_min = propgrid_y + m_propGrid.m_lineHeight;
        if ( new_splittery < nspy_min )
            new_splittery = nspy_min;

        propgrid_bottom_y = new_splittery;

        RefreshHelpBox( new_splittery, width, height );
    }

    int pgh = propgrid_bottom_y - propgrid_y;
    m_propGrid.SetSize ( 0, propgrid_y,
        width, pgh );

    m_extraHeight = height - pgh;

    m_width = width;
    m_height = height;

    //InvalidateBestSize();

}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetDescBoxHeight ( int ht, bool refresh )
{
    if ( m_windowStyle & wxPG_DESCRIPTION )
    {
        m_nextDescBoxSize = ht;
        if ( refresh )
            RecalculatePositions(m_width,m_height);
        /*
        int newypos = m_height - ht - m_splitterHeight;
        if ( refresh && m_pTxtHelpContent )
            RefreshHelpBox(newypos,m_width,m_height);
        else
            m_splitterY = newypos;*/
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnPaint ( wxPaintEvent& WXUNUSED(event) )
{
    wxPaintDC dc(this);

    // Update everything inside the box
    wxRect r = GetUpdateRegion().GetBox();

    // Repaint splitter?
    int r_bottom = r.y + r.height;
    int splitter_bottom = m_splitterY + m_splitterHeight;
    if ( r.y < splitter_bottom && r_bottom >= m_splitterY )
        RepaintSplitter ( m_splitterY, m_width, m_height, FALSE );

}

// -----------------------------------------------------------------------

void wxPropertyGridManager::Refresh (bool eraseBackground, const wxRect* rect )
{
    m_propGrid.Refresh(eraseBackground);
    wxWindow::Refresh(eraseBackground,rect);
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::RecreateControls ()
{

    bool was_shown = IsShown();
    if ( was_shown )
        Show ( FALSE );

    int base_id = m_propGrid.GetId();

#if wxUSE_TOOLBAR
    if ( m_windowStyle & wxPG_TOOLBAR )
    {
        // Has toolbar.
        if ( !m_pToolbar )
        {
            m_pToolbar = new wxToolBar (this,base_id+ID_ADVTOOLBAR_OFFSET,
                wxDefaultPosition,wxDefaultSize,
                ((GetExtraStyle()&wxPG_EX_NO_FLAT_TOOLBAR)?0:wxTB_FLAT) );
            m_pToolbar->SetCursor ( *wxSTANDARD_CURSOR );

            if ( !(GetExtraStyle()&wxPG_EX_NO_MODE_BUTTONS) )
            {
                wxString desc1(_("Categorized Mode"));
                wxString desc2(_("Alphabetic Mode"));
                m_pToolbar->AddTool(base_id+ID_ADVTBITEMSBASE_OFFSET+0,
                    desc1,wxBitmap ( (const char**)gs_xpm_catmode ),
                    desc1,wxITEM_RADIO);
                m_pToolbar->AddTool(base_id+ID_ADVTBITEMSBASE_OFFSET+1,
                    desc2,wxBitmap ( (const char**)gs_xpm_noncatmode ),
                    desc2,wxITEM_RADIO);
                m_pToolbar->Realize();
            }

        }

        if ( !(GetExtraStyle()&wxPG_EX_NO_MODE_BUTTONS) )
        {
            // Toggle correct mode button.
            // TODO: This doesn't work in wxMSW (when changing,
            // both items will get toggled).
            int toggle_but_on_ind = ID_ADVTBITEMSBASE_OFFSET+0;
            int toggle_but_off_ind = ID_ADVTBITEMSBASE_OFFSET+1;
            if ( m_propGrid.m_pState->IsInNonCatMode() )
            {
                toggle_but_on_ind++;
                toggle_but_off_ind--;
            }
        
            m_pToolbar->ToggleTool(base_id+toggle_but_on_ind,TRUE);
            m_pToolbar->ToggleTool(base_id+toggle_but_off_ind,FALSE);
        }

    }
    else
    {
        // No toolbar.
        if ( m_pToolbar )
            m_pToolbar->Destroy();
        m_pToolbar = (wxToolBar*) NULL;
    }
#endif

    if ( m_windowStyle & wxPG_COMPACTOR )
    {
        // Has button.
        if ( !m_pButCompactor )
        {
            m_pButCompactor = new wxButton (this,base_id+ID_ADVBUTTON_OFFSET,
                !(m_propGrid.m_iFlags & wxPG_FL_HIDE_STATE)?_("<< Compact"):_("Expand >>"));
            m_pButCompactor->SetCursor ( *wxSTANDARD_CURSOR );
        }
    }
    else
    {
        // No button.
        if ( m_pButCompactor )
            m_pButCompactor->Destroy();
        m_pButCompactor = (wxButton*) NULL;
    }

    if ( m_windowStyle & wxPG_DESCRIPTION )
    {
        // Has help box.
        m_propGrid.m_iFlags |= (wxPG_FL_NOSTATUSBARHELP);

        if ( !m_pTxtHelpCaption )
        {
            m_pTxtHelpCaption = new wxStaticText (this,base_id+ID_ADVHELPCAPTION_OFFSET,wxT(""));
            m_pTxtHelpCaption->SetFont( m_propGrid.m_captionFont );
            m_pTxtHelpCaption->SetCursor ( *wxSTANDARD_CURSOR );
        }
        if ( !m_pTxtHelpContent )
        {
            m_pTxtHelpContent = new wxStaticText (this,base_id+ID_ADVHELPCONTENT_OFFSET,
                wxT(""),wxDefaultPosition,wxDefaultSize,wxALIGN_LEFT|wxST_NO_AUTORESIZE);
            m_pTxtHelpContent->SetCursor ( *wxSTANDARD_CURSOR );
        }
    }
    else
    {
        // No help box.
        m_propGrid.m_iFlags &= ~(wxPG_FL_NOSTATUSBARHELP);

        if ( m_pTxtHelpCaption )
            m_pTxtHelpCaption->Destroy();

        m_pTxtHelpCaption = (wxStaticText*) NULL;

        if ( m_pTxtHelpContent )
            m_pTxtHelpContent->Destroy();

        m_pTxtHelpContent = (wxStaticText*) NULL;
    }

    int width, height;

    GetClientSize(&width,&height);

    RecalculatePositions(width,height);

    if ( was_shown )
        Show ( TRUE );
}

// -----------------------------------------------------------------------

wxPGId wxPropertyGridManager::DoGetPropertyByName ( wxPGNameStr name ) const
{
    return GetPropertyByName2(name, (wxPropertyGridState**)NULL );
}

// -----------------------------------------------------------------------

wxPGId wxPropertyGridManager::GetPropertyByName2 ( wxPGNameStr name, wxPropertyGridState** ppState ) const
{
    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        wxPropertyGridState* pState = &((wxPropertyGridPageData*)m_arrPages.Item(i))->m_state;
        wxPGId id = pState->BaseGetPropertyByName(name);
        if ( wxPGIdIsOk(id) )
        {
            if ( ppState ) *ppState = pState;
            return id;
        }
    }
    return wxPGIdGen((wxPGProperty*)NULL);
}

// -----------------------------------------------------------------------

wxPGId wxPropertyGridManager::GetPropertyByLabel ( const wxString& label, wxPropertyGridState** ppState ) const
{
    size_t i;
    for ( i=0; i<m_arrPages.GetCount(); i++ )
    {
        wxPropertyGridState* pState = &((wxPropertyGridPageData*)m_arrPages.Item(i))->m_state;
        wxPGId id = pState->GetPropertyByLabel(label);
        if ( wxPGIdIsOk(id) )
        {
            if ( ppState ) *ppState = pState;
            return id;
        }
    }
    return wxPGIdGen((wxPGProperty*)NULL);
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SelectProperty ( wxPGId id, bool focus )
{
    wxPGProperty* p = wxPGIdToPtr(id);
    wxASSERT ( p );
    wxPropertyGridState* parent_state = p->GetParentState();
    if ( parent_state == m_propGrid.m_pState )
        m_propGrid.SelectProperty(p,focus);
    else
        parent_state->m_selected = p;
}

// -----------------------------------------------------------------------

bool wxPropertyGridManager::EnsureVisible ( wxPGId id, wxPropertyGridState* parent_state )
{
    // Select correct page.
    if ( m_propGrid.m_pState != parent_state )
        SelectPage( GetPageByState(parent_state) );

    return m_propGrid.EnsureVisible(id);
}

// -----------------------------------------------------------------------

#define wxPG_IMPLEMENT_PGMAN_METHOD_WRET0(NAME,RETVAL) \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGId id ) \
{ \
    wxPGProperty* p = wxPGIdToPtr(id); \
    wxASSERT ( p ); \
    wxPropertyGridState* pState = p->GetParentState(); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id); \
    return pState->NAME(p); \
} \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGNameStr name ) \
{ \
    wxPropertyGridState* pState; \
    wxPGId id = GetPropertyByName2(name,&pState); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id); \
    return pState->NAME(wxPGIdToPtr(id)); \
}

#define wxPG_IMPLEMENT_PGMAN_METHOD_WRET0(NAME,RETVAL) \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGId id ) \
{ \
    wxPGProperty* p = wxPGIdToPtr(id); \
    wxASSERT ( p ); \
    wxPropertyGridState* pState = p->GetParentState(); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id); \
    return pState->NAME(p); \
} \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGNameStr name ) \
{ \
    wxPropertyGridState* pState; \
    wxPGId id = GetPropertyByName2(name,&pState); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id); \
    return pState->NAME(wxPGIdToPtr(id)); \
}

#define wxPG_IMPLEMENT_PGMAN_METHOD_WRET1(NAME,RETVAL,AT1) \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGId id, AT1 _av1_ ) \
{ \
    wxPGProperty* p = wxPGIdToPtr(id); \
    wxASSERT ( p ); \
    wxPropertyGridState* pState = p->GetParentState(); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id,_av1_); \
    return pState->NAME(p,_av1_); \
} \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGNameStr name, AT1 _av1_ ) \
{ \
    wxPropertyGridState* pState; \
    wxPGId id = GetPropertyByName2(name,&pState); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id,_av1_); \
    return pState->NAME(wxPGIdToPtr(id),_av1_); \
}

#define wxPG_IMPLEMENT_PGMAN_METHOD_WRET2(NAME,RETVAL,AT1,AT2) \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGId id, AT1 _av1_, AT2 _av2_ ) \
{ \
    wxPGProperty* p = wxPGIdToPtr(id); \
    wxASSERT ( p ); \
    wxPropertyGridState* pState = p->GetParentState(); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id,_av1_,_av2_); \
    return pState->NAME(p,_av1_,_av2_); \
} \
wxPG_IPAM_DECL RETVAL wxPropertyGridManager::NAME ( wxPGNameStr name, AT1 _av1_, AT2 _av2_ ) \
{ \
    wxPropertyGridState* pState; \
    wxPGId id = GetPropertyByName2(name,&pState); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) return m_propGrid.NAME(id,_av1_,_av2_); \
    return pState->NAME(wxPGIdToPtr(id),_av1_,_av2_); \
}

#define wxPG_IMPLEMENT_PGMAN_METHOD_NORET0(NAME) \
wxPG_IPAM_DECL void wxPropertyGridManager::NAME ( wxPGId id ) \
{ \
    wxPGProperty* p = wxPGIdToPtr(id); \
    wxASSERT ( p ); \
    wxPropertyGridState* pState = p->GetParentState(); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) m_propGrid.NAME(id); \
    else pState->NAME(p); \
} \
wxPG_IPAM_DECL void wxPropertyGridManager::NAME ( wxPGNameStr name ) \
{ \
    wxPropertyGridState* pState; \
    wxPGId id = GetPropertyByName2(name,&pState); \
    wxASSERT ( pState != (wxPropertyGridState*) NULL ); \
    if ( pState == m_propGrid.m_pState ) m_propGrid.NAME(id); \
    else pState->NAME(wxPGIdToPtr(id)); \
}

#undef wxPG_IPAM_DECL
#define wxPG_IPAM_DECL

wxPG_IMPLEMENT_PGMAN_METHOD_WRET0(ClearPropertyValue,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_WRET0(Collapse,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET0(Delete)
wxPG_IMPLEMENT_PGMAN_METHOD_WRET1(EnableProperty,bool,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_WRET0(Expand,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(LimitPropertyEditing,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyLabel,const wxString&)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyPriority,int)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,long)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,int)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,double)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,bool)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,const wxString&)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,const wxArrayString&)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,wxObject*)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,void*)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET1(SetPropertyValue,wxVariant&)
wxPG_IMPLEMENT_PGMAN_METHOD_NORET0(SetPropertyValueUnspecified)

// -----------------------------------------------------------------------

void wxPropertyGridManager::ClearModifiedStatus ( wxPGId id )
{
    wxPropertyGridState* pState = wxPGIdToPtr(id)->GetParentState();
    wxASSERT ( pState != (wxPropertyGridState*) NULL );
    pState->ClearModifiedStatus(wxPGIdToPtr(id));
}

// -----------------------------------------------------------------------

size_t wxPropertyGridManager::GetChildrenCount ( int page_index )
{
    return GetChildrenCount( wxPGIdGen(GetPage(page_index)->m_state.m_properties) );
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnToolbarClick ( wxCommandEvent &event )
{
    int id = event.GetId();
    if ( id >= 0 )
    {
        int base_id = m_propGrid.GetId();
        if ( id == ( base_id + ID_ADVTBITEMSBASE_OFFSET + 0 ) )
        {
            // Categorized mode.
            if ( m_propGrid.m_windowStyle & wxPG_HIDE_CATEGORIES )
                SetWindowStyleFlag ( m_windowStyle & ~(wxPG_HIDE_CATEGORIES) );
                //m_propGrid.EnableCategories ( TRUE );
        }
        else if ( id == ( base_id + ID_ADVTBITEMSBASE_OFFSET + 1 ) )
        {
            // Alphabetic mode.
            if ( !(m_propGrid.m_windowStyle & wxPG_HIDE_CATEGORIES) )
                SetWindowStyleFlag ( m_windowStyle | wxPG_HIDE_CATEGORIES );
                //m_propGrid.EnableCategories ( FALSE );
        }
        else
        {
            // Page Switching.

            int index = -1;
            size_t i;
            wxPropertyGridPageData* pdc;

            // Find page with given id.
            for ( i=0; i<m_arrPages.GetCount(); i++ )
            {
                pdc = (wxPropertyGridPageData*)m_arrPages.Item(i);
                if ( pdc->m_id == id )
                {
                    index = i;
                    break;
                }
            }

            wxASSERT ( index >= 0 );

            SelectPage ( index );

            // Event dispatching must be last.
            wxPropertyGridEvent evt( wxEVT_PG_PAGE_CHANGED, GetId() );
            evt.SetPropertyGrid (&m_propGrid);
            evt.SetEventObject (this);
            GetEventHandler()->AddPendingEvent(evt);

        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetDescription ( const wxString& label, const wxString& content )
{
    if ( m_pTxtHelpCaption )
    {
        wxSize osz1 = m_pTxtHelpCaption->GetSize();
        wxSize osz2 = m_pTxtHelpContent->GetSize();

        m_pTxtHelpCaption->SetLabel(label);
        m_pTxtHelpContent->SetLabel(content);

        m_pTxtHelpCaption->SetSize(-1,osz1.y);
        m_pTxtHelpContent->SetSize(-1,osz2.y);

        if ( (m_iFlags & wxPG_FL_DESC_REFRESH_REQUIRED) || (osz2.x<(m_width-10)) )
            RefreshHelpBox( m_splitterY, m_width, m_height );
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::SetDescribedProperty ( wxPGProperty* p )
{
    if ( m_pTxtHelpCaption )
    {
        if ( p )
            SetDescription( p->GetLabel(), p->GetHelpString() );
        else
        {
            m_pTxtHelpCaption->SetLabel(wxT(""));
            m_pTxtHelpContent->SetLabel(wxT(""));
        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnPropertyGridSelect ( wxPropertyGridEvent& event )
{
    // Check id.
    wxASSERT_MSG ( GetId() == m_propGrid.GetId(),
        wxT("wxPropertyGridManager id must be set with wxPropertyGridManager::SetId (not wxWindow::SetId).") );

    SetDescribedProperty(event.GetPropertyPtr());
    event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnCompactorClick ( wxCommandEvent& WXUNUSED(event) )
{
    if ( !(m_propGrid.m_iFlags & wxPG_FL_HIDE_STATE) )
    {
        m_propGrid.Compact( TRUE );
        m_pButCompactor->SetLabel( _("Expand >>") );
    }
    else
    {
        m_propGrid.Compact( FALSE );
        m_pButCompactor->SetLabel( _("<< Compact") );
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnResize ( wxSizeEvent& WXUNUSED(event) )
{

    int width, height;

    GetClientSize(&width,&height);

    if ( m_width == -12345 ) 
        RecreateControls();

    RecalculatePositions(width,height);

    //event.Skip();
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnMouseEntry ( wxMouseEvent& WXUNUSED(event) )
{
    //wxLogDebug(wxT("wxPropertyGridManager::OnMouseEntry"));

    // Correct cursor. This is required atleast for wxGTK, for which
    // setting button's cursor to *wxSTANDARD_CURSOR does not work.
    SetCursor( wxNullCursor );
    m_onSplitter = 0;
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnMouseMove ( wxMouseEvent &event )
{
    if ( !m_pTxtHelpCaption )
        return;

    int y = event.m_y;

    if ( m_dragStatus > 0 )
    {
        int sy = y - m_dragOffset;

        // Calculate drag limits
        int bottom_limit = m_height - m_splitterHeight + 1;
        if ( m_pButCompactor ) bottom_limit -= m_pButCompactor->GetSize().y;
        int top_limit = m_propGrid.m_lineHeight;
#if wxUSE_TOOLBAR
        if ( m_pToolbar ) top_limit += m_pToolbar->GetSize().y;
#endif

        if ( sy >= top_limit && sy < bottom_limit )
        {

            //wxLogDebug (wxT("...dragging continues..."));

            int change = sy - m_splitterY;
            if ( change )
            {
                m_splitterY = sy;

                m_propGrid.SetSize( m_width, m_splitterY - m_propGrid.GetPosition().y );
                RefreshHelpBox( m_splitterY, m_width, m_height );

                m_extraHeight -= change;
#if wxMINOR_VERSION > 5 || ( wxMINOR_VERSION == 5 && wxRELEASE_NUMBER >= 3 )
                InvalidateBestSize();
#endif
            }

        }

    }
    else
    {
        if ( y >= m_splitterY && y < (m_splitterY+m_splitterHeight+2) )
        {
            //if ( !m_onSplitter )
                SetCursor ( m_cursorSizeNS );
            m_onSplitter = 1;
        }
        else
        {
            if ( m_onSplitter )
            {
                //wxLogDebug(wxT("SetCursor ( *wxSTANDARD_CURSOR )"));
                //SetCursor ( *wxSTANDARD_CURSOR );
                SetCursor ( wxNullCursor );
            }
            m_onSplitter = 0;
        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnMouseClick ( wxMouseEvent &event )
{
    int y = event.m_y;

    // Click on splitter.
    if ( y >= m_splitterY && y < (m_splitterY+m_splitterHeight+2) )
    {

        /*
        if ( event.m_eventType == wxEVT_LEFT_DCLICK )
        {
            // Double-clicking the splitter causes auto-centering
            CenterSplitter ( TRUE );
        }
        else
        */
        if ( m_dragStatus == 0 )
        {
        //
        // Begin draggin the splitter
        //

            BEGIN_MOUSE_CAPTURE

            m_dragStatus = 1;

            m_dragOffset = y - m_splitterY;

            /*
            Update(); // clear graphics mess
            DrawSplitterDragRow( this, m_splitterY, m_splitterHeight );
            m_splitterPrevDrawnWin = this;
            */

        }
    }
}

// -----------------------------------------------------------------------

void wxPropertyGridManager::OnMouseUp ( wxMouseEvent &event )
{
    // No event type check - basicly calling this method should
    // just stop dragging.
    // Left up after dragged?

    if ( m_dragStatus >= 1 )
    {
        int y = event.m_y;
    //
    // End Splitter Dragging
    //

        // DO NOT ENABLE FOLLOWING LINE!
        // (it is only here as a reminder to not to do it)
        //m_splitterY = y;

        //if ( m_splitterPrevDrawnWin )
        //    DrawSplitterDragRow( m_splitterPrevDrawnWin, m_splitterY, m_splitterHeight );

        // This is necessary to return cursor
        END_MOUSE_CAPTURE

        // Set back the default cursor, if necessary
        if ( y < m_splitterY || y >= (m_splitterY+m_splitterHeight+2) )
        {
            //SetCursor ( *wxSTANDARD_CURSOR );
            SetCursor ( wxNullCursor );
        }


        m_dragStatus = 0;
    }
}

// -----------------------------------------------------------------------
