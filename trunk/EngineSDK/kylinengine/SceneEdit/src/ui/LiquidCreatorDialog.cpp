// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "LiquidCreatorDialog.h"
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

#include "LiquidCreatorDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/LiquidCreator_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreMaterialManager.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

IMPLEMENT_DYNAMIC_CLASS(LiquidCreatorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(LiquidCreatorDialog, wxDialog)
EVT_INIT_DIALOG(LiquidCreatorDialog::OnInitDialog)
END_EVENT_TABLE()

void
LiquidCreatorDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);
	wxSizer* sizer = LiquidCreator(this, true, true);

	/// ��ȡ���ؼ�������
	mComboBox = wxDynamicCast(this->FindWindow(ID_MATERIALCOMBO),wxComboBox);

	assert (mComboBox);

	Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

	while ( resourceMapIterator.hasMoreElements() )
	{
		if ( resourceMapIterator.peekNextValue()->getGroup() == Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME )
		{
			Ogre::String matName = resourceMapIterator.peekNextValue()->getName();

			// ֻ�в��������а�����ˮ���ż��뵽combobox��
			if (matName.find("water") != Ogre::String::npos || 
                matName.find("Water") != Ogre::String::npos ||
                matName.find("ˮ01") != Ogre::String::npos)
				mComboBox->AppendString( matName.c_str() );
		}	

		resourceMapIterator.moveNext();
	}

	resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
	mComboBox->SetValue( resourceMapIterator.peekNextValue()->getName().c_str() );

	mSubDivideTextCtrl = wxDynamicCast(this->FindWindow(ID_SUBDIVISIONTEXTCTRL),wxTextCtrl);
	assert (mSubDivideTextCtrl);

	mTexScaleTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXCOORDSCALETEXTCTRL),wxTextCtrl);
	assert (mTexScaleTextCtrl);

	mDiffuseTextCtrl = wxDynamicCast(this->FindWindow(ID_DIFFUSETEXTCTRL),wxTextCtrl);
	assert (mDiffuseTextCtrl);

	mDepthTextCtrl = wxDynamicCast(this->FindWindow(ID_DEPTHTEXTCTRL),wxTextCtrl);
	assert (mDepthTextCtrl);
}