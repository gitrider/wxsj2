/*
 * Name:        wx/mac/chkconf.h
 * Purpose:     Compiler-specific configuration checking
 * Author:      Julian Smart
 * Modified by:
 * Created:     01/02/97
 * RCS-ID:      $Id: chkconf.h,v 1.6 2005/06/02 13:12:06 DS Exp $
 * Copyright:   (c) Julian Smart
 * Licence:     wxWindows licence
 */

#ifndef _WX_MAC_CHKCONF_H_
#define _WX_MAC_CHKCONF_H_

/*
 * disable the settings which don't work for some compilers
 */

#if defined(__MWERKS__)
    #undef wxUSE_DEBUG_NEW_ALWAYS
    #define wxUSE_DEBUG_NEW_ALWAYS      0

    /* DS: Fixes compilation when wxUSE_FATAL_EXCEPTION is 1 */
    #ifndef wxTYPE_SA_HANDLER
        #define wxTYPE_SA_HANDLER int
    #endif
#endif

/*
 * use OS X CoreGraphics (1) or QuickDraw (0) for rendering
 */

#ifndef wxMAC_USE_CORE_GRAPHICS
    #define wxMAC_USE_CORE_GRAPHICS 0
#endif

#endif
    /* _WX_MAC_CHKCONF_H_ */

