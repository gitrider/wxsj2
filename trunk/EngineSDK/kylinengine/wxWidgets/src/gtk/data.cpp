/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/data.cpp
// Purpose:     Various global GTK-specific data
// Author:      Robert Roebling
// Id:          $Id: data.cpp,v 1.48 2004/05/23 20:52:19 JS Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/window.h"
#include "wx/dc.h"
#include "wx/cursor.h"

/* Current cursor, in order to hang on to
 * cursor handle when setting the cursor globally */
wxCursor g_globalCursor;

/* Don't allow event propagation during drag */
bool g_blockEventsOnDrag = FALSE;

/* Don't allow mouse event propagation during scroll */
bool g_blockEventsOnScroll = FALSE;

/* Don't allow window closing if there are open dialogs */
int g_openDialogs = 0;

/* TRUE when the message queue is empty. this gets set to
   FALSE by all event callbacks before anything else is done */
bool g_isIdle = FALSE;

