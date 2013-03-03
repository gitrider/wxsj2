///////////////////////////////////////////////////////////////////////////////
// Name:        common/bookctrl.cpp
// Purpose:     wxBookCtrlBase implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     19.08.03
// RCS-ID:      $Id: bookctrl.cpp,v 1.14 2005/05/31 09:19:48 JS Exp $
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "bookctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_BOOKCTRL

#include "wx/imaglist.h"

#include "wx/bookctrl.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// constructors and destructors
// ----------------------------------------------------------------------------

void wxBookCtrlBase::Init()
{
    m_imageList = NULL;
    m_ownsImageList = false;
}

bool
wxBookCtrlBase::Create(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name)
{
    return wxControl::Create
                     (
                        parent,
                        id,
                        pos,
                        size,
                        style,
                        wxDefaultValidator,
                        name
                     );
}

wxBookCtrlBase::~wxBookCtrlBase()
{
    if ( m_ownsImageList )
    {
        // may be NULL, ok
        delete m_imageList;
    }
}

// ----------------------------------------------------------------------------
// image list
// ----------------------------------------------------------------------------

void wxBookCtrlBase::SetImageList(wxImageList *imageList)
{
    if ( m_ownsImageList )
    {
        // may be NULL, ok
        delete m_imageList;

        m_ownsImageList = false;
    }

    m_imageList = imageList;
}

void wxBookCtrlBase::AssignImageList(wxImageList* imageList)
{
    SetImageList(imageList);

    m_ownsImageList = true;
}

// ----------------------------------------------------------------------------
// geometry
// ----------------------------------------------------------------------------

void wxBookCtrlBase::SetPageSize(const wxSize& size)
{
    SetClientSize(CalcSizeFromPage(size));
}

wxSize wxBookCtrlBase::DoGetBestSize() const
{
    wxSize bestSize;

    // iterate over all pages, get the largest width and height
    const size_t nCount = m_pages.size();
    for ( size_t nPage = 0; nPage < nCount; nPage++ )
    {
        wxWindow *pPage = m_pages[nPage];
        wxSize childBestSize(pPage->GetBestSize());

        if ( childBestSize.x > bestSize.x )
            bestSize.x = childBestSize.x;

        if ( childBestSize.y > bestSize.y )
            bestSize.y = childBestSize.y;
    }

    // convert display area to window area, adding the size necessary for the
    // tabs
    wxSize best = CalcSizeFromPage(bestSize);
    CacheBestSize(best);
    return best;
}

// ----------------------------------------------------------------------------
// pages management
// ----------------------------------------------------------------------------

bool
wxBookCtrlBase::InsertPage(size_t nPage,
                           wxWindow *page,
                           const wxString& WXUNUSED(text),
                           bool WXUNUSED(bSelect),
                           int WXUNUSED(imageId))
{
    wxCHECK_MSG( page, false, _T("NULL page in wxBookCtrlBase::InsertPage()") );
    wxCHECK_MSG( nPage <= m_pages.size(), false,
                 _T("invalid page index in wxBookCtrlBase::InsertPage()") );

    m_pages.Insert(page, nPage);
    InvalidateBestSize();

    return true;
}

bool wxBookCtrlBase::DeletePage(size_t nPage)
{
    wxWindow *page = DoRemovePage(nPage);
    if ( !page )
        return false;

    delete page;

    return true;
}

wxWindow *wxBookCtrlBase::DoRemovePage(size_t nPage)
{
    wxCHECK_MSG( nPage < m_pages.size(), NULL,
                 _T("invalid page index in wxBookCtrlBase::DoRemovePage()") );

    wxWindow *pageRemoved = m_pages[nPage];
    m_pages.RemoveAt(nPage);
    InvalidateBestSize();

    return pageRemoved;
}

int wxBookCtrlBase::GetNextPage(bool forward) const
{
    int nPage;

    int nMax = GetPageCount();
    if ( nMax-- ) // decrement it to get the last valid index
    {
        int nSel = GetSelection();

        // change selection wrapping if it becomes invalid
        nPage = forward ? nSel == nMax ? 0
                                       : nSel + 1
                        : nSel == 0 ? nMax
                                    : nSel - 1;
    }
    else // notebook is empty, no next page
    {
        nPage = -1;
    }

    return nPage;
}

#endif // wxUSE_BOOKCTRL

