/*
 * Name:        wx/mac/chkconf.h
 * Purpose:     Mac-specific config settings checks
 * Author:      Vadim Zeitlin
 * Modified by:
 * Created:     2005-04-05 (extracted from wx/chkconf.h)
 * RCS-ID:      $Id: chkconf.h,v 1.5 2005/04/05 20:54:56 VZ Exp $
 * Copyright:   (c) 2005 Vadim Zeitlin <vadim@wxwidgets.org>
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#if wxUSE_UNICODE
#    if !TARGET_CARBON
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxUSE_UNICODE is only supported for Carbon Targets."
#        else
#            define wxUSE_UNICODE 0
#        endif
#    endif
#endif

#ifdef __WXMAC_CLASSIC__
#   include "wx/mac/classic/chkconf.h"
#else
#   include "wx/mac/carbon/chkconf.h"
#endif
