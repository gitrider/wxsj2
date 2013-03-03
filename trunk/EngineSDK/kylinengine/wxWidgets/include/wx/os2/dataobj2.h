///////////////////////////////////////////////////////////////////////////////
// Name:        mac/dataobj2.h
// Purpose:     declaration of standard wxDataObjectSimple-derived classes
// Author:      Stefan Csomor (adapted from Robert Roebling's gtk port
// Modified by:
// Created:     10/21/99
// RCS-ID:      $Id: dataobj2.h,v 1.5 2004/05/23 20:51:38 JS Exp $
// Copyright:   (c) 1998, 1999 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_DATAOBJ2_H_
#define _WX_MAC_DATAOBJ2_H_

#ifdef __GNUG__
    #pragma interface "dataobj.h"
#endif

// ----------------------------------------------------------------------------
// wxBitmapDataObject is a specialization of wxDataObject for bitmaps
// ----------------------------------------------------------------------------

class wxBitmapDataObject : public wxBitmapDataObjectBase
{
public:
    // ctors
    wxBitmapDataObject();
    wxBitmapDataObject(const wxBitmap& bitmap);

    // destr
    ~wxBitmapDataObject();

    // override base class virtual to update PNG data too
    virtual void SetBitmap(const wxBitmap& bitmap);

    // implement base class pure virtuals
    // ----------------------------------

    virtual size_t GetDataSize() const { return m_pngSize; }
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);

protected:
    void Init() { m_pngData = (void *)NULL; m_pngSize = 0; }
    void Clear() { free(m_pngData); }
    void ClearAll() { Clear(); Init(); }

    size_t      m_pngSize;
    void       *m_pngData;

    void DoConvertToPng();

private:
    // Virtual function hiding supression
    size_t GetDataSize(const wxDataFormat& rFormat) const
    { return(wxDataObjectSimple::GetDataSize(rFormat)); }
    bool GetDataHere(const wxDataFormat& rFormat, void* pBuf) const
    { return(wxDataObjectSimple::GetDataHere(rFormat, pBuf)); }
    bool SetData(const wxDataFormat& rFormat, size_t nLen, const void* pBuf)
    { return(wxDataObjectSimple::SetData(rFormat, nLen, pBuf)); }
};

// ----------------------------------------------------------------------------
// wxFileDataObject is a specialization of wxDataObject for file names
// ----------------------------------------------------------------------------

class wxFileDataObject : public wxFileDataObjectBase
{
public:
    // implement base class pure virtuals
    // ----------------------------------

    void AddFile( const wxString &filename );

    virtual size_t GetDataSize() const;
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);

private:
    // Virtual function hiding supression
    size_t GetDataSize(const wxDataFormat& rFormat) const
    { return(wxDataObjectSimple::GetDataSize(rFormat)); }
    bool GetDataHere(const wxDataFormat& rFormat, void* pBuf) const
    { return(wxDataObjectSimple::GetDataHere(rFormat, pBuf)); }
    bool SetData(const wxDataFormat& rFormat, size_t nLen, const void* pBuf)
    { return(wxDataObjectSimple::SetData(rFormat, nLen, pBuf)); }
};

#endif // _WX_MAC_DATAOBJ2_H_

