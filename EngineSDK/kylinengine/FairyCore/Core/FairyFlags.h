/********************************************************************
    filename:   FairyFlags.h
    created:    2005/10/12
    author:     Genva Xie (genva@21cn.com)
    
    purpose:    Object query and visibility flags for special
                case purpose.
*********************************************************************/

#ifndef __FairyFlags_H__
#define __FairyFlags_H__

#include "FairyPrerequisites.h"

namespace Fairy {

    // Object query flags
    enum ObjectQueryFlags
    {
        // The object can project decal on it
        OQF_DECAL_TARGET    = 1 << 0,

        // The object can use to collide detect
        OQF_COLLIDEABLE     = 1 << 1,

		//	HDF Added special query
		OQF_SPECIAL_FLAG	= 1	<< 7,

        // The mask is use as default object query mask
        OQF_DEFAULT         = ~0 << 8,
    };

    // Object visibility flags
    enum ObjectVisibilityFlags
    {
        // The flag use special for post filter processing
        OVF_POSTFILTER      = 1 << 0,

        // The flags use for special for editor indicators
        OVF_INDICATORS      = 1 << 1,

        // The flags use for GUI elements
        OVF_GUI_ELEMENTS    = 1 << 2,

		/// hudf added special query
		OVF_SPECIAL_FLAG	= 1 << 7,

        // The mask is use as default object visibility mask
        OVF_DEFAULT         = ~0 << 8,

		OVF_TERRAIN			= 1 << 16,

		OVF_WATER			= 1 << 17,

		OVF_DETAILOBJ			= 1 << 18,

		OVF_STATIC_OBJ		= 1 << 19,

		OVF_DYNAMIC_OBJ		= 1 << 20,

    };

}

#endif // __FairyFlags_H__
