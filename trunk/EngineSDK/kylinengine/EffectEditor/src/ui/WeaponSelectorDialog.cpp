// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WeaponSelectorDialog.h"
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

#include <wx/treectrl.h>
#include <wx/colordlg.h>

#include "WeaponSelectorDialog.h"
#include "UIColourConverter.h"
#include "WXConfig.h"

#include "ModelSystem/FairyLogicModelManager.h"

#include "EffectSystem/FairyEffectManager.h"

#include "Core/FairyVariant.h"
#include <OgreStringVector.h>
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/WeaponSelector_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(WeaponSelectorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(WeaponSelectorDialog, wxDialog)
    EVT_INIT_DIALOG(WeaponSelectorDialog::OnInitDialog)

    EVT_TREE_SEL_CHANGED(ID_TREECTRL_WEAPON, WeaponSelectorDialog::OnWeaponSelected)
    EVT_LISTBOX(ID_LISTBOX_WEAPON_EFFECT, WeaponSelectorDialog::OnWeaponEffectSelected)
    EVT_BUTTON(ID_BUTTON_WEAPON_EFFECT_COLOUR, WeaponSelectorDialog::OnColourButtonDown)
    EVT_TEXT_ENTER(ID_TEXTCTRL_ALPHA, WeaponSelectorDialog::OnAlphaTextChanged)

END_EVENT_TABLE()

void
WeaponSelectorDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = WeaponSelector(this, true, true);

    if (false == mInitDialog)
    {
        // ��ȡ������
        mWeaponData = new FairyConfig("../EditorConfigFiles/Weapon.cfg");

        // ������������������
        mWeaponTree = wxDynamicCast(this->FindWindow(ID_TREECTRL_WEAPON), wxTreeCtrl);

        mWeaponTree->Freeze();
        mWeaponTree->DeleteAllItems();

        wxTreeItemId rootId = mWeaponTree->AddRoot(_("Weapons"));

        if (mWeaponData)
        {
            // ����weapon tree
            int sectionCount = mWeaponData->startGetSections();

            for (int i=0; i<sectionCount; ++i)
            {
                const Ogre::String& sectionName = mWeaponData->getNextSection();

                wxTreeItemId sectionId = mWeaponTree->AppendItem(rootId, sectionName.c_str());

				Ogre::StringVector valueList = 
                    mWeaponData->getList(sectionName, "Combo", ",");

                for (size_t j=0; j<valueList.size(); ++j)
                {
                    mWeaponTree->AppendItem(sectionId, valueList[j].c_str());
                }
            }

            mWeaponTree->Thaw();
            mWeaponTree->UnselectAll();
            mWeaponTree->Expand( mWeaponTree->GetRootItem() );
        }

        // ��ʾ��Ч�б�
        mWeaponEffectList = wxDynamicCast(this->FindWindow(ID_LISTBOX_WEAPON_EFFECT), wxListBox);

        Fairy::EffectManager::EffectTemplateIterator it = 
            Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

        while ( it.hasMoreElements() )
        {
            mWeaponEffectList->AppendString(it.peekNextKey().c_str());

            it.moveNext();
        }

        mColourButton = wxDynamicCast(this->FindWindow(ID_BUTTON_WEAPON_EFFECT_COLOUR), wxButton);

        mColourText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_COLOUR_TEXT), wxTextCtrl);
        mAlphaTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ALPHA), wxTextCtrl);

        mInitDialog = true;
    }

  //  mWeaponEffectList->Refresh();

    unsigned char r, g, b;

    FColourToIColour(mCurrentColour.r, mCurrentColour.g, mCurrentColour.b,
        r, g, b);

    mColourButton->SetBackgroundColour(wxColour(r,g,b));
    setColourText();

    setAlphaText();
}

WeaponSelectorDialog::~WeaponSelectorDialog()
{
    if (mWeaponData)
    {
        delete mWeaponData;
        mWeaponData = NULL;
    }
}

void WeaponSelectorDialog::OnWeaponSelected(wxTreeEvent& e)
{
    wxTreeItemId itemId = e.GetItem();

    if (itemId.IsOk() && itemId != mWeaponTree->GetRootItem())
    {
        Ogre::String weaponName = mWeaponTree->GetItemText(itemId).c_str();

        // ���û�����obj�ļ�������
        if ( NULL == Fairy::LogicModelManager::getSingleton().getTemplateByName(weaponName) )
        {
            return;
        }

        if (mLeftHand)
        {
            mEquipModel->changeAttribute("LeftWeaponObj", weaponName);

          //  mLeftHandWeapon = Fairy::VariantCast<Fairy::LogicModel*>(mEquipModel->getAttribute("LeftWeaponObj"));
            mCurrentWeapon = Fairy::VariantCast<Fairy::LogicModel*>(mEquipModel->getAttribute("LeftWeaponObj"));
        }
        else
        {
            mEquipModel->changeAttribute("RightWeaponObj", weaponName);
        //    mRightHandWeapon = Fairy::VariantCast<Fairy::LogicModel*>(mEquipModel->getAttribute("RightWeaponObj"));
            mCurrentWeapon = Fairy::VariantCast<Fairy::LogicModel*>(mEquipModel->getAttribute("RightWeaponObj"));
        }

        addEffect();
    }
}

void WeaponSelectorDialog::OnWeaponEffectSelected( wxCommandEvent &event )
{
    if (mWeaponEffectList->GetSelection() == wxNOT_FOUND)
        return;

    //if (NULL == mRightHandWeapon && NULL == mLeftHandWeapon)
    //    return;

    mCurrentEffectName = mWeaponEffectList->GetStringSelection().c_str();    

    addEffect();
}

void WeaponSelectorDialog::addEffect(void)
{
    if (false == mCurrentEffectName.empty())
    {
        /*if (mLeftHand)
        {
            if (mLeftHandWeapon && mLeftHandWeapon->isLocatorExist("������Ч��"))
            {
                mLeftHandWeapon->delAllEffect();
                mCurrentEffectHandle = mLeftHandWeapon->addEffect(mCurrentEffectName, "������Ч��");
            }
        }
        else
        {
            if (mRightHandWeapon && mRightHandWeapon->isLocatorExist("������Ч��"))
            {
                mRightHandWeapon->delAllEffect();
                mCurrentEffectHandle = mRightHandWeapon->addEffect(mCurrentEffectName, "������Ч��");
            }
        }*/

        if ( mCurrentWeapon && mCurrentWeapon->isLocatorExist("������Ч��") )
        {
            mCurrentWeapon->delAllEffect();
            mCurrentEffectHandle = mCurrentWeapon->addEffect(mCurrentEffectName, "������Ч��", 
                Fairy::LogicModel::GTIT_ALL);
        }

    } 

    Fairy::Effect* effect = getCurrentEffect();

    if (effect)
        effect->setColour(mCurrentColour * mCurrentAlpha);
}

void WeaponSelectorDialog::OnColourButtonDown(wxCommandEvent &e)
{
    wxColourDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxColour colour;

        wxColourData retData = dialog.GetColourData();
        colour = retData.GetColour();

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            mCurrentColour.r, mCurrentColour.g, mCurrentColour.b);

        Fairy::Effect* effect = getCurrentEffect();
        
        if (effect)
            effect->setColour(mCurrentColour * mCurrentAlpha);

        // ���ð�ť��ɫ����ɫ�����ı�
        mColourButton->SetBackgroundColour(colour);

        setColourText();
    }
}

Fairy::Effect* WeaponSelectorDialog::getCurrentEffect(void)
{
    //if (NULL == mRightHandWeapon && NULL == mLeftHandWeapon ||
    //    mCurrentEffectHandle == 0)
    //    return NULL;

    //if (mLeftHand)
    //{
    //    return mLeftHandWeapon->getEffect(mCurrentEffectHandle);
    //}
    //else
    //{
    //    return mRightHandWeapon->getEffect(mCurrentEffectHandle);
    //} 

    if (NULL == mCurrentWeapon || mCurrentEffectHandle == 0)
        return NULL;

    return mCurrentWeapon->getEffect(mCurrentEffectHandle);
}

void WeaponSelectorDialog::OnAlphaTextChanged(wxCommandEvent &e)
{
    mCurrentAlpha = Ogre::StringConverter::parseReal(mAlphaTextCtrl->GetValue().c_str());

    Fairy::Effect* effect = getCurrentEffect();

    if (effect)
        effect->setColour(mCurrentColour * mCurrentAlpha);
}

void WeaponSelectorDialog::setColourText(void)
{
    mColourText->SetValue( Ogre::StringConverter::toString(mCurrentColour) );
}

void WeaponSelectorDialog::setAlphaText(void)
{
    mAlphaTextCtrl->SetValue( Ogre::StringConverter::toString(mCurrentAlpha) );
}