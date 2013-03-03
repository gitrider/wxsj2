/////////////////////////////////////////////////////////////////////////////
// Name:        font.h
// Purpose:     wxFont class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: font.h,v 1.6 2004/05/23 20:50:42 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FONT_H_
#define _WX_FONT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "font.h"
#endif

class WXDLLEXPORT wxFontRefData: public wxGDIRefData
{
    friend class WXDLLEXPORT wxFont;
public:
    wxFontRefData()
        : m_fontId(0)
        , m_pointSize(10)
        , m_family(wxDEFAULT)
        , m_style(wxNORMAL)
        , m_weight(wxNORMAL)
        , m_underlined(FALSE)
        , m_faceName(wxT("Geneva"))
        , m_encoding(wxFONTENCODING_DEFAULT)
    {
        Init(10, wxDEFAULT, wxNORMAL, wxNORMAL, FALSE,
             wxT("Geneva"), wxFONTENCODING_DEFAULT);
    }

    wxFontRefData(const wxFontRefData& data)
        : wxGDIRefData()
        , m_fontId(data.m_fontId)
        , m_pointSize(data.m_pointSize)
        , m_family(data.m_family)
        , m_style(data.m_style)
        , m_weight(data.m_weight)
        , m_underlined(data.m_underlined)
        , m_faceName(data.m_faceName)
        , m_encoding(data.m_encoding)
    {
        Init(data.m_pointSize, data.m_family, data.m_style, data.m_weight,
             data.m_underlined, data.m_faceName, data.m_encoding);
    }

    wxFontRefData(int size,
                  int family,
                  int style,
                  int weight,
                  bool underlined,
                  const wxString& faceName,
                  wxFontEncoding encoding)
        : m_fontId(0)
        , m_pointSize(size)
        , m_family(family)
        , m_style(style)
        , m_weight(weight)
        , m_underlined(underlined)
        , m_faceName(faceName)
        , m_encoding(encoding)
    {
        Init(size, family, style, weight, underlined, faceName, encoding);
    }

    virtual ~wxFontRefData();
protected:
    // common part of all ctors
    void Init(int size,
              int family,
              int style,
              int weight,
              bool underlined,
              const wxString& faceName,
              wxFontEncoding encoding);

    // font characterstics
    int            m_fontId;
    int            m_pointSize;
    int            m_family;
    int            m_style;
    int            m_weight;
    bool           m_underlined;
    wxString       m_faceName;
    wxFontEncoding m_encoding;
    
public:
};
// ----------------------------------------------------------------------------
// wxFont
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFont : public wxFontBase
{
public:
    // ctors and such
    wxFont() { Init(); }
    wxFont(const wxFont& font)
        : wxFontBase()
    {
        Init();
        Ref(font);
    }

    wxFont(int size,
           int family,
           int style,
           int weight,
           bool underlined = FALSE,
           const wxString& face = wxEmptyString,
           wxFontEncoding encoding = wxFONTENCODING_DEFAULT)
    {
        Init();

        (void)Create(size, family, style, weight, underlined, face, encoding);
    }

    wxFont(const wxNativeFontInfo& info)
    {
        Init();

        (void)Create(info);
    }

    wxFont(const wxString& fontDesc);

    bool Create(int size,
                int family,
                int style,
                int weight,
                bool underlined = FALSE,
                const wxString& face = wxEmptyString,
                wxFontEncoding encoding = wxFONTENCODING_DEFAULT);

    bool Create(const wxNativeFontInfo& info);

    virtual ~wxFont();

    // assignment
    wxFont& operator=(const wxFont& font);

    // implement base class pure virtuals
    virtual int GetPointSize() const;
    virtual int GetFamily() const;
    virtual int GetStyle() const;
    virtual int GetWeight() const;
    virtual bool GetUnderlined() const;
    virtual wxString GetFaceName() const;
    virtual wxFontEncoding GetEncoding() const;
    virtual const wxNativeFontInfo *GetNativeFontInfo() const;

    virtual void SetPointSize(int pointSize);
    virtual void SetFamily(int family);
    virtual void SetStyle(int style);
    virtual void SetWeight(int weight);
    virtual void SetFaceName(const wxString& faceName);
    virtual void SetUnderlined(bool underlined);
    virtual void SetEncoding(wxFontEncoding encoding);

    // implementation only from now on
    // -------------------------------

    virtual bool RealizeResource();

protected:
    // common part of all ctors
    void Init();

    void Unshare();

private:
    DECLARE_DYNAMIC_CLASS(wxFont)
};

#endif
    // _WX_FONT_H_
