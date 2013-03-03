/////////////////////////////////////////////////////////////////////////////
// Name:        xh_statpict.h
// Purpose:     XRC resource handler for wxStaticPicture
// Author:      David A. Norris
// Created:     2005/03/13
// RCS-ID:      $Id: xh_statpict.h,v 1.4 2005/04/11 12:11:00 ABX Exp $
// Copyright:   David A. Norris
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STATPICT_H
#define _WX_XH_STATPICT_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_statpict.h"
#endif

#include "wx/xrc/xmlres.h"
#include "wx/gizmos/gizmos.h"

//
// XML resource handler for the wxStaticPicture class in wxContrib.
//

class WXDLLIMPEXP_GIZMOS_XRC wxStaticPictureXmlHandler
    : public wxXmlResourceHandler
{
public:

    // Constructor.
    wxStaticPictureXmlHandler();

    // Creates the control and returns a pointer to it.
    virtual wxObject *DoCreateResource();

    // Returns true if we know how to create a control for the given node.
    virtual bool CanHandle(wxXmlNode *node);

    // Register with wxWindows' dynamic class subsystem.
    DECLARE_DYNAMIC_CLASS(wxStaticPictureXmlHandler)
};

#endif
