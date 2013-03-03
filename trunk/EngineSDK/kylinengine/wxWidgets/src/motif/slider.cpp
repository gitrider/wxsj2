/////////////////////////////////////////////////////////////////////////////
// Name:        slider.cpp
// Purpose:     wxSlider
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: slider.cpp,v 1.24 2005/01/18 15:06:31 KH Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "slider.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#if wxUSE_SLIDER

#include "wx/slider.h"
#include "wx/utils.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

void wxSliderCallback (Widget widget, XtPointer clientData, XmScaleCallbackStruct * cbs);

IMPLEMENT_DYNAMIC_CLASS(wxSlider, wxControl)

BEGIN_EVENT_TABLE(wxSlider, wxControl)
END_EVENT_TABLE()



// Slider
wxSlider::wxSlider()
{
    m_pageSize = 1;
    m_lineSize = 1;
    m_rangeMax = 0;
    m_rangeMin = 0;
}

bool wxSlider::Create(wxWindow *parent, wxWindowID id,
                      int value, int minValue, int maxValue,
                      const wxPoint& pos,
                      const wxSize& size, long style,
                      const wxValidator& validator,
                      const wxString& name)
{
    if ( !((style & wxSL_HORIZONTAL) || (style & wxSL_VERTICAL)) )
         style |= wxSL_HORIZONTAL;

    if( !CreateControl( parent, id, pos, size, style, validator, name ) )
        return false;

    m_lineSize = 1;
    m_windowStyle = style;

    m_rangeMax = maxValue;
    m_rangeMin = minValue;

    // Not used in Motif, I think
    m_pageSize = (int)((maxValue-minValue)/10);

    Widget parentWidget = (Widget) parent->GetClientWidget();

    Widget sliderWidget = XtVaCreateManagedWidget ("sliderWidget",
        xmScaleWidgetClass, parentWidget,
        XmNorientation,
        (((m_windowStyle & wxSL_VERTICAL) == wxSL_VERTICAL) ? XmVERTICAL : XmHORIZONTAL),
        XmNprocessingDirection,
        (((m_windowStyle & wxSL_VERTICAL) == wxSL_VERTICAL) ? XmMAX_ON_TOP : XmMAX_ON_RIGHT),
        XmNmaximum, maxValue,
        XmNminimum, minValue,
        XmNvalue, value,
        XmNshowValue, True,
        NULL);

    m_mainWidget = (WXWidget) sliderWidget;

#ifdef __VMS__
#pragma message disable codcauunr
   // VMS gives here the compiler warning :
   // statement either is unreachable or causes unreachable code
#endif
    if(style & wxSL_NOTIFY_DRAG)
        XtAddCallback (sliderWidget, XmNdragCallback,
        (XtCallbackProc) wxSliderCallback, (XtPointer) this);
    else
        XtAddCallback (sliderWidget, XmNvalueChangedCallback,
        (XtCallbackProc) wxSliderCallback, (XtPointer) this);
#ifdef __VMS__
#pragma message enable codcauunr
#endif

    XtAddCallback (sliderWidget, XmNdragCallback, (XtCallbackProc) wxSliderCallback, (XtPointer) this);

    ChangeFont(false);
    AttachWidget (parent, m_mainWidget, (WXWidget) NULL, pos.x, pos.y, size.x, size.y);

    ChangeBackgroundColour();

    return true;
}

wxSlider::~wxSlider()
{
}

int wxSlider::GetValue() const
{
    int val;
    XtVaGetValues ((Widget) m_mainWidget, XmNvalue, &val, NULL);
    return val;
}

void wxSlider::SetValue(int value)
{
    XtVaSetValues ((Widget) m_mainWidget, XmNvalue, value, NULL);
}

void wxSlider::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    Widget widget = (Widget) m_mainWidget;

    bool managed = XtIsManaged(widget);

    if (managed)
        XtUnmanageChild (widget);

    if (((m_windowStyle & wxHORIZONTAL) == wxHORIZONTAL) && (width > -1))
    {
        XtVaSetValues (widget, XmNscaleWidth, wxMax (width, 10), NULL);
    }

    if (((m_windowStyle & wxVERTICAL) == wxVERTICAL) && (height > -1))
    {
        XtVaSetValues (widget, XmNscaleHeight, wxMax (height, 10), NULL);
    }

    int xx = x; int yy = y;
    AdjustForParentClientOrigin(xx, yy, sizeFlags);

    if (x > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        XtVaSetValues (widget, XmNx, xx, NULL);
    if (y > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        XtVaSetValues (widget, XmNy, yy, NULL);

    if (managed)
        XtManageChild (widget);
}

void wxSlider::SetRange(int minValue, int maxValue)
{
    m_rangeMin = minValue;
    m_rangeMax = maxValue;

    XtVaSetValues ((Widget) m_mainWidget, XmNminimum, minValue, XmNmaximum, maxValue, NULL);
}

// For trackbars only
void wxSlider::SetPageSize(int pageSize)
{
    // Not implemented in Motif
    m_pageSize = pageSize;
}

int wxSlider::GetPageSize() const
{
    return m_pageSize;
}

void wxSlider::SetLineSize(int lineSize)
{
    // Not implemented in Motif
    m_lineSize = lineSize;
}

int wxSlider::GetLineSize() const
{
    // Not implemented in Motif
    return m_lineSize;
}

void wxSlider::SetThumbLength(int WXUNUSED(len))
{
    // Not implemented in Motif (?)
}

int wxSlider::GetThumbLength() const
{
    // Not implemented in Motif (?)
    return 0;
}

void wxSlider::Command (wxCommandEvent & event)
{
    SetValue (event.GetInt());
    ProcessCommand (event);
}

void wxSliderCallback (Widget widget, XtPointer clientData,
                       XmScaleCallbackStruct * cbs)
{
    wxSlider *slider = (wxSlider *) clientData;
    wxEventType scrollEvent;

    switch (cbs->reason)
    {
    case XmCR_VALUE_CHANGED:
        scrollEvent = wxEVT_SCROLL_THUMBRELEASE;
        break;

    case XmCR_DRAG:
        scrollEvent = wxEVT_SCROLL_THUMBTRACK;
        break;

    default:
        return;
    }

    wxScrollEvent event(scrollEvent, slider->GetId());
    int commandInt = event.GetInt();
    XtVaGetValues (widget, XmNvalue, &commandInt, NULL);
    event.SetInt(commandInt);
    event.SetEventObject(slider);
    slider->GetEventHandler()->ProcessEvent(event);

    // Also send a wxCommandEvent for compatibility.
    wxCommandEvent event2(wxEVT_COMMAND_SLIDER_UPDATED, slider->GetId());
    event2.SetEventObject(slider);
    event2.SetInt( event.GetInt() );
    slider->GetEventHandler()->ProcessEvent(event2);
}

#endif // wxUSE_SLIDER
