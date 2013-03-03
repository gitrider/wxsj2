/////////////////////////////////////////////////////////////////////////////
// Name:        toolbar.cpp
// Purpose:     wxToolBar
// Author:      Stefan Csomor
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: toolbar.cpp,v 1.81 2005/05/10 06:26:59 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     The wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "toolbar.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_TOOLBAR

#include "wx/wx.h"
#include "wx/toolbar.h"
#include "wx/notebook.h"
#include "wx/tabctrl.h"
#include "wx/bitmap.h"

IMPLEMENT_DYNAMIC_CLASS(wxToolBar, wxControl)

BEGIN_EVENT_TABLE(wxToolBar, wxToolBarBase)
    EVT_PAINT( wxToolBar::OnPaint ) 
END_EVENT_TABLE()

#include "wx/mac/uma.h"
#include "wx/geometry.h"

#ifdef __WXMAC_OSX__
const short kwxMacToolBarToolDefaultWidth = 16 ;
const short kwxMacToolBarToolDefaultHeight = 16 ;
const short kwxMacToolBarTopMargin = 4 ;
const short kwxMacToolBarLeftMargin = 4 ;
const short kwxMacToolBorder = 0 ;
const short kwxMacToolSpacing = 6 ;
#else
const short kwxMacToolBarToolDefaultWidth = 24 ;
const short kwxMacToolBarToolDefaultHeight = 22 ;
const short kwxMacToolBarTopMargin = 2 ;
const short kwxMacToolBarLeftMargin = 2 ;
const short kwxMacToolBorder = 4 ;
const short kwxMacToolSpacing = 0 ;
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class wxToolBarTool : public wxToolBarToolBase
{
public:
    wxToolBarTool(wxToolBar *tbar,
                  int id,
                  const wxString& label,
                  const wxBitmap& bmpNormal,
                  const wxBitmap& bmpDisabled,
                  wxItemKind kind,
                  wxObject *clientData,
                  const wxString& shortHelp,
                  const wxString& longHelp) ;
                  
    wxToolBarTool(wxToolBar *tbar, wxControl *control)
        : wxToolBarToolBase(tbar, control)
    {
        Init() ;
    }
    
    ~wxToolBarTool()
    {
        if ( m_controlHandle )
            DisposeControl( m_controlHandle ) ;
    }
    
    WXWidget GetControlHandle() { return (WXWidget) m_controlHandle ; }
    void SetControlHandle( ControlRef handle ) { m_controlHandle = handle ; }

    void SetSize(const wxSize& size) ;
    void SetPosition( const wxPoint& position ) ;
    
    void ClearControl() { m_control = NULL ; }
    
    wxSize GetSize() const
    {
        if ( IsControl() )
        {
            return GetControl()->GetSize() ;
        }
        else if ( IsButton() )
        {
            return GetToolBar()->GetToolSize() ;
        }
        else
        {
            // separator size
            wxSize sz = GetToolBar()->GetToolSize() ;
            if ( GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL )
                sz.y /= 4 ;
            else
                sz.x /= 4 ;
            return sz ;
        }
    }
    wxPoint GetPosition() const
    {
        return wxPoint(m_x, m_y);
    }    
    bool DoEnable( bool enable ) ;
    
    void UpdateToggleImage( bool toggle ) ;
private :
    void Init() 
    {
        m_controlHandle = NULL ;
    }
    ControlRef m_controlHandle ;

    wxCoord     m_x;
    wxCoord     m_y;
};

static const EventTypeSpec eventList[] =
{
	{ kEventClassControl , kEventControlHit } ,
#ifdef __WXMAC_OSX__
	{ kEventClassControl , kEventControlHitTest } ,
#endif
} ;

static pascal OSStatus wxMacToolBarToolControlEventHandler( EventHandlerCallRef handler , EventRef event , void *data )
{
    OSStatus result = eventNotHandledErr ;

    wxMacCarbonEvent cEvent( event ) ;
    
    ControlRef controlRef ;

    cEvent.GetParameter( kEventParamDirectObject , &controlRef ) ;

    switch( GetEventKind( event ) )
    {
        case kEventControlHit :
            {
                wxToolBarTool* tbartool = (wxToolBarTool*)data ;
                if ( tbartool->CanBeToggled() )
                {
#ifdef __WXMAC_OSX__
                    ((wxToolBar*)tbartool->GetToolBar())->ToggleTool(tbartool->GetId(), !tbartool->IsToggled() );
#else
                    ((wxToolBar*)tbartool->GetToolBar())->ToggleTool(tbartool->GetId(), GetControl32BitValue((ControlRef)tbartool->GetControlHandle()));
#endif
                }
                ((wxToolBar*)tbartool->GetToolBar())->OnLeftClick( tbartool->GetId() , tbartool -> IsToggled() ) ;
                result = noErr; 
            }
            break ;
#ifdef __WXMAC_OSX__
        case kEventControlHitTest :
            {
                HIPoint pt = cEvent.GetParameter<HIPoint>(kEventParamMouseLocation) ;
                HIRect rect ;
                HIViewGetBounds( controlRef , &rect ) ;
                
                ControlPartCode pc = kControlNoPart ;
                if ( CGRectContainsPoint( rect , pt ) )
                    pc = kControlIconPart ;
                cEvent.SetParameter( kEventParamControlPart , typeControlPartCode, pc ) ;
                result = noErr ;
            }
            break ;
#endif
        default :
            break ;
    }
    return result ;
}

static pascal OSStatus wxMacToolBarToolEventHandler( EventHandlerCallRef handler , EventRef event , void *data )
{
    OSStatus result = eventNotHandledErr ;

    switch ( GetEventClass( event ) )
    {
        case kEventClassControl :
            result = wxMacToolBarToolControlEventHandler( handler, event, data ) ;
            break ;
        default :
            break ;
    }
    return result ;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( wxMacToolBarToolEventHandler )

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxToolBarTool
// ----------------------------------------------------------------------------

bool wxToolBarTool::DoEnable(bool enable)
{
    if ( IsControl() )
    {
        GetControl()->Enable( enable ) ;
    }
    else if ( IsButton() )
    {
#if TARGET_API_MAC_OSX
        if ( enable )
            EnableControl( m_controlHandle ) ;
        else
            DisableControl( m_controlHandle ) ;
#else
        if ( enable )
            ActivateControl( m_controlHandle ) ;
        else
            DeactivateControl( m_controlHandle ) ;
#endif
    }
    return true ;
}
void wxToolBarTool::SetSize(const wxSize& size)
{
    if ( IsControl() )
    {
        GetControl()->SetSize( size ) ;
    }
}

void wxToolBarTool::SetPosition(const wxPoint& position)
{
    m_x = position.x;
    m_y = position.y;

    int x , y ;
    x = y = 0 ;
    int mac_x = position.x ;
    int mac_y = position.y ;

    if ( ! GetToolBar()->MacGetTopLevelWindow()->MacUsesCompositing() )
    {
        GetToolBar()->MacWindowToRootWindow( &x , &y ) ;
        mac_x += x;
        mac_y += y;
    }

    if ( IsButton() )
    {
        Rect contrlRect ;       
        GetControlBounds( m_controlHandle , &contrlRect ) ; 
        int former_mac_x = contrlRect.left ;
        int former_mac_y = contrlRect.top ;
        GetToolBar()->GetToolSize() ;
        
        if ( mac_x != former_mac_x || mac_y != former_mac_y )
        {
            UMAMoveControl( m_controlHandle , mac_x , mac_y ) ;
        }
    }
    else if ( IsControl() )
    {
        GetControl()->Move( position ) ;
    }
    else
    {
        // separator 
#ifdef __WXMAC_OSX__
        Rect contrlRect ;       
        GetControlBounds( m_controlHandle , &contrlRect ) ; 
        int former_mac_x = contrlRect.left ;
        int former_mac_y = contrlRect.top ;
        
        if ( mac_x != former_mac_x || mac_y != former_mac_y )
        {
            UMAMoveControl( m_controlHandle , mac_x , mac_y ) ;
        }
#endif
    }
}

void wxToolBarTool::UpdateToggleImage( bool toggle ) 
{
#ifdef __WXMAC_OSX__
    if ( toggle )
    {
        int w = m_bmpNormal.GetWidth() ;
        int h = m_bmpNormal.GetHeight() ;
        wxBitmap bmp( w , h ) ;
        wxMemoryDC dc ;
        dc.SelectObject( bmp ) ;
        dc.SetPen( wxNullPen ) ;
        dc.SetBackground( *wxWHITE ) ;
        dc.DrawRectangle( 0 , 0 , w , h ) ;
        dc.DrawBitmap( m_bmpNormal , 0 , 0 , true) ;
        dc.SelectObject( wxNullBitmap ) ;
        ControlButtonContentInfo info ;
        wxMacCreateBitmapButton( &info , bmp ) ;
    	SetControlData( m_controlHandle , 0, kControlIconContentTag, sizeof( info ),
    			(Ptr)&info );
        wxMacReleaseBitmapButton( &info ) ;
    }
    else
    {
        ControlButtonContentInfo info ;
        wxMacCreateBitmapButton( &info , m_bmpNormal ) ;
    	SetControlData( m_controlHandle , 0, kControlIconContentTag, sizeof( info ),
    			(Ptr)&info );
        wxMacReleaseBitmapButton( &info ) ;
    }

    IconTransformType transform = toggle ? kTransformSelected : kTransformNone ;
	SetControlData( m_controlHandle, 0, kControlIconTransformTag, sizeof( transform ),
			(Ptr)&transform );
    HIViewSetNeedsDisplay( m_controlHandle , true ) ;

#else
    ::SetControl32BitValue( m_controlHandle , toggle ) ;
#endif
}

wxToolBarTool::wxToolBarTool(wxToolBar *tbar,
  int id,
  const wxString& label,
  const wxBitmap& bmpNormal,
  const wxBitmap& bmpDisabled,
  wxItemKind kind,
  wxObject *clientData,
  const wxString& shortHelp,
  const wxString& longHelp)
        : wxToolBarToolBase(tbar, id, label, bmpNormal, bmpDisabled, kind,
                            clientData, shortHelp, longHelp)
{
    Init();
}


wxToolBarToolBase *wxToolBar::CreateTool(int id,
                                         const wxString& label,
                                         const wxBitmap& bmpNormal,
                                         const wxBitmap& bmpDisabled,
                                         wxItemKind kind,
                                         wxObject *clientData,
                                         const wxString& shortHelp,
                                         const wxString& longHelp)
{
    return new wxToolBarTool(this, id, label, bmpNormal, bmpDisabled, kind,
                             clientData, shortHelp, longHelp);
}

wxToolBarToolBase *wxToolBar::CreateTool(wxControl *control)
{
    return new wxToolBarTool(this, control);
}

void wxToolBar::Init()
{
    m_maxWidth = -1;
    m_maxHeight = -1;
    m_defaultWidth = kwxMacToolBarToolDefaultWidth;
    m_defaultHeight = kwxMacToolBarToolDefaultHeight;
}

bool wxToolBar::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
            long style, const wxString& name)
{  

    if ( !wxToolBarBase::Create( parent , id , pos , size , style ) )
        return FALSE ;
    
    return TRUE;
}

wxToolBar::~wxToolBar()
{    
    // we must refresh the frame size when the toolbar is deleted but the frame
    // is not - otherwise toolbar leaves a hole in the place it used to occupy
}

bool wxToolBar::Realize()
{
    if (m_tools.GetCount() == 0)
        return FALSE;

    int x = m_xMargin + kwxMacToolBarLeftMargin ;
    int y = m_yMargin + kwxMacToolBarTopMargin ;

    int tw, th;
    GetSize(& tw, & th);
    
    int maxWidth = 0 ;
    int maxHeight = 0 ;
    
    int maxToolWidth = 0;
    int maxToolHeight = 0;

    // Find the maximum tool width and height
    wxToolBarToolsList::compatibility_iterator node = m_tools.GetFirst();
    while ( node )
    {
        wxToolBarTool *tool = (wxToolBarTool *)node->GetData();
        wxSize sz = tool->GetSize() ;

        if ( sz.x > maxToolWidth )
            maxToolWidth = sz.x ;
        if (sz.y> maxToolHeight)
            maxToolHeight = sz.y;

        node = node->GetNext();
    }

    bool lastWasRadio = FALSE;
    node = m_tools.GetFirst();
    while (node)
    {
        wxToolBarTool *tool = (wxToolBarTool *)node->GetData();
        wxSize cursize = tool->GetSize() ;
        
        bool isRadio = FALSE;

        if ( tool->IsButton() && tool->GetKind() == wxITEM_RADIO )
        {
            if ( !lastWasRadio )
            {
                if (tool->Toggle(true))
                {
                    DoToggleTool(tool, true);
                }
            }
            else if (tool->IsToggled())
            {
                wxToolBarToolsList::compatibility_iterator nodePrev = node->GetPrevious();
                while ( nodePrev )
                {
                    wxToolBarToolBase *tool = nodePrev->GetData();
                    if ( !tool->IsButton() || (tool->GetKind() != wxITEM_RADIO) )
                        break;
                    if ( tool->Toggle(false) )
                    {
                        DoToggleTool(tool, false);
                    }
                    nodePrev = nodePrev->GetPrevious();
                }
            }
            isRadio = TRUE;
        }
        else
        {
            isRadio = FALSE;
        }
        lastWasRadio = isRadio;

        // for the moment we just do a single row/column alignement
        if ( x + cursize.x > maxWidth )
            maxWidth = x + cursize.x ;
        if ( y + cursize.y > maxHeight )
            maxHeight = y + cursize.y ;
            
        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            int x1 = x + (maxToolWidth - cursize.x)/2 ;
            tool->SetPosition( wxPoint( x1 , y ) ) ;
        }
        else
        {
            int y1 = y + (maxToolHeight - cursize.y)/2 ;
            tool->SetPosition( wxPoint( x , y1 ) ) ;
        }    
        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            y += cursize.y ;
            y += kwxMacToolSpacing ;
        }
        else
        {
            x += cursize.x ;
            x += kwxMacToolSpacing ;
        }

        node = node->GetNext();
    }
    
    if ( GetWindowStyleFlag() & wxTB_HORIZONTAL )
    {
        if ( m_maxRows == 0 )
        {
            // if not set yet, only one row
            SetRows(1);
        }
        m_minWidth = maxWidth;
        maxWidth = tw ; 
        maxHeight += m_yMargin + kwxMacToolBarTopMargin;
        m_minHeight = m_maxHeight = maxHeight ;
    }
    else
    {
        if ( GetToolsCount() > 0 && m_maxRows == 0 )
        {
            // if not set yet, have one column
            SetRows(GetToolsCount());
        }
        m_minHeight = maxHeight;
        maxHeight = th ;
        maxWidth += m_xMargin + kwxMacToolBarLeftMargin;
        m_minWidth = m_maxWidth = maxWidth ;
    }
    
    SetSize( maxWidth, maxHeight );
    InvalidateBestSize();
    
    return TRUE;
}

void wxToolBar::SetToolBitmapSize(const wxSize& size)
{
    m_defaultWidth = size.x+kwxMacToolBorder; m_defaultHeight = size.y+kwxMacToolBorder;
}

// The button size is bigger than the bitmap size
wxSize wxToolBar::GetToolSize() const
{
    return wxSize(m_defaultWidth + kwxMacToolBorder, m_defaultHeight + kwxMacToolBorder);
}

void wxToolBar::SetRows(int nRows)
{
    if ( nRows == m_maxRows )
    {
        // avoid resizing the frame uselessly
        return;
    }

    m_maxRows = nRows;
}

void wxToolBar::MacSuperChangedPosition() 
{
    wxWindow::MacSuperChangedPosition() ;
    Realize() ;
}

wxToolBarToolBase *wxToolBar::FindToolForPosition(wxCoord x, wxCoord y) const
{
    wxToolBarToolsList::compatibility_iterator node = m_tools.GetFirst();
    while (node)
    {
        wxToolBarTool *tool = (wxToolBarTool *)node->GetData() ;
        wxRect2DInt r( tool->GetPosition() , tool->GetSize() ) ;
        if ( r.Contains( wxPoint( x , y ) ) )
        {
            return tool;
        }

        node = node->GetNext();
    }

    return (wxToolBarToolBase *)NULL;
}

wxString wxToolBar::MacGetToolTipString( wxPoint &pt )
{
    wxToolBarToolBase* tool = FindToolForPosition( pt.x , pt.y ) ;
    if ( tool )
    {
        return tool->GetShortHelp() ;
    }
    return wxEmptyString ;
}

void wxToolBar::DoEnableTool(wxToolBarToolBase *t, bool enable)
{
    ((wxToolBarTool*)t)->DoEnable( enable ) ;
}

void wxToolBar::DoToggleTool(wxToolBarToolBase *t, bool toggle)
{
    wxToolBarTool *tool = (wxToolBarTool *)t;
    if ( tool->IsButton() )
    {
        tool->UpdateToggleImage( toggle ) ;
    }
}

bool wxToolBar::DoInsertTool(size_t WXUNUSED(pos),
                             wxToolBarToolBase *toolBase)
{
    wxToolBarTool* tool = wx_static_cast( wxToolBarTool* , toolBase ) ;
    
    WindowRef window = (WindowRef) MacGetTopLevelWindowRef() ;    
    wxSize toolSize = GetToolSize() ;    
    Rect toolrect = { 0, 0 , toolSize.y , toolSize.x } ;
    ControlRef controlHandle = NULL ;

    switch( tool->GetStyle() )
    {
        case wxTOOL_STYLE_SEPARATOR :
            {
                wxASSERT( tool->GetControlHandle() == NULL ) ;
                toolSize.x /= 4 ;
                toolSize.y /= 4 ;
                if ( GetWindowStyleFlag() & wxTB_VERTICAL )
                {
                    toolrect.bottom = toolSize.y ;
                }
                else
                {
                    toolrect.right = toolSize.x ;
                }
        #ifdef __WXMAC_OSX__
                // in flat style we need a visual separator
                CreateSeparatorControl( window , &toolrect , &controlHandle ) ;
                tool->SetControlHandle( controlHandle ) ;
        #endif
            }
            break ;
        case wxTOOL_STYLE_BUTTON :
            {
                wxASSERT( tool->GetControlHandle() == NULL ) ;
                ControlButtonContentInfo info ;
                wxMacCreateBitmapButton( &info , tool->GetNormalBitmap()  , kControlContentIconRef ) ;
                
        #ifdef __WXMAC_OSX__
                CreateIconControl( window , &toolrect , &info , false , &controlHandle ) ;
        #else        
                SInt16 behaviour = kControlBehaviorOffsetContents ;
                if ( tool->CanBeToggled() )
                    behaviour += kControlBehaviorToggles ;
                CreateBevelButtonControl( window , &toolrect , CFSTR("") , kControlBevelButtonNormalBevel , behaviour , &info , 
                    0 , 0 , 0 , &controlHandle ) ;
        #endif
                    
                wxMacReleaseBitmapButton( &info ) ;
                /*
                SetBevelButtonTextPlacement( m_controlHandle , kControlBevelButtonPlaceBelowGraphic ) ;
                UMASetControlTitle(  m_controlHandle , label , wxFont::GetDefaultEncoding() ) ;
                */
                
                InstallControlEventHandler( (ControlRef) controlHandle, GetwxMacToolBarToolEventHandlerUPP(),
                    GetEventTypeCount(eventList), eventList, tool,NULL);          

                tool->SetControlHandle( controlHandle ) ;
            }
            break ;
        case wxTOOL_STYLE_CONTROL :
            wxASSERT( tool->GetControl() != NULL ) ;
            // right now there's nothing to do here
            break ;
    }
 
    if ( controlHandle )
    {
        ControlRef container = (ControlRef) GetHandle() ;
        wxASSERT_MSG( container != NULL , wxT("No valid mac container control") ) ;

        UMAShowControl( controlHandle ) ;
        ::EmbedControl( controlHandle , container ) ;
    }

    if ( tool->CanBeToggled() && tool->IsToggled() )
    {
        tool->UpdateToggleImage( true ) ;
    }

    // nothing special to do here - we relayout in Realize() later
    tool->Attach(this);
    InvalidateBestSize();

    return TRUE;
}

void wxToolBar::DoSetToggle(wxToolBarToolBase *WXUNUSED(tool), bool WXUNUSED(toggle))
{
    wxFAIL_MSG( _T("not implemented") );
}

bool wxToolBar::DoDeleteTool(size_t WXUNUSED(pos), wxToolBarToolBase *toolbase)
{
    wxToolBarTool* tool = wx_static_cast( wxToolBarTool* , toolbase ) ;
    wxToolBarToolsList::compatibility_iterator node;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool2 = node->GetData();
        if ( tool2 == tool )
        {
            // let node point to the next node in the list
            node = node->GetNext();

            break;
        }
    }

    wxSize sz = ((wxToolBarTool*)tool)->GetSize() ;

    tool->Detach();

    switch ( tool->GetStyle() )
    {
        case wxTOOL_STYLE_CONTROL:
            {
                tool->GetControl()->Destroy();
                tool->ClearControl() ; 
            }
            break;

        case wxTOOL_STYLE_BUTTON:
        case wxTOOL_STYLE_SEPARATOR:
            if ( tool->GetControlHandle() )
            {
                DisposeControl( (ControlRef) tool->GetControlHandle() ) ;
                tool->SetControlHandle( (ControlRef) NULL ) ;
            }
            break;
    }

    // and finally reposition all the controls after this one
    
    for ( /* node -> first after deleted */ ; node; node = node->GetNext() )
    {
        wxToolBarTool *tool2 = (wxToolBarTool*) node->GetData();
        wxPoint pt = tool2->GetPosition() ;

        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            pt.y -= sz.y ;
        }
        else
        {
            pt.x -= sz.x ;
        }
        tool2->SetPosition( pt ) ;
    }
    
    InvalidateBestSize();
    return TRUE ;
}

void wxToolBar::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this) ;

    int w, h ;
    GetSize( &w , &h ) ;
#if wxMAC_USE_CORE_GRAPHICS && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3
    if ( !MacGetTopLevelWindow()->MacGetMetalAppearance() )
    {
        if ( UMAGetSystemVersion() >= 0x1030 )
        {
            HIThemePlacardDrawInfo info ;
            memset( &info, 0 , sizeof( info ) ) ;
            info.version = 0 ;
            info.state = IsEnabled() ? kThemeStateActive : kThemeStateInactive ;
            
            CGContextRef cgContext = (CGContextRef) MacGetCGContextRef() ;
            HIRect rect = CGRectMake( 0 , 0 , w , h ) ;
            HIThemeDrawPlacard( &rect , & info , cgContext, kHIThemeOrientationNormal) ;
        }
    }
    else
    {
        // leave the background as it is (striped or metal)
    }
#else
    wxMacPortSetter helper(&dc) ;
    
    Rect toolbarrect = { dc.YLOG2DEVMAC(0) , dc.XLOG2DEVMAC(0) , 
        dc.YLOG2DEVMAC(h) , dc.XLOG2DEVMAC(w) } ;
/*
    if( toolbarrect.left < 0 )
        toolbarrect.left = 0 ;
    if ( toolbarrect.top < 0 )
        toolbarrect.top = 0 ;
*/
    if ( !MacGetTopLevelWindow()->MacGetMetalAppearance() )
    {
        UMADrawThemePlacard( &toolbarrect , IsEnabled() ? kThemeStateActive : kThemeStateInactive) ;
    }
    else
    {
#if TARGET_API_MAC_OSX
#if MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_2
        if ( UMAGetSystemVersion() >= 0x1030 )
        {
            HIRect hiToolbarrect = CGRectMake( dc.YLOG2DEVMAC(0) , dc.XLOG2DEVMAC(0) , 
            dc.YLOG2DEVREL(h) , dc.XLOG2DEVREL(w) );
            CGContextRef cgContext ;
            Rect bounds ;
            GetPortBounds( (CGrafPtr) dc.m_macPort , &bounds ) ;
            QDBeginCGContext( (CGrafPtr) dc.m_macPort , &cgContext ) ;
            CGContextTranslateCTM( cgContext , 0 , bounds.bottom - bounds.top ) ;
            CGContextScaleCTM( cgContext , 1 , -1 ) ;

            {
                HIThemeBackgroundDrawInfo drawInfo ;
                drawInfo.version = 0 ;
                drawInfo.state = kThemeStateActive ;
                drawInfo.kind = kThemeBackgroundMetal ;
                HIThemeApplyBackground( &hiToolbarrect, &drawInfo , cgContext,kHIThemeOrientationNormal) ;
            }
            QDEndCGContext( (CGrafPtr) dc.m_macPort , &cgContext ) ;
        }
        else
#endif
        {
            UMADrawThemePlacard( &toolbarrect , IsEnabled() ? kThemeStateActive : kThemeStateInactive) ;
        }
#endif
    }
#endif

    event.Skip() ;
}

#endif // wxUSE_TOOLBAR

