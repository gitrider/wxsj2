#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WXEffectEditDialog.h"
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

#include "WXEffectEditDialog.h"
#include "SceneManipulator.h"

#include "EffectSystem/FairyEffectManager.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairySkill.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"

#include "Core/FairySceneInfo.h"
#include "Core/FairyEffectObject.h"

#include <OgreMeshManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <OgreStringInterface.h>
#include <OgreStringVector.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "WXColourProperty.h"
#include "PropertyManager.h"
#include "WXMaterialEditorDialog.h"

#include "EffectSettingGlobalData.h"

IMPLEMENT_DYNAMIC_CLASS(WXEffectEditDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(WXEffectEditDialog, wxDialog)
	EVT_INIT_DIALOG(WXEffectEditDialog::OnInitDialog)
	EVT_PG_CHANGED(feID_PROPERTIES_VIEWER, WXEffectEditDialog::OnPropertyChanged)
	EVT_BUTTON(feID_BUTTON_ADD_ELEMENT, WXEffectEditDialog::OnAddElementButtonDown)
	EVT_BUTTON(feID_BUTTON_DEL_ELEMENT, WXEffectEditDialog::OnDelElementButtonDown)
	EVT_BUTTON(feID_BUTTON_SAVE_AS_EFFECT, WXEffectEditDialog::OnSaveAsButtonDown)
	EVT_BUTTON(ID_BUTTON_RESET_EFFECT, WXEffectEditDialog::OnResetEffectButtonDown)
	EVT_BUTTON(ID_BUTTON_EFFECT_ELEMENT_MATERIAL, WXEffectEditDialog::OnMaterialButtonDown)
    EVT_TEXT_ENTER(ID_TEXT_EFFECT_ALPHA, WXEffectEditDialog::OnEffectAlphaTextDown)

END_EVENT_TABLE()

wxPGConstants * WXEffectEditDialog::PopulateConstants(const Ogre::String& type)
{
	size_t id = reinterpret_cast<size_t>(&type);
	wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

	if (constants)
	{
		for (std::list<wxPGConstants*>::iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
		{
			if (constants == *it)
			{
				if (constants->UnRef())
					delete constants;

				mConstantList.erase(it);

				break;
			}
		}
	}
//	if (!constants)
//	{
		constants = wxPropertyGrid::CreateConstantsArray(id);

		if ( type == "Material" )
		{
			typedef std::list<Ogre::String> MaterialFileNameList;
			MaterialFileNameList materialFileNameList;

			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.material");
			for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{
				if ( it->archive->getName() == EFFECT_PATH)
				{
					materialFileNameList.push_back(it->filename);
				}
			}
			// ��һ��Ϊ�գ���ʾ�������Ԫ�صĲ�������ԭmesh�Ĳ���
			constants->Add("none");

			Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

			while ( resourceMapIterator.hasMoreElements() )
			{				
				Ogre::String matName = resourceMapIterator.peekNextValue()->getName();

				for ( MaterialFileNameList::iterator i = materialFileNameList.begin();
					i != materialFileNameList.end(); ++i )
				{
					if ( *i == resourceMapIterator.peekNextValue()->getOrigin() )
					{
						constants->Add(matName.c_str());
						break;
					}
				}

				resourceMapIterator.moveNext();
			}
		}
		else if ( type == "MeshName" )
		{			
			constants->Add("none");

			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.mesh");
			for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{
				if ( it->archive->getName() == EFFECT_PATH)
				{
					const Fairy::String& name = it->filename;
					constants->Add(name.c_str());
				}
			}
		}	
		else if ( type == "ParticleSystem" )
		{
			Ogre::ParticleSystemManager::ParticleSystemTemplateIterator iterator = Ogre::ParticleSystemManager::getSingleton().getTemplateIterator();

			while ( iterator.hasMoreElements() )
			{
				// ��ȡ������ϵͳ������
				Ogre::String parName = iterator.peekNextKey();

				constants->Add(parName.c_str());

				// ʹiterator������
				iterator.moveNext();
			}
		}

		registerConstants(constants);
//	}

	return constants;
}

WXEffectEditDialog::~WXEffectEditDialog()
{
	// ɾ�����ʶԻ���
	if (mMaterialEditorDialog)
	{
		mMaterialEditorDialog->Destroy();
		delete mMaterialEditorDialog;
		mMaterialEditorDialog = NULL;
	}

	for (std::list<wxPGConstants*>::const_iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
	{
		wxPGConstants* constants = *it;
		if (constants->UnRef())
			delete constants;
	}
	mConstantList.clear();
}

void WXEffectEditDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	if ( false == mInit )
	{
		wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 0, 0);
		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(0);

		mPropertiesViewer =
			new wxPropertyGridManager(this, feID_PROPERTIES_VIEWER,
			wxDefaultPosition, wxSize(300,300),
			wxCLIP_CHILDREN | wxCLIP_SIBLINGS |
			wxPG_BOLD_MODIFIED | // Modified values are drawn in bold font
			wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
			wxPG_TOOLTIPS | // Display tooltips for cell text that cannot be shown completely
			wxPG_TOOLBAR | // Include toolbar
			wxPG_DESCRIPTION | // Include description box
			wxFULL_REPAINT_ON_RESIZE |
			wxPGMAN_DEFAULT_STYLE); // Plus default style
		mPropertiesViewer->SetExtraStyle(wxPG_EX_CLASSIC_SPACING);
		sizer->Add(mPropertiesViewer, 0, wxFIXED_MINSIZE|wxGROW|wxALL, 2);	

		wxButton *addElementButton = new wxButton( this, feID_BUTTON_ADD_ELEMENT, wxT("Add Element"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( addElementButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *delElementButton = new wxButton( this, feID_BUTTON_DEL_ELEMENT, wxT("Del Element"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( delElementButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *saveAsButton = new wxButton( this, feID_BUTTON_SAVE_AS_EFFECT, wxT("SaveAs"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( saveAsButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *resetEffectButton = new wxButton( this, ID_BUTTON_RESET_EFFECT, wxT("Reset Effect"), wxDefaultPosition, wxSize(80,-1), 0 );
		sizer->Add( resetEffectButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *materialButton = new wxButton( this, ID_BUTTON_EFFECT_ELEMENT_MATERIAL, wxT("Material"), wxDefaultPosition, wxSize(80,-1), 0 );
		sizer->Add( materialButton, 0, wxALIGN_CENTER|wxALL, 5 );

        mAlphaTextCtrl = new wxTextCtrl( this, ID_TEXT_EFFECT_ALPHA, _("1"), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
        sizer->Add( mAlphaTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );


		this->SetSizer(sizer);
		sizer->SetSizeHints(this);

		mInit = true;
	}
}

wxPGProperty*
WXEffectEditDialog::CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::StringInterface *basic)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::String value = basic->getParameter(name.c_str());
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			wxPGConstants* constants = PopulateConstants(propertyDef.name);
			if (constants)
			{
				// ������combobox
				property = wxEnumProperty(name, name, *constants);
				// ���óɵ�ǰ���������ֵ
				property->SetValueFromString(value.c_str(),0);
			}
			
			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);
	
	return property;
}

void
WXEffectEditDialog::registerConstants(wxPGConstants* constants)
{
	assert(constants);
	constants->Ref();
	mConstantList.push_back(constants);
}

void
WXEffectEditDialog::OnPropertyChanged(wxPropertyGridEvent& e)
{
	// Skip parent properties
    if (e.GetPropertyPtr()->GetParentingType() != 0)
        return;
        
	wxPGId id = e.GetPropertyPtr()->GetId();
	wxPGId parentId = mPropertiesViewer->GetPropertyParent(id);

	const wxString &parentLabel = mPropertiesViewer->GetPropertyLabel(parentId);

	unsigned short index = mPropertiesViewer->GetPropertyIndex(parentId);

	Fairy::String propertyName = e.GetPropertyName().c_str();
	Fairy::String propertyValue = e.GetPropertyValueAsString().c_str();

	Fairy::Scene::ObjectsByTypeRange effects =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::EffectObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
	{
		const Fairy::ObjectPtr& object = *it;
		const Fairy::EffectObject* effectObject = static_cast<Fairy::EffectObject *>(object.get());

		Fairy::Effect *effect = effectObject->getEffect();

		assert (effect);

		// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
		if ( effect->getTemplateName() == mEffect->getTemplateName() )
		{
			if (index < effect->getNumElements())
			{
				Fairy::EffectElement *element = effect->getElement(index);
				assert (element);

				element->setParameter(propertyName, propertyValue);
			}
			else
			{
				effect->setParameter(propertyName, propertyValue);
			}
		}
	}

    // ���µ�ǰ��skill
    if (gEffectSettingGlobalData.mCurrentSkill)
    {
        for ( unsigned short i = 0; i < gEffectSettingGlobalData.mCurrentSkill->getNumAnimationEffectInfos(); ++i )
        {
            Fairy::AnimationEffectInfo *effectInfo = gEffectSettingGlobalData.mCurrentSkill->getAnimationEffectInfo(i);
            assert (effectInfo);

            Fairy::Effect *effect = effectInfo->getEffect();

            assert (effect);

            // ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
            if ( effect->getTemplateName() == mEffect->getTemplateName() )
            {
                if (index < effect->getNumElements())
                {
                    Fairy::EffectElement *element = effect->getElement(index);
                    assert (element);

                    element->setParameter(propertyName, propertyValue);
                }
                else
                {
                    effect->setParameter(propertyName, propertyValue);
                }
            }
        }
    }
}

void WXEffectEditDialog::InitEffectDialog( Fairy::Effect *effect, Fairy::SceneManipulator *sceneManipulator )
{
	assert ( effect && sceneManipulator );

	mEffect = effect;
	mSceneManipulator = sceneManipulator;

	mPropertiesViewer->Freeze();
	mPropertiesViewer->GetGrid()->Clear();

	// ����element������
	for ( size_t i=0; i < mEffect->getNumElements(); ++i )
	{
		Fairy::EffectElement *element = mEffect->getElement(i);
		assert (element);

		wxPGId id =	mPropertiesViewer->Append( wxCustomProperty(element->getType().c_str()) );

		if ( id.IsOk() )
		{
			mPropertiesViewer->DisableProperty(id);
			const Ogre::ParameterList &paraList = element->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], element) );
			}
		}		
	}

	// ����effect����
	const Ogre::ParameterList &effectParaList = effect->getParameters();

	for ( size_t paraCount=0; paraCount < effectParaList.size(); ++paraCount )
	{
		mPropertiesViewer->Append( CreateProperty(effectParaList[paraCount], effect) );
	}

	mPropertiesViewer->Thaw();
	mPropertiesViewer->ExpandAll();
	mPropertiesViewer->Refresh();
}

void WXEffectEditDialog::OnSaveButtonDown(wxCommandEvent &e)
{
	
}

void WXEffectEditDialog::OnResetEffectButtonDown(wxCommandEvent &e)
{
	// �����е�effect object��Ȼ�ǵ��õ�createSceneNode(SceneNode *)��������ڵ��ǿ������ô��ڵ�
	// ������removeʱû��Ҫ�����Ӹ��ڵ���ж����
	Fairy::EffectManager::getSingleton().removeEffect(mEffect,false);
	Fairy::EffectManager::getSingleton().createEffect(mEffect->getTemplateName());
}

void WXEffectEditDialog::OnEffectAlphaTextDown(wxCommandEvent& e)
{
    float alphaValue = Ogre::StringConverter::parseReal(mAlphaTextCtrl->GetValue().c_str());

    mEffect->setColour( Ogre::ColourValue(alphaValue, alphaValue, alphaValue, alphaValue) );
}

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC_CLASS(AddEffectElementDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddEffectElementDialog, wxDialog)
EVT_INIT_DIALOG(AddEffectElementDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddEffectElementDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item2 = new wxComboBox( this, feID_COMBOCTRL_ELEMENT_TYPE, wxT(""), wxDefaultPosition, wxSize(100,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item1, 0, wxALIGN_CENTER|wxALL, 10 );

	wxFlexGridSizer *item3 = new wxFlexGridSizer( 2, 0, 0 );

	wxButton *item4 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item4->SetDefault();
	item3->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item5 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item3->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item3, 0, wxALIGN_CENTER|wxALL, 10 );

	this->SetSizer( sizer );
	sizer->SetSizeHints( this );

	Fairy::EffectManager::ElementFactoryIterator it = 
		Fairy::EffectManager::getSingleton().getElementFactoryIterator();

	while ( it.hasMoreElements() )
	{
		Ogre::String elementType = it.peekNextKey();

		item2->AppendString(elementType.c_str());

		it.moveNext();
	}

	item2->SetSelection(0);
	/*for ( size_t i=0; i<mBrushNameList.size(); ++i )
	{
		item2->AppendString(mBrushNameList[i]);
	}

	item2->SetValue(mBrushNameList[0]);*/
}

void WXEffectEditDialog::OnAddElementButtonDown(wxCommandEvent &e)
{
	AddEffectElementDialog *dlg = new AddEffectElementDialog(this, wxID_ANY,
		_("��ЧԪ������"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		Ogre::String type = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_ELEMENT_TYPE),wxComboBox)->GetValue().c_str();

		if ( false == type.empty() )
		{
			Fairy::Scene::ObjectsByTypeRange effects =
				mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::EffectObject::msType);

			for (Fairy::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
			{
				const Fairy::ObjectPtr& object = *it;
				const Fairy::EffectObject* effectObject = static_cast<Fairy::EffectObject *>(object.get());

				Fairy::Effect *effect = effectObject->getEffect();

				assert (effect);

				// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
				if ( effect->getTemplateName() == mEffect->getTemplateName() )
				{
					effect->addElement(type);
				}
			}

			InitEffectDialog(mEffect, mSceneManipulator);
		}		
	}

	dlg->Destroy();
}

void WXEffectEditDialog::OnDelElementButtonDown(wxCommandEvent &e)
{
	wxPGId id = mPropertiesViewer->GetSelectedProperty();

	if ( true == id.IsOk() && false == mPropertiesViewer->IsPropertyEnabled(id) )
	{
		unsigned short index = mPropertiesViewer->GetPropertyIndex(id);
		
		Fairy::Scene::ObjectsByTypeRange effects =
			mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::EffectObject::msType);

		for (Fairy::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
		{
			const Fairy::ObjectPtr& object = *it;
			const Fairy::EffectObject* effectObject = static_cast<Fairy::EffectObject *>(object.get());

			Fairy::Effect *effect = effectObject->getEffect();

			assert (effect);

			if (index < effect->getNumElements())
			{
				// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
				if ( effect->getTemplateName() == mEffect->getTemplateName() )
				{
					effect->destroyElement(index);
				}

				mPropertiesViewer->Delete(id);
			}
		}
	}
}

IMPLEMENT_DYNAMIC_CLASS(SaveAsEffectDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveAsEffectDialog, wxDialog)
EVT_INIT_DIALOG(SaveAsEffectDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveAsEffectDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxFlexGridSizer *item0 = new wxFlexGridSizer( 2, 0, 0 );

	wxStaticText *item1 = new wxStaticText( this, ID_TEXT, wxT("Script File"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs2 = (wxString*) NULL;
	wxComboBox *item2 = new wxComboBox( this, ID_COMBO_SAVE_AS_EFFECT, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, strs2, wxCB_DROPDOWN );
	item0->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item3 = new wxStaticText( this, ID_TEXT, wxT("Template Name"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item4 = new wxTextCtrl( this, ID_TEXTCTRL_SAVE_AS_EFFECT, wxT(""), wxDefaultPosition, wxSize(200,-1), 0 );
	item0->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item5 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item6 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	this->SetSizer( item0 );
	item0->SetSizeHints( this );

	Ogre::StringVector loadedScripts;
	Fairy::EffectManager::getSingleton().getLoadedScripts( loadedScripts );

	for ( size_t i=0; i<loadedScripts.size(); ++i )
	{
		if (loadedScripts[i] != "EmptyEffect.effect")
			item2->AppendString(loadedScripts[i]);
	}

	Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mEffect->getTemplateName());

	if ( fileName == "EmptyEffect.effect" )
		item2->SetSelection(0);
	else
		item2->SetValue(fileName);

	mComboBox = item2;

	if (mEffect->getTemplateName() != "EffectEmpty")
		item4->SetValue(mEffect->getTemplateName());

	mTextCtrl = item4;
}

void WXEffectEditDialog::OnSaveAsButtonDown(wxCommandEvent &e)
{
	SaveAsEffectDialog *dlg = new SaveAsEffectDialog(this, wxID_ANY,
		_("������Ч"), mEffect);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
	//	bool append = true;
		// ���ģ�����Ʋ�һ���ˣ�˵����Ҫ���һ���µ�ģ��
		/*if ( dlg->mTextCtrl->GetValue().c_str() == mEffect->getTemplateName() )
		{
			append = false;
		}*/
		Ogre::StringVector templates;

		const Ogre::String fileName = dlg->mComboBox->GetValue().c_str();
		const Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		Ogre::StringVector strings = Ogre::StringUtil::split( fileName, "." );
		if ( (strings.size() != 2) || (strings[1] != "effect") )
		{
			wxMessageBox ( _("The Script File Name Is Wrong!") );
			return;
		}

		Fairy::EffectManager::getSingleton().getTemplatesFromScriptFile(fileName, templates);

		std::ofstream outFile;

		Ogre::String pathFileName = EFFECT_PATH+fileName;
		outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!outFile.is_open())
		{
			return;
		}
		
		bool newTemplate = true;

		// �����е�ģ�嶼д����ļ���
		for ( size_t i=0; i<templates.size(); ++i )
		{
		//	if (templates[i] != mEffect->getTemplateName())
			///{
				Fairy::Effect *effect = Fairy::EffectManager::getSingleton().getTemplate(templates[i]);
				assert (effect);

				if (effect->getTemplateName() == dlg->mTextCtrl->GetValue().c_str())
				{
					saveEffect(mEffect, dlg->mTextCtrl->GetValue().c_str(), outFile );

					*effect = *mEffect;

					newTemplate = false;
				}
				else
					saveEffect(effect, effect->getTemplateName(), outFile);
		
		}


		if (newTemplate)
		{
			saveEffect(mEffect, templateName, outFile );
			mEffect->setTemplateName(templateName);

			// ˢ��EffectManager�е�ģ������
			Fairy::Effect *effect = Fairy::EffectManager::getSingleton().createEffectTemplate(mEffect->getTemplateName());

			*effect = *mEffect;

			Fairy::EffectManager::getSingleton().addToEffectTemplateScriptFileMap(templateName, fileName);
		}

		outFile.close();
	}

	dlg->Destroy();
}

void WXEffectEditDialog::OnMaterialButtonDown(wxCommandEvent &e)
{
	wxPGId id = mPropertiesViewer->GetSelectedProperty();

	if ( true == id.IsOk() && false == mPropertiesViewer->IsPropertyEnabled(id) )
	{
		unsigned short index = mPropertiesViewer->GetPropertyIndex(id);

		if ( mPropertiesViewer->GetPropertyLabel(id) == "Beam" ||
			mPropertiesViewer->GetPropertyLabel(id) == "Projector" ||
            mPropertiesViewer->GetPropertyLabel(id) == "BillboardSet" )
		{
			if ( NULL == mMaterialEditorDialog )
			{
				mMaterialEditorDialog = new MaterialEditorDialog(this, wxID_ANY, _("���ʱ༭��"));
			}
			if ( false == mMaterialEditorDialog->IsShown() )
				mMaterialEditorDialog->Show();	

			gEffectSettingGlobalData.mCurrentEffectElementIndex = index;
			if ( mPropertiesViewer->GetPropertyLabel(id) == "Beam" )
				gEffectSettingGlobalData.mCurrentEditEffectType = CEFT_EFFECT_BEAM;
			else if ( mPropertiesViewer->GetPropertyLabel(id) == "Projector" )
			{
				gEffectSettingGlobalData.mCurrentEditEffectType = CEFT_EFFECT_PROJECTOR;
			}
            else if ( mPropertiesViewer->GetPropertyLabel(id) == "BillboardSet" )
            {
                gEffectSettingGlobalData.mCurrentEditEffectType = CEFT_EFFECT_BILLBOARDSET;
            }

			gEffectSettingGlobalData.mCurrentEffect = mEffect;

			mMaterialEditorDialog->InitDialog(mSceneManipulator);
		}
	}
}

void WXEffectEditDialog::saveEffect(Fairy::Effect *effect, const Ogre::String &templateName, std::ofstream &of)
{
	assert (effect);

	of << "\n";
	of << "effect " << templateName << "\n";
	of << "{";


	for ( size_t i=0; i < effect->getNumElements(); ++i )
	{
		Fairy::EffectElement *element = effect->getElement(i);
		assert (element);

		of << "\n";
		of << "\telement " << element->getType() << "\n";
		of << "\t{\n";

		const Ogre::ParameterList &paraList = element->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			of << "\t\t" << paraList[paraCount].name << "\t" << element->getParameter( paraList[paraCount].name ) << "\n";
		}		

		of << "\t}\n";
	}

	const Ogre::ParameterList &paraList = effect->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		of << "\n\t" << paraList[paraCount].name << "\t" << effect->getParameter( paraList[paraCount].name ) << "\n";
	}	

	of << "}\n";
}
