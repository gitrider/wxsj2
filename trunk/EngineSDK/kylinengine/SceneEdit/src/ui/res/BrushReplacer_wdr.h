
#ifndef __WDR_BrushReplacer_H__
#define __WDR_BrushReplacer_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "BrushReplacer_wdr.h"
#endif

// Include wxWidgets' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/statline.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/toolbar.h>

// Declare window functions

const int ID_COMBOCTRL = 10000;
wxSizer *BrushReplacer( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

// Declare menubar functions

// Declare toolbar functions

// Declare bitmap functions

#endif

// End of generated file
