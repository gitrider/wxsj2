///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wrapshl.h
// Purpose:     wrapper class for stuff from shell32.dll
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2004-10-19
// RCS-ID:      $Id: wrapshl.h,v 1.1 2004/10/19 16:43:43 VZ Exp $
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_WRAPSHL_H_
#define _WX_MSW_WRAPSHL_H_

#ifdef __WXWINCE__
    #include <winreg.h>
    #include <objbase.h>
    #include <shlguid.h>
#endif

#include <shlobj.h>

// ----------------------------------------------------------------------------
// wxItemIdList implements RAII on top of ITEMIDLIST
// ----------------------------------------------------------------------------

class wxItemIdList
{
public:
    // ctor takes ownership of the item and will free it
    wxItemIdList(LPITEMIDLIST pidl)
    {
        m_pidl = pidl;
    }

    static void Free(LPITEMIDLIST pidl)
    {
        if ( pidl )
        {
            LPMALLOC pMalloc;
            SHGetMalloc(&pMalloc);
            if ( pMalloc )
            {
                pMalloc->Free(pidl);
                pMalloc->Release();
            }
            else
            {
                wxLogLastError(wxT("SHGetMalloc"));
            }
        }
    }

    ~wxItemIdList()
    {
        Free(m_pidl);
    }

    // implicit conversion to LPITEMIDLIST
    operator LPITEMIDLIST() const { return m_pidl; }

    // get the corresponding path, returns empty string on error
    wxString GetPath() const
    {
        wxString path;
        if ( !SHGetPathFromIDList(m_pidl, wxStringBuffer(path, MAX_PATH)) )
        {
            wxLogLastError(_T("SHGetPathFromIDList"));
        }

        return path;
    }

private:
    LPITEMIDLIST m_pidl;

    DECLARE_NO_COPY_CLASS(wxItemIdList)
};

#endif // _WX_MSW_WRAPSHL_H_

