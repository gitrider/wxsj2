///////////////////////////////////////////////////////////////////////////////
// Name:        dnd.cpp
// Purpose:     wxDropTarget, wxDropSource classes
// Author:      Julian Smart
// Id:          $Id: dnd.cpp,v 1.5 2004/05/23 20:53:30 JS Exp $
// Copyright:   (c) 1998 Julian Smart
// Licence:   	wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dnd.h"
#endif

#include "wx/setup.h"

#if wxUSE_DRAG_AND_DROP

#include "wx/dnd.h"
#include "wx/window.h"
#include "wx/app.h"
#include "wx/gdicmn.h"
#include "wx/intl.h"
#include "wx/utils.h"
#include "wx/log.h"

#include <X11/Xlib.h>

// ----------------------------------------------------------------------------
// global
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// wxDropTarget
// ----------------------------------------------------------------------------

wxDropTarget::wxDropTarget()
{
}

wxDropTarget::~wxDropTarget()
{
}

// ----------------------------------------------------------------------------
// wxTextDropTarget
// ----------------------------------------------------------------------------

bool wxTextDropTarget::OnDrop( long x, long y, const void *data, size_t WXUNUSED(size) )
{
  OnDropText( x, y, (const char*)data );
  return TRUE;
}

bool wxTextDropTarget::OnDropText( long x, long y, const char *psz )
{
  wxLogDebug( "Got dropped text: %s.", psz );
  wxLogDebug( "At x: %d, y: %d.", (int)x, (int)y );
  return TRUE;
}

size_t wxTextDropTarget::GetFormatCount() const
{
  return 1;
}

wxDataFormat wxTextDropTarget::GetFormat(size_t WXUNUSED(n)) const
{
  return wxDF_TEXT;
}

// ----------------------------------------------------------------------------
// wxFileDropTarget
// ----------------------------------------------------------------------------

bool wxFileDropTarget::OnDropFiles( long x, long y, size_t nFiles, const char * const aszFiles[] )
{
  wxLogDebug( "Got %d dropped files.", (int)nFiles );
  wxLogDebug( "At x: %d, y: %d.", (int)x, (int)y );
  size_t i;
  for (i = 0; i < nFiles; i++)
  {
    wxLogDebug( aszFiles[i] );
  }
  return TRUE;
}

bool wxFileDropTarget::OnDrop(long x, long y, const void *data, size_t size )
{
  size_t number = 0;
  char *text = (char*) data;
  size_t i;
  for (i = 0; i < size; i++)
    if (text[i] == 0) number++;

  if (number == 0) return TRUE;    
    
  char **files = new char*[number];
  
  text = (char*) data;
  for ( i = 0; i < number; i++)
  {
    files[i] = text;
    int len = strlen( text );
    text += len+1;
  }

  bool ret = OnDropFiles( x, y, 1, files ); 
  
  free( files );
  
  return ret;
}

size_t wxFileDropTarget::GetFormatCount() const
{
  return 1;
}

wxDataFormat wxFileDropTarget::GetFormat(size_t WXUNUSED(n)) const
{
  return wxDF_FILENAME;
}

//-------------------------------------------------------------------------
// wxDropSource
//-------------------------------------------------------------------------

wxDropSource::wxDropSource( wxWindow *win )
{
#if 0
  m_window = win;
  m_data = (wxDataObject *) NULL;
  m_retValue = wxDragCancel;

  m_defaultCursor = wxCursor( wxCURSOR_NO_ENTRY );
  m_goaheadCursor = wxCursor( wxCURSOR_HAND );
#endif
}

wxDropSource::wxDropSource( wxDataObject &data, wxWindow *win )
{
#if 0
  g_blockEventsOnDrag = TRUE;
  
  m_window = win;
  m_widget = win->m_widget;
  if (win->m_wxwindow) m_widget = win->m_wxwindow;
  m_retValue = wxDragCancel;
  
  m_data = &data;

  m_defaultCursor = wxCursor( wxCURSOR_NO_ENTRY );
  m_goaheadCursor = wxCursor( wxCURSOR_HAND );
#endif
}

void wxDropSource::SetData( wxDataObject &data )
{
//  m_data = &data;
}

wxDropSource::~wxDropSource(void)
{
//  if (m_data) delete m_data;
}
   
wxDragResult wxDropSource::DoDragDrop( int WXUNUSED(flags) )
{
  //  wxASSERT_MSG( m_data, "wxDragSource: no data" );

  return wxDragNone;
#if 0
  if (!m_data) return (wxDragResult) wxDragNone;
  if (m_data->GetDataSize() == 0) return (wxDragResult) wxDragNone;
  
  RegisterWindow();
  
  // TODO
  
  UnregisterWindow();
  
  g_blockEventsOnDrag = FALSE;
  
  return m_retValue;
#endif
}

#if 0
void wxDropSource::RegisterWindow(void)
{
  if (!m_data) return;

  wxString formats;
    
  wxDataFormat df = m_data->GetPreferredFormat();
  
    switch (df) 
    {
      case wxDF_TEXT: 
        formats += "text/plain";
	break;
      case wxDF_FILENAME:
        formats += "file:ALL";
	break;
      default:
        break;
    }
  
  char *str = WXSTRINGCAST formats;
  
  // TODO
}

void wxDropSource::UnregisterWindow(void)
{
  if (!m_widget) return;
  
  // TODO
}
#endif

wxPrivateDropTarget::wxPrivateDropTarget()
{
    m_id = wxTheApp->GetAppName();
}
  
size_t wxPrivateDropTarget::GetFormatCount() const
{
    return 1;
}

wxDataFormat wxPrivateDropTarget::GetFormat(size_t n) const
{
    return wxDF_INVALID;
}

#endif
      // wxUSE_DRAG_AND_DROP
