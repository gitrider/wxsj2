/////////////////////////////////////////////////////////////////////////////
// Name:        m_pre.cpp
// Purpose:     wxHtml module for <PRE> ... </PRE> tag (code citation)
// Author:      Vaclav Slavik
// RCS-ID:      $Id: m_pre.cpp,v 1.30 2004/09/27 19:15:06 ABX Exp $
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation
#endif

#include "wx/wxprec.h"

#include "wx/defs.h"
#if wxUSE_HTML && wxUSE_STREAMS
#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WXPRECOMP
#endif

#include "wx/html/forcelnk.h"
#include "wx/html/m_templ.h"

#include "wx/html/htmlcell.h"
#include "wx/tokenzr.h"
#include "wx/encconv.h"

FORCE_LINK_ME(m_pre)

// replaces '\t', ' ' and '\n' with HTML markup:
static wxString LINKAGEMODE HtmlizeWhitespaces(const wxString& str)
{
    wxString out;
    size_t len = str.Len();
    size_t linepos = 0;
    for (size_t i = 0; i < len; i++)
    {
        switch (str[i])
        {
            case wxT('<'):
                while (i < len && str[i] != wxT('>'))
                {
                    out << str[i++];
                    linepos++;
                }
                out << wxT('>');
                linepos++;
                break;
            case wxT(' '):
                out << wxT("&nbsp;");
                linepos++;
                break;
            case wxT('\n'):
                out << wxT("<br>");
                linepos = 0;
                break;
            case wxT('\t'):
                {
                    for (size_t j = 8 - linepos % 8; j > 0; j--)
                        out << wxT("&nbsp;");
                    linepos += 8 - linepos % 8;
                }
                break;
            default:
                out << str[i];
                linepos++;
                break;
        }
    }
    return out;
}


//-----------------------------------------------------------------------------
// The list handler:
//-----------------------------------------------------------------------------


TAG_HANDLER_BEGIN(PRE, "PRE")
    TAG_HANDLER_CONSTR(PRE) { }

    TAG_HANDLER_PROC(tag)
    {
        wxHtmlContainerCell *c;

        int fixed = m_WParser->GetFontFixed(),
            italic = m_WParser->GetFontItalic(),
            underlined = m_WParser->GetFontUnderlined(),
            bold = m_WParser->GetFontBold(),
            fsize = m_WParser->GetFontSize();

        c = m_WParser->GetContainer();
        m_WParser->SetFontUnderlined(false);
        m_WParser->SetFontBold(false);
        m_WParser->SetFontItalic(false);
        m_WParser->SetFontFixed(true);
        m_WParser->SetFontSize(3);
        c->InsertCell(new wxHtmlFontCell(m_WParser->CreateCurrentFont()));

        m_WParser->CloseContainer();
        c = m_WParser->OpenContainer();
        c->SetWidthFloat(tag);
        c = m_WParser->OpenContainer();
        c->SetAlignHor(wxHTML_ALIGN_LEFT);
        c->SetIndent(m_WParser->GetCharHeight(), wxHTML_INDENT_TOP);

        wxString srcMid =
            m_WParser->GetSource()->Mid(tag.GetBeginPos(),
                                        tag.GetEndPos1() - tag.GetBeginPos());
        // It is safe to temporarily change the source being parsed,
        // provided we restore the state back after parsing
        m_Parser->SetSourceAndSaveState(HtmlizeWhitespaces(srcMid));
        m_Parser->DoParsing();
        m_Parser->RestoreState();

        m_WParser->CloseContainer();
        m_WParser->CloseContainer();
        c = m_WParser->OpenContainer();

        m_WParser->SetFontUnderlined(underlined);
        m_WParser->SetFontBold(bold);
        m_WParser->SetFontItalic(italic);
        m_WParser->SetFontFixed(fixed);
        m_WParser->SetFontSize(fsize);
        c->InsertCell(new wxHtmlFontCell(m_WParser->CreateCurrentFont()));

        return true;
    }

TAG_HANDLER_END(PRE)





TAGS_MODULE_BEGIN(Pre)

    TAGS_MODULE_ADD(PRE)

TAGS_MODULE_END(Pre)

#endif
