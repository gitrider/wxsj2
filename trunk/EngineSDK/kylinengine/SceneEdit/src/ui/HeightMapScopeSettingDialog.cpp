// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "HeightMapScopeSettingDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include "HeightMapScopeSettingDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/HeightMapScopeSetting_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreCodec.h>
#include <OgreStringConverter.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// HeightMapScopeSettingDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(HeightMapScopeSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(HeightMapScopeSettingDialog, wxDialog)
    EVT_INIT_DIALOG(HeightMapScopeSettingDialog::OnInitDialog)
END_EVENT_TABLE()

void HeightMapScopeSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);

	wxString strMin, strMax;
	strMin << mMinHeight;
	strMax << mMaxHeight;

    wxSizer* sizer = HeightMapScopeSetting(this, true, true, strMin, strMax);

    /// 获取各控件的引用
    mMinHeightTextCtrl = wxDynamicCast(this->FindWindow(ID_MINHEIGHTTEXTCTRL),wxTextCtrl);
    mMaxHeightTextCtrl = wxDynamicCast(this->FindWindow(ID_MAXHEIGHTTEXTCTRL),wxTextCtrl);
}

float HeightMapScopeSettingDialog::getMinHeight()
{
	return 	Ogre::StringConverter::parseReal(static_cast<Ogre::String>(mMinHeightTextCtrl->GetValue()));
}

float HeightMapScopeSettingDialog::getMaxHeight()
{
	return 	Ogre::StringConverter::parseReal(static_cast<Ogre::String>(mMaxHeightTextCtrl->GetValue()));
}

void HeightMapScopeSettingDialog::setMinHeight(float height)
{
	mMinHeight = height;
}

void HeightMapScopeSettingDialog::setMaxHeight(float height)
{
	mMaxHeight = height;
}
