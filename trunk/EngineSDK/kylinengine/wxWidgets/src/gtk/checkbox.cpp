/////////////////////////////////////////////////////////////////////////////
// Name:        checkbox.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: checkbox.cpp,v 1.68 2005/03/21 23:42:15 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "checkbox.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#if wxUSE_CHECKBOX

#include "wx/checkbox.h"

#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool           g_blockEventsOnDrag;
extern wxCursor       g_globalCursor;
extern wxWindowGTK   *g_delayedFocus;

//-----------------------------------------------------------------------------
// "clicked"
//-----------------------------------------------------------------------------

extern "C" {
static void gtk_checkbox_toggled_callback(GtkWidget *widget, wxCheckBox *cb)
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!cb->m_hasVMT) return;

    if (g_blockEventsOnDrag) return;
    
    if (cb->m_blockEvent) return;

#ifdef __WXGTK20__
    // Transitions for 3state checkbox must be done manually, GTK's checkbox
    // is 2state with additional "undetermined state" flag which isn't
    // changed automatically:
    if (cb->Is3State())
    {
        GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(widget);

        if (cb->Is3rdStateAllowedForUser())
        {
            // The 3 states cycle like this when clicked:
            // checked -> undetermined -> unchecked -> checked -> ...
            bool active = gtk_toggle_button_get_active(toggle);
            bool inconsistent = gtk_toggle_button_get_inconsistent(toggle);

            cb->m_blockEvent = true;
            
            if (!active && !inconsistent)
            {
                // checked -> undetermined
                gtk_toggle_button_set_active(toggle, true);
                gtk_toggle_button_set_inconsistent(toggle, true);
            }
            else if (!active && inconsistent)
            {
                // undetermined -> unchecked
                gtk_toggle_button_set_inconsistent(toggle, false);
            }
            else if (active && !inconsistent)
            {
                // unchecked -> checked
                // nothing to do
            }
            else
            {
                wxFAIL_MSG(_T("3state wxCheckBox in unexpected state!"));
            }
            
            cb->m_blockEvent = false;
        }
        else
        {
            // user's action unsets undetermined state:
            gtk_toggle_button_set_inconsistent(toggle, false);
        }
    }
#endif

    wxCommandEvent event(wxEVT_COMMAND_CHECKBOX_CLICKED, cb->GetId());
#ifdef __WXGTK20__
    event.SetInt(cb->Get3StateValue());
#else
    event.SetInt(cb->GetValue());
#endif
    event.SetEventObject(cb);
    cb->GetEventHandler()->ProcessEvent(event);
}
}

//-----------------------------------------------------------------------------
// wxCheckBox
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxCheckBox,wxControl)

wxCheckBox::wxCheckBox()
{
}

bool wxCheckBox::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString &label,
                        const wxPoint &pos,
                        const wxSize &size,
                        long style,
                        const wxValidator& validator,
                        const wxString &name )
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;
    m_blockEvent = FALSE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxCheckBox creation failed") );
        return FALSE;
    }

    wxASSERT_MSG( (style & wxCHK_ALLOW_3RD_STATE_FOR_USER) == 0 ||
                  (style & wxCHK_3STATE) != 0,
                  wxT("Using wxCHK_ALLOW_3RD_STATE_FOR_USER")
                  wxT(" style flag for a 2-state checkbox is useless") );

    if ( style & wxALIGN_RIGHT )
    {
        // VZ: as I don't know a way to create a right aligned checkbox with
        //     GTK we will create a checkbox without label and a label at the
        //     left of it
        m_widgetCheckbox = gtk_check_button_new();

        m_widgetLabel = gtk_label_new("");
        gtk_misc_set_alignment(GTK_MISC(m_widgetLabel), 0.0, 0.5);

        m_widget = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(m_widget), m_widgetLabel, FALSE, FALSE, 3);
        gtk_box_pack_start(GTK_BOX(m_widget), m_widgetCheckbox, FALSE, FALSE, 3);

        gtk_widget_show( m_widgetLabel );
        gtk_widget_show( m_widgetCheckbox );
    }
    else
    {
        m_widgetCheckbox = gtk_check_button_new_with_label("");
        m_widgetLabel = BUTTON_CHILD( m_widgetCheckbox );
        m_widget = m_widgetCheckbox;
    }
    SetLabel( label );

    gtk_signal_connect( GTK_OBJECT(m_widgetCheckbox),
                        "toggled",
                        GTK_SIGNAL_FUNC(gtk_checkbox_toggled_callback),
                        (gpointer *)this );

    m_parent->DoAddChild( this );

    PostCreation(size);

    return TRUE;
}

void wxCheckBox::SetValue( bool state )
{
    wxCHECK_RET( m_widgetCheckbox != NULL, wxT("invalid checkbox") );

    if (state == GetValue())
        return;

    m_blockEvent = TRUE;

    gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON(m_widgetCheckbox), state );

    m_blockEvent = FALSE;
}

bool wxCheckBox::GetValue() const
{
    wxCHECK_MSG( m_widgetCheckbox != NULL, FALSE, wxT("invalid checkbox") );

#ifdef __WXGTK20__
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_widgetCheckbox));
#else
    return GTK_TOGGLE_BUTTON(m_widgetCheckbox)->active;
#endif
}

#ifdef __WXGTK20__
void wxCheckBox::DoSet3StateValue(wxCheckBoxState state)
{
    SetValue(state != wxCHK_UNCHECKED);
    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(m_widgetCheckbox),
                                       state == wxCHK_UNDETERMINED);
}

wxCheckBoxState wxCheckBox::DoGet3StateValue() const
{
    if (gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(m_widgetCheckbox)))
    {
        return wxCHK_UNDETERMINED;
    }
    else
    {
        return GetValue() ? wxCHK_CHECKED : wxCHK_UNCHECKED;
    }
}
#endif

void wxCheckBox::SetLabel( const wxString& label )
{
    wxCHECK_RET( m_widgetLabel != NULL, wxT("invalid checkbox") );

    wxControl::SetLabel( label );

#ifdef __WXGTK20__
    wxString label2 = PrepareLabelMnemonics( label );
    gtk_label_set_text_with_mnemonic( GTK_LABEL(m_widgetLabel), wxGTK_CONV( label2 ) );
#else
    gtk_label_set( GTK_LABEL(m_widgetLabel), wxGTK_CONV( GetLabel() ) );
#endif
}

bool wxCheckBox::Enable( bool enable )
{
    if ( !wxControl::Enable( enable ) )
        return FALSE;

    gtk_widget_set_sensitive( m_widgetLabel, enable );

    return TRUE;
}

void wxCheckBox::DoApplyWidgetStyle(GtkRcStyle *style)
{
    gtk_widget_modify_style(m_widgetCheckbox, style);
    gtk_widget_modify_style(m_widgetLabel, style);
}

bool wxCheckBox::IsOwnGtkWindow( GdkWindow *window )
{
    return window == TOGGLE_BUTTON_EVENT_WIN(m_widget);
}

void wxCheckBox::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    if (g_globalCursor.Ok()) cursor = g_globalCursor;

    GdkWindow *event_window = TOGGLE_BUTTON_EVENT_WIN(m_widgetCheckbox);
    if ( event_window && cursor.Ok() )
    {
        /* I now set the cursor the anew in every OnInternalIdle call
           as setting the cursor in a parent window also effects the
           windows above so that checking for the current cursor is
           not possible. */

       gdk_window_set_cursor( event_window, cursor.GetCursor() );
    }

    if (g_delayedFocus == this)
    {
        if (GTK_WIDGET_REALIZED(m_widget))
        {
            gtk_widget_grab_focus( m_widget );
            g_delayedFocus = NULL;
        }
    }
    
    if (wxUpdateUIEvent::CanUpdate(this))
        UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
}

wxSize wxCheckBox::DoGetBestSize() const
{
    return wxControl::DoGetBestSize();
}

// static
wxVisualAttributes
wxCheckBox::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    return GetDefaultAttributesFromGTKWidget(gtk_check_button_new);
}

#endif
