/////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.cpp
// Purpose:     wxTextCtrl
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: textctrl.cpp,v 1.47 2005/02/06 17:38:32 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "textctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __VMS
#define XtParent XTPARENT
#endif

#include "wx/defs.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include "wx/textctrl.h"
#include "wx/settings.h"
#include "wx/filefn.h"
#include "wx/utils.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Text.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// helper: inserts the new text in the value of the text ctrl and returns the
// result in place
static void MergeChangesIntoString(wxString& value,
                                   XmTextVerifyCallbackStruct *textStruct);

// callbacks
static void wxTextWindowChangedProc(Widget w, XtPointer clientData, XtPointer ptr);
static void wxTextWindowModifyProc(Widget w, XtPointer clientData, XmTextVerifyCallbackStruct *cbs);
static void wxTextWindowGainFocusProc(Widget w, XtPointer clientData, XmAnyCallbackStruct *cbs);
static void wxTextWindowLoseFocusProc(Widget w, XtPointer clientData, XmAnyCallbackStruct *cbs);
static void wxTextWindowActivateProc(Widget w, XtPointer clientData, XmAnyCallbackStruct *ptr);

    IMPLEMENT_DYNAMIC_CLASS(wxTextCtrl, wxControl)

    BEGIN_EVENT_TABLE(wxTextCtrl, wxControl)
        EVT_DROP_FILES(wxTextCtrl::OnDropFiles)
        EVT_CHAR(wxTextCtrl::OnChar)

    EVT_MENU(wxID_CUT, wxTextCtrl::OnCut)
    EVT_MENU(wxID_COPY, wxTextCtrl::OnCopy)
    EVT_MENU(wxID_PASTE, wxTextCtrl::OnPaste)
    EVT_MENU(wxID_UNDO, wxTextCtrl::OnUndo)
    EVT_MENU(wxID_REDO, wxTextCtrl::OnRedo)

    EVT_UPDATE_UI(wxID_CUT, wxTextCtrl::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxTextCtrl::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxTextCtrl::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxTextCtrl::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxTextCtrl::OnUpdateRedo)

    END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxTextCtrl
// ----------------------------------------------------------------------------

// Text item
wxTextCtrl::wxTextCtrl()
{
    m_tempCallbackStruct = (void*) NULL;
    m_modified = false;
    m_processedDefault = false;
}

bool wxTextCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString& value,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxValidator& validator,
                        const wxString& name)
{
    if( !CreateControl( parent, id, pos, size, style, validator, name ) )
        return false;

    m_tempCallbackStruct = (void*) NULL;
    m_modified = false;
    m_processedDefault = false;

    m_backgroundColour = *wxWHITE;

    Widget parentWidget = (Widget) parent->GetClientWidget();

    bool wantHorizScrolling = ((m_windowStyle & wxHSCROLL) != 0);

    // If we don't have horizontal scrollbars, we want word wrap.
    bool wantWordWrap = !wantHorizScrolling;

    if (m_windowStyle & wxTE_MULTILINE)
    {
        Arg args[2];
        XtSetArg (args[0], XmNscrollHorizontal, wantHorizScrolling ? True : False);
        XtSetArg (args[1], XmNwordWrap, wantWordWrap ? True : False);

        m_mainWidget = (WXWidget) XmCreateScrolledText(parentWidget,
                                                       wxConstCast(name.c_str(), char),
                                                       args, 2);

        XtVaSetValues ((Widget) m_mainWidget,
                        XmNeditable, ((style & wxTE_READONLY) ? False : True),
                        XmNeditMode, XmMULTI_LINE_EDIT,
                        NULL);
        XtManageChild ((Widget) m_mainWidget);
    }
    else
    {
        m_mainWidget = (WXWidget)XtVaCreateManagedWidget
                                 (
                                  wxConstCast(name.c_str(), char),
                                  xmTextWidgetClass,
                                  parentWidget,
                                  NULL
                                 );

        XtVaSetValues ((Widget) m_mainWidget,
                        XmNeditable, ((style & wxTE_READONLY) ? False : True),
                        NULL);

        // TODO: Is this relevant? What does it do?
        int noCols = 2;
        if (!value.IsNull() && (value.Length() > (unsigned int) noCols))
            noCols = value.Length();
        XtVaSetValues((Widget) m_mainWidget,
                      XmNcolumns, noCols,
                      NULL);
    }

    // remove border if asked for
    if ( style & wxNO_BORDER )
    {
        XtVaSetValues((Widget)m_mainWidget,
                      XmNshadowThickness, 0,
                      NULL);
    }

    if ( !value.empty() )
    {
        // do this instead... MB
        //
        XtVaSetValues( (Widget) m_mainWidget,
                       XmNvalue, wxConstCast(value.c_str(), char),
                       NULL);
    }

    // install callbacks
    XtAddCallback((Widget) m_mainWidget, XmNvalueChangedCallback, (XtCallbackProc)wxTextWindowChangedProc, (XtPointer)this);

    XtAddCallback((Widget) m_mainWidget, XmNmodifyVerifyCallback, (XtCallbackProc)wxTextWindowModifyProc, (XtPointer)this);

    XtAddCallback((Widget) m_mainWidget, XmNactivateCallback, (XtCallbackProc)wxTextWindowActivateProc, (XtPointer)this);

    XtAddCallback((Widget) m_mainWidget, XmNfocusCallback, (XtCallbackProc)wxTextWindowGainFocusProc, (XtPointer)this);

    XtAddCallback((Widget) m_mainWidget, XmNlosingFocusCallback, (XtCallbackProc)wxTextWindowLoseFocusProc, (XtPointer)this);

    // font
    ChangeFont(false);

    wxSize best = GetBestSize();
    if( size.x != -1 ) best.x = size.x;
    if( size.y != -1 ) best.y = size.y;

    AttachWidget (parent, m_mainWidget, (WXWidget) NULL,
                  pos.x, pos.y, best.x, best.y);

    ChangeBackgroundColour();

    return true;
}

WXWidget wxTextCtrl::GetTopWidget() const
{
    return IsMultiLine() ? (WXWidget)XtParent((Widget)m_mainWidget)
                         : m_mainWidget;
}

wxString wxTextCtrl::GetValue() const
{
    wxString str; // result

    if (m_windowStyle & wxTE_PASSWORD)
    {
        // the value is stored always in m_value because it can't be retrieved
        // from the text control
        str = m_value;
    }
    else
    {
        // just get the string from Motif
        char *s = XmTextGetString ((Widget) m_mainWidget);
        if ( s )
        {
            str = s;
            XtFree (s);
        }
        //else: return empty string

        if ( m_tempCallbackStruct )
        {
            // the string in the control isn't yet updated, can't use it as is
            MergeChangesIntoString(str, (XmTextVerifyCallbackStruct *)
                                   m_tempCallbackStruct);
        }
    }

    return str;
}

void wxTextCtrl::SetValue(const wxString& value)
{
    m_inSetValue = true;

    // do this instead... MB
    //
    // with (at least) OpenMotif 2.1 this causes a lot of flicker
#if 0
    XtVaSetValues( (Widget) m_mainWidget,
                   XmNvalue, wxConstCast(value.c_str(), char),
                   NULL);
#endif

    Clear();
    AppendText( value );

    m_inSetValue = false;
}

// Clipboard operations
void wxTextCtrl::Copy()
{
    XmTextCopy((Widget) m_mainWidget, CurrentTime);
}

void wxTextCtrl::Cut()
{
    XmTextCut((Widget) m_mainWidget, CurrentTime);
}

void wxTextCtrl::Paste()
{
    XmTextPaste((Widget) m_mainWidget);
}

bool wxTextCtrl::CanCopy() const
{
    // Can copy if there's a selection
    long from, to;
    GetSelection(& from, & to);
    return (from != to) ;
}

bool wxTextCtrl::CanCut() const
{
    // Can cut if there's a selection
    long from, to;
    GetSelection(& from, & to);
    return (from != to) && (IsEditable());
}

bool wxTextCtrl::CanPaste() const
{
    return IsEditable() ;
}

// Undo/redo
void wxTextCtrl::Undo()
{
    // Not possible in Motif
}

void wxTextCtrl::Redo()
{
    // Not possible in Motif
}

bool wxTextCtrl::CanUndo() const
{
    // No Undo in Motif
    return false;
}

bool wxTextCtrl::CanRedo() const
{
    // No Redo in Motif
    return false;
}

// If the return values from and to are the same, there is no
// selection.
void wxTextCtrl::GetSelection(long* from, long* to) const
{
    XmTextPosition left, right;

    XmTextGetSelectionPosition((Widget) m_mainWidget, & left, & right);

    *from = (long) left;
    *to = (long) right;
}

bool wxTextCtrl::IsEditable() const
{
    return (XmTextGetEditable((Widget) m_mainWidget) != 0);
}

void wxTextCtrl::SetEditable(bool editable)
{
    XmTextSetEditable((Widget) m_mainWidget, (Boolean) editable);
}

void wxTextCtrl::SetInsertionPoint(long pos)
{
    XmTextSetInsertionPosition ((Widget) m_mainWidget, (XmTextPosition) pos);
}

void wxTextCtrl::SetInsertionPointEnd()
{
    wxTextPos pos = GetLastPosition();
    SetInsertionPoint(pos);
}

long wxTextCtrl::GetInsertionPoint() const
{
    return (long) XmTextGetInsertionPosition ((Widget) m_mainWidget);
}

wxTextPos wxTextCtrl::GetLastPosition() const
{
    return (long) XmTextGetLastPosition ((Widget) m_mainWidget);
}

void wxTextCtrl::Replace(long from, long to, const wxString& value)
{
    XmTextReplace ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
        wxConstCast(value.c_str(), char));
}

void wxTextCtrl::Remove(long from, long to)
{
    XmTextSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
                      (Time) 0);
    XmTextRemove ((Widget) m_mainWidget);
}

void wxTextCtrl::SetSelection(long from, long to)
{
    if( to == -1 )
        to = GetLastPosition();

    XmTextSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
                      (Time) 0);
}

void wxTextCtrl::WriteText(const wxString& text)
{
    long textPosition = GetInsertionPoint() + text.length();
    XmTextInsert ((Widget) m_mainWidget, GetInsertionPoint(),
                  wxConstCast(text.c_str(), char));
    XtVaSetValues ((Widget) m_mainWidget, XmNcursorPosition, textPosition, NULL);
    SetInsertionPoint(textPosition);
    XmTextShowPosition ((Widget) m_mainWidget, textPosition);
    m_modified = true;
}

void wxTextCtrl::AppendText(const wxString& text)
{
    wxTextPos textPosition = GetLastPosition() + text.length();
    XmTextInsert ((Widget) m_mainWidget, GetLastPosition(),
                  wxConstCast(text.c_str(), char));
    XtVaSetValues ((Widget) m_mainWidget, XmNcursorPosition, textPosition, NULL);
    SetInsertionPoint(textPosition);
    XmTextShowPosition ((Widget) m_mainWidget, textPosition);
    m_modified = true;
}

void wxTextCtrl::Clear()
{
    XmTextSetString ((Widget) m_mainWidget, "");
    m_modified = false;
}

bool wxTextCtrl::IsModified() const
{
    return m_modified;
}

// Makes modified or unmodified
void wxTextCtrl::MarkDirty()
{
    m_modified = true;
}

void wxTextCtrl::DiscardEdits()
{
    m_modified = false;
}

int wxTextCtrl::GetNumberOfLines() const
{
    // HIDEOUSLY inefficient, but we have no choice.
    char *s = XmTextGetString ((Widget) m_mainWidget);
    if (s)
    {
        long i = 0;
        int currentLine = 0;
        bool finished = false;
        while (!finished)
        {
            int ch = s[i];
            if (ch == '\n')
            {
                currentLine++;
                i++;
            }
            else if (ch == 0)
            {
                finished = true;
            }
            else
                i++;
        }

        XtFree (s);
        return currentLine;
    }
    return 0;
}

long wxTextCtrl::XYToPosition(long x, long y) const
{
/* It seems, that there is a bug in some versions of the Motif library,
    so the original wxWin-Code doesn't work. */
    /*
    Widget textWidget = (Widget) handle;
    return (long) XmTextXYToPos (textWidget, (Position) x, (Position) y);
    */
    /* Now a little workaround: */
    long r=0;
    for (int i=0; i<y; i++) r+=(GetLineLength(i)+1);
    return r+x;
}

bool wxTextCtrl::PositionToXY(long pos, long *x, long *y) const
{
    Position xx, yy;
    XmTextPosToXY((Widget) m_mainWidget, pos, &xx, &yy);
    if ( x )
        *x = xx;
    if ( y )
        *y = yy;

    return true;
}

void wxTextCtrl::ShowPosition(long pos)
{
    XmTextShowPosition ((Widget) m_mainWidget, (XmTextPosition) pos);
}

int wxTextCtrl::GetLineLength(long lineNo) const
{
    wxString str = GetLineText (lineNo);
    return (int) str.Length();
}

wxString wxTextCtrl::GetLineText(long lineNo) const
{
    // HIDEOUSLY inefficient, but we have no choice.
    char *s = XmTextGetString ((Widget) m_mainWidget);

    if (s)
    {
        wxString buf("");
        long i;
        int currentLine = 0;
        for (i = 0; currentLine != lineNo && s[i]; i++ )
            if (s[i] == '\n')
                currentLine++;
            // Now get the text
            int j;
            for (j = 0; s[i] && s[i] != '\n'; i++, j++ )
                buf += s[i];

            XtFree(s);
            return buf;
    }
    else
        return wxEmptyString;
}

/*
* Text item
*/

void wxTextCtrl::Command(wxCommandEvent & event)
{
    SetValue (event.GetString());
    ProcessCommand (event);
}

void wxTextCtrl::OnDropFiles(wxDropFilesEvent& event)
{
    // By default, load the first file into the text window.
    if (event.GetNumberOfFiles() > 0)
    {
        LoadFile(event.GetFiles()[0]);
    }
}

void wxTextCtrl::OnChar(wxKeyEvent& event)
{
    // Indicates that we should generate a normal command, because
    // we're letting default behaviour happen (otherwise it's vetoed
    // by virtue of overriding OnChar)
    m_processedDefault = true;

    if (m_tempCallbackStruct)
    {
        XmTextVerifyCallbackStruct *textStruct =
            (XmTextVerifyCallbackStruct *) m_tempCallbackStruct;
        textStruct->doit = True;
        if (isascii(event.m_keyCode) && (textStruct->text->length == 1))
        {
            textStruct->text->ptr[0] = ((event.m_keyCode == WXK_RETURN) ? 10 : event.m_keyCode);
        }
    }
}

void wxTextCtrl::ChangeFont(bool keepOriginalSize)
{
    wxWindow::ChangeFont(keepOriginalSize);
}

void wxTextCtrl::ChangeBackgroundColour()
{
    wxWindow::ChangeBackgroundColour();

    /* TODO: should scrollbars be affected? Should probably have separate
    * function to change them (by default, taken from wxSystemSettings)
    */
    if (m_windowStyle & wxTE_MULTILINE)
    {
        Widget parent = XtParent ((Widget) m_mainWidget);
        Widget hsb, vsb;

        XtVaGetValues (parent,
            XmNhorizontalScrollBar, &hsb,
            XmNverticalScrollBar, &vsb,
            NULL);
        wxColour backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
        if (hsb)
            wxDoChangeBackgroundColour((WXWidget) hsb, backgroundColour, true);
        if (vsb)
            wxDoChangeBackgroundColour((WXWidget) vsb, backgroundColour, true);

        // MBN: why change parent background?
        // DoChangeBackgroundColour((WXWidget) parent, m_backgroundColour, true);
    }
}

void wxTextCtrl::ChangeForegroundColour()
{
    wxWindow::ChangeForegroundColour();

    if (m_windowStyle & wxTE_MULTILINE)
    {
        Widget parent = XtParent ((Widget) m_mainWidget);
        Widget hsb, vsb;

        XtVaGetValues (parent,
            XmNhorizontalScrollBar, &hsb,
            XmNverticalScrollBar, &vsb,
            NULL);

            /* TODO: should scrollbars be affected? Should probably have separate
            * function to change them (by default, taken from wxSystemSettings)
            if (hsb)
            DoChangeForegroundColour((WXWidget) hsb, m_foregroundColour);
            if (vsb)
            DoChangeForegroundColour((WXWidget) vsb, m_foregroundColour);
        */
        wxDoChangeForegroundColour((WXWidget) parent, m_foregroundColour);
    }
}

void wxTextCtrl::DoSendEvents(void *wxcbs, long keycode)
{
    // we're in process of updating the text control
    m_tempCallbackStruct = wxcbs;

    XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *)wxcbs;

    wxKeyEvent event (wxEVT_CHAR);
    event.SetId(GetId());
    event.m_keyCode = keycode;
    event.SetEventObject(this);

    // Only if wxTextCtrl::OnChar is called will this be set to True (and
    // the character passed through)
    cbs->doit = False;

    GetEventHandler()->ProcessEvent(event);

    if ( !InSetValue() && m_processedDefault )
    {
        // Can generate a command
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        commandEvent.SetEventObject(this);
        ProcessCommand(commandEvent);
    }

    // do it after the (user) event handlers processed the events because
    // otherwise GetValue() would return incorrect (not yet updated value)
    m_tempCallbackStruct = NULL;
}

wxSize wxDoGetSingleTextCtrlBestSize( Widget textWidget,
                                      const wxWindow* window )
{
    Dimension xmargin, ymargin, highlight, shadow;
    char* value;

    XtVaGetValues( textWidget,
                   XmNmarginWidth, &xmargin,
                   XmNmarginHeight, &ymargin,
                   XmNvalue, &value,
                   XmNhighlightThickness, &highlight,
                   XmNshadowThickness, &shadow,
                   NULL );

    if( !value )
        value = "|";

    int x, y;
    window->GetTextExtent( value, &x, &y );

    if( x < 100 ) x = 100;

    return wxSize( x + 2 * xmargin + 2 * highlight + 2 * shadow,
                   // MBN: +2 necessary: Lesstif bug or mine?
                   y + 2 * ymargin + 2 * highlight + 2 * shadow + 2 );
}

wxSize wxTextCtrl::DoGetBestSize() const
{
    if( IsSingleLine() )
        return wxDoGetSingleTextCtrlBestSize( (Widget)m_mainWidget, this );
    else
        return wxWindow::DoGetBestSize();
}

// ----------------------------------------------------------------------------
// helpers and Motif callbacks
// ----------------------------------------------------------------------------

static void MergeChangesIntoString(wxString& value,
                                   XmTextVerifyCallbackStruct *cbs)
{
    /* _sm_
     * At least on my system (SunOS 4.1.3 + Motif 1.2), you need to think of
     * every event as a replace event.  cbs->text->ptr gives the replacement
     * text, cbs->startPos gives the index of the first char affected by the
     * replace, and cbs->endPos gives the index one more than the last char
     * affected by the replace (startPos == endPos implies an empty range).
     * Hence, a deletion is represented by replacing all input text with a
     * blank string ("", *not* NULL!).  A simple insertion that does not
     * overwrite any text has startPos == endPos.
     */

    if ( !value )
    {
        // easy case: the ol value was empty
        value = cbs->text->ptr;
    }
    else
    {
        // merge the changes into the value
        const char * const passwd = value;
        int len = value.length();

        len += ( cbs->text->ptr ?
                 strlen(cbs->text->ptr) :
                 0 ) + 1;                      // + new text (if any) + NUL
        len -= cbs->endPos - cbs->startPos;    // - text from affected region.

        char * newS = new char [len];
        char * dest = newS,
             * insert = cbs->text->ptr;

        // Copy (old) text from passwd, up to the start posn of the change.
        int i;
        const char * p = passwd;
        for (i = 0; i < cbs->startPos; ++i)
            *dest++ = *p++;

        // Copy the text to be inserted).
        if (insert)
            while (*insert)
                *dest++ = *insert++;

        // Finally, copy into newS any remaining text from passwd[endPos] on.
        for (p = passwd + cbs->endPos; *p; )
            *dest++ = *p++;
        *dest = 0;

        value = newS;

        delete[] newS;
    }
}

static void
wxTextWindowChangedProc (Widget w, XtPointer clientData, XtPointer WXUNUSED(ptr))
{
    if (!wxGetWindowFromTable(w))
        // Widget has been deleted!
        return;

    wxTextCtrl *tw = (wxTextCtrl *) clientData;
    tw->SetModified(true);
}

static void
wxTextWindowModifyProc (Widget WXUNUSED(w), XtPointer clientData, XmTextVerifyCallbackStruct *cbs)
{
    wxTextCtrl *tw = (wxTextCtrl *) clientData;
    tw->m_processedDefault = false;

    // First, do some stuff if it's a password control: in this case, we need
    // to store the string inside the class because GetValue() can't retrieve
    // it from the text ctrl. We do *not* do it in other circumstances because
    // it would double the amount of memory needed.

    if ( tw->GetWindowStyleFlag() & wxTE_PASSWORD )
    {
        MergeChangesIntoString(tw->m_value, cbs);

        if ( cbs->text->length > 0 )
        {
            int i;
            for (i = 0; i < cbs->text->length; ++i)
                cbs->text->ptr[i] = '*';
            cbs->text->ptr[i] = '\0';
        }
    }

    if(tw->InSetValue())
        return;

    // If we're already within an OnChar, return: probably a programmatic
    // insertion.
    if (tw->m_tempCallbackStruct)
        return;

    // Check for a backspace
    if (cbs->startPos == (cbs->currInsert - 1))
    {
        tw->DoSendEvents((void *)cbs, WXK_DELETE);

        return;
    }

    // Pasting operation: let it through without calling OnChar
    if (cbs->text->length > 1)
        return;

    // Something other than text
    if (cbs->text->ptr == NULL)
        return;

    // normal key press
    char ch = cbs->text->ptr[0];
    tw->DoSendEvents((void *)cbs, ch == '\n' ? '\r' : ch);
}

static void
wxTextWindowGainFocusProc (Widget w, XtPointer clientData, XmAnyCallbackStruct *WXUNUSED(cbs))
{
    if (!wxGetWindowFromTable(w))
        return;

    wxTextCtrl *tw = (wxTextCtrl *) clientData;
    wxFocusEvent event(wxEVT_SET_FOCUS, tw->GetId());
    event.SetEventObject(tw);
    tw->GetEventHandler()->ProcessEvent(event);
}

static void
wxTextWindowLoseFocusProc (Widget w, XtPointer clientData, XmAnyCallbackStruct *WXUNUSED(cbs))
{
    if (!wxGetWindowFromTable(w))
        return;

    wxTextCtrl *tw = (wxTextCtrl *) clientData;
    wxFocusEvent event(wxEVT_KILL_FOCUS, tw->GetId());
    event.SetEventObject(tw);
    tw->GetEventHandler()->ProcessEvent(event);
}

static void wxTextWindowActivateProc(Widget w, XtPointer clientData,
                                     XmAnyCallbackStruct *WXUNUSED(ptr))
{
    if (!wxGetWindowFromTable(w))
        return;

    wxTextCtrl *tw = (wxTextCtrl *) clientData;

    if (tw->InSetValue())
        return;

    wxCommandEvent event(wxEVT_COMMAND_TEXT_ENTER);
    event.SetId(tw->GetId());
    event.SetEventObject(tw);
    tw->ProcessCommand(event);
}

void wxTextCtrl::OnCut(wxCommandEvent& WXUNUSED(event))
{
    Cut();
}

void wxTextCtrl::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    Copy();
}

void wxTextCtrl::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    Paste();
}

void wxTextCtrl::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    Undo();
}

void wxTextCtrl::OnRedo(wxCommandEvent& WXUNUSED(event))
{
    Redo();
}

void wxTextCtrl::OnUpdateCut(wxUpdateUIEvent& event)
{
    event.Enable( CanCut() );
}

void wxTextCtrl::OnUpdateCopy(wxUpdateUIEvent& event)
{
    event.Enable( CanCopy() );
}

void wxTextCtrl::OnUpdatePaste(wxUpdateUIEvent& event)
{
    event.Enable( CanPaste() );
}

void wxTextCtrl::OnUpdateUndo(wxUpdateUIEvent& event)
{
    event.Enable( CanUndo() );
}

void wxTextCtrl::OnUpdateRedo(wxUpdateUIEvent& event)
{
    event.Enable( CanRedo() );
}
