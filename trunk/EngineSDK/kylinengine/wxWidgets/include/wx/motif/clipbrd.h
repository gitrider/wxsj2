/////////////////////////////////////////////////////////////////////////////
// Name:        clipbrd.h
// Purpose:     Clipboard functionality.
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: clipbrd.h,v 1.17 2004/05/23 20:51:21 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CLIPBRD_H_
#define _WX_CLIPBRD_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "clipbrd.h"
#endif

#if wxUSE_CLIPBOARD

class wxDataObject;
struct wxDataIdToDataObject;

#include "wx/list.h"

WX_DECLARE_LIST(wxDataObject, wxDataObjectList);
WX_DECLARE_LIST(wxDataIdToDataObject, wxDataIdToDataObjectList);

bool WXDLLEXPORT wxOpenClipboard();
bool WXDLLEXPORT wxClipboardOpen();
bool WXDLLEXPORT wxCloseClipboard();
bool WXDLLEXPORT wxEmptyClipboard();
bool WXDLLEXPORT wxIsClipboardFormatAvailable(wxDataFormat dataFormat);
bool WXDLLEXPORT wxSetClipboardData(wxDataFormat dataFormat, wxObject *obj, int width = 0, int height = 0);
wxObject* WXDLLEXPORT wxGetClipboardData(wxDataFormat dataFormat, long *len = NULL);
wxDataFormat WXDLLEXPORT wxEnumClipboardFormats(wxDataFormat dataFormat);
wxDataFormat WXDLLEXPORT wxRegisterClipboardFormat(char *formatName);
bool WXDLLEXPORT wxGetClipboardFormatName(wxDataFormat dataFormat, char *formatName, int maxCount);

//-----------------------------------------------------------------------------
// wxClipboard
//-----------------------------------------------------------------------------

class wxClipboard : public wxClipboardBase
{
public:
    wxClipboard();
    ~wxClipboard();
    
    // open the clipboard before SetData() and GetData()
    virtual bool Open();
    
    // close the clipboard after SetData() and GetData()
    virtual void Close();
    
    // opened?
    virtual bool IsOpened() const { return m_open; }
    
    // replaces the data on the clipboard with data
    virtual bool SetData( wxDataObject *data );
    
    // adds data to the clipboard
    virtual bool AddData( wxDataObject *data );
    
    // format available on the clipboard ?
    virtual bool IsSupported( const wxDataFormat& format );
    
    // fill data with data on the clipboard (if available)
    virtual bool GetData( wxDataObject& data );
    
    // clears wxTheClipboard and the system's clipboard if possible
    virtual void Clear();
    
    virtual void UsePrimarySelection(bool primary = true)
    { m_usePrimary = primary; }
    
    // implementation from now on
    bool              m_open;
    wxDataObjectList  m_data;
    bool              m_usePrimary;
    wxDataIdToDataObjectList m_idToObject;

private:
    DECLARE_DYNAMIC_CLASS(wxClipboard)
};

#endif // wxUSE_CLIPBOARD

#endif
// _WX_CLIPBRD_H_
