#ifndef __WDR_AvatarDialog_H__
#define __WDR_AvatarDialog_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "ModelPartChangeDialog_wdr.h"
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
const int ID_BUTTON_HEAD			= 10000;
const int ID_BUTTON_FACE			= 10001;
const int ID_BUTTON_SHOULDER		= 10002;
const int ID_BUTTON_BODY			= 10003;
const int ID_BUTTON_HAND			= 10004;
const int ID_BUTTON_FOOT			= 10005;
const int ID_BUTTON_LWEAPON			= 10006;
const int ID_BUTTON_RWEAPON			= 10007;
const int ID_BUTTON_SHIELD			= 10008;
const int ID_BUTTON_RIDER			= 10009;


const int ID_ModelPartChangeDialog	= 10010;
const int ID_TEXT					= 10011;

wxSizer *AvatarDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );
#endif