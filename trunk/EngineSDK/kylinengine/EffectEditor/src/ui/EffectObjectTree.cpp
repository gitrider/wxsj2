

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/log.h"
#endif

#include "wx/colordlg.h"

#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"

#include "math.h"

#ifdef __WIN32__
// this is not supported by native control
#define NO_VARIABLE_HEIGHT
#endif

#include "commctrl.h"
#include "EffectObjectTree.h"
#include "EffectObjectEditor.h"
#include "EffectObjectPropertyEditor.h"
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystemRenderer.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairyMeshElement.h"
#include "EffectSystem/FairyBillboardSetElement.h"
#include "EffectSystem/FairyBeamElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyCylinderElement.h"
#include "OgreParticleSystemManager.h"


static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TV_HITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return (HTREEITEM)TreeView_HitTest(hwndTV, &tvht);
}
static const int feID_COPY_PARTICLE =wxNewId();
static const int feID_PASET_PARTICLE =wxNewId();

static const int feID_COPY_EFFECT_ELEMENT =wxNewId();
static const int feID_PASET_EFFECT_ELEMENT =wxNewId();

//Menu for effectObjectEditor toolbar
static const int feID_MENU_EFFECT_OBJ_DELETE_ELEMENT		= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_PARTICLE			= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_MESH				= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_BILLBOARDSET		= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_PROJECTOR			= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_BEAM				= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_POINTLIGHT		= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_PARTICLESYSTEM	= wxNewId();

static const int feID_MENU_EFFECT_OBJ_NEW_EMITTER			= wxNewId();
static const int feID_MENU_EFFECT_OBJ_NEW_AFFECTOR			= wxNewId();

IMPLEMENT_DYNAMIC_CLASS(EffectObjectTree, wxTreeCtrl)

BEGIN_EVENT_TABLE(EffectObjectTree, wxTreeCtrl)
EVT_TREE_BEGIN_LABEL_EDIT(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnBeginLabelEdit)
EVT_TREE_END_LABEL_EDIT(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnEndLabelEdit)
EVT_TREE_DELETE_ITEM(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnDeleteItem)
EVT_TREE_SET_INFO(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnSetInfo)
EVT_TREE_ITEM_EXPANDED(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemExpanded)
EVT_TREE_ITEM_EXPANDING(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemExpanding)
EVT_TREE_ITEM_COLLAPSED(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemCollapsed)
EVT_TREE_ITEM_COLLAPSING(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemCollapsing)

EVT_TREE_SEL_CHANGED(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnSelChanged)
EVT_TREE_SEL_CHANGING(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnSelChanging)
EVT_TREE_KEY_DOWN(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnTreeKeyDown)
EVT_TREE_ITEM_ACTIVATED(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemActivated)

// so many differents ways to handle right mouse button clicks...
EVT_CONTEXT_MENU(EffectObjectTree::OnContextMenu)
// EVT_TREE_ITEM_MENU is the preferred event for creating context menus
// on a tree control, because it includes the point of the click or item,
// meaning that no additional placement calculations are required.
EVT_TREE_ITEM_MENU(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemMenu)
EVT_TREE_ITEM_RIGHT_CLICK(ID_EFFECT_OBJECT_TREE, EffectObjectTree::OnItemRClick)
EVT_MENU(feID_COPY_PARTICLE,/*EffectObjectTree::OnCopyParticleSystem*/EffectObjectTree::OnMenu)
EVT_MENU(feID_PASET_PARTICLE,/*EffectObjectTree::OnPasteParticleSystem*/EffectObjectTree::OnMenu)

EVT_MENU(feID_COPY_EFFECT_ELEMENT,EffectObjectTree::OnMenu)
EVT_MENU(feID_PASET_EFFECT_ELEMENT,EffectObjectTree::OnMenu)

EVT_MENU(feID_MENU_EFFECT_OBJ_DELETE_ELEMENT,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_PARTICLE,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_MESH,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_BILLBOARDSET,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_PROJECTOR,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_BEAM,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_POINTLIGHT,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_PARTICLESYSTEM,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_EMITTER,EffectObjectTree::OnMenu)
EVT_MENU(feID_MENU_EFFECT_OBJ_NEW_AFFECTOR,EffectObjectTree::OnMenu)

EVT_RIGHT_DOWN(EffectObjectTree::OnRMouseDown)
EVT_RIGHT_UP(EffectObjectTree::OnRMouseUp)
EVT_RIGHT_DCLICK(EffectObjectTree::OnRMouseDClick)
END_EVENT_TABLE()

int EffectObjectTree::OnCompareItems(const wxTreeItemId& item1,
							  const wxTreeItemId& item2)
{
	if ( m_reverseSort )
	{
		// just exchange 1st and 2nd items
		return wxTreeCtrl::OnCompareItems(item2, item1);
	}
	else
	{
		return wxTreeCtrl::OnCompareItems(item1, item2);
	}
}

void EffectObjectTree::GetItemsRecursively(const wxTreeItemId& idParent,
									 wxTreeItemIdValue cookie)
{
	wxTreeItemId id;

	if ( !cookie )
		id = GetFirstChild(idParent, cookie);
	else
		id = GetNextChild(idParent, cookie);

	if ( !id.IsOk() )
		return;

	wxString text = GetItemText(id);
	wxLogMessage(text);

	if (ItemHasChildren(id))
		GetItemsRecursively(id);

	GetItemsRecursively(idParent, cookie);
}


void EffectObjectTree::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	wxString text;
	if ( item.IsOk() )
		text << _T('"') << GetItemText(item).c_str() << _T('"');
	else
		text = _T("invalid item");
	wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

long EffectObjectTree::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if(GetHwnd())
    { 
       
        switch ( nMsg )
        {
        case WM_LBUTTONDOWN: 
            int x = GET_X_LPARAM(lParam),
                y = GET_Y_LPARAM(lParam);
            HTREEITEM htItem = GetItemFromPoint(GetHwnd(), x, y);

            if ( htItem)
            {
                if ( wParam & MK_CONTROL )
                { 
                    SetFocus();

                    // toggle selected state
                    wxTreeCtrl::ToggleItemSelection(htItem);

                    tagNMTREEVIEWW ntv;

                    NMHDR hdr;
                    hdr.hwndFrom = GetHwnd();
                    hdr.idFrom = GetId();
                    hdr.code = TVN_SELCHANGEDW;

                    TVITEMW itemNew;
                    if(IsSelected(htItem))
                        itemNew.hItem = HTREEITEM(htItem);
                    else
                        itemNew.hItem = NULL;

                    ntv.hdr = hdr;
                    ntv.itemNew = itemNew;

                    WXLPARAM rc;
                    wxTreeCtrl::MSWOnNotify(GetId(), (LPARAM)&ntv, &rc); 

                    ::SetFocus(GetHwnd());                  

                    return 0L;
                }
            }
        }



    }
    return wxTreeCtrl::MSWWindowProc(nMsg,wParam,lParam);

}

void EffectObjectTree::OnMenu(wxCommandEvent & e)
{
	int id = e.GetId();
	if(id == feID_COPY_PARTICLE)
	{
		OnCopyParticleSystem(e);
	}
	else if(id == feID_PASET_PARTICLE)
	{
		OnPasteParticleSystem(e);
	}
	else if(id == feID_COPY_EFFECT_ELEMENT)
	{
		OnCopyEffectElement(e);
	}
	else if(id == feID_PASET_EFFECT_ELEMENT)
	{
		OnPasteEffectElement(e);
	}
	else
	{
		OnEditorToolBarMenu(e);		
	}
}

void EffectObjectTree::OnEditorToolBarMenu(wxCommandEvent &e)
{
	EffectObjectEditor* parent = static_cast <EffectObjectEditor*>(this->GetParent());
	if (parent == NULL)
	{
		return;
	}

	int id = e.GetId();
	if(id == feID_MENU_EFFECT_OBJ_DELETE_ELEMENT)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_DELETE_ElEMENT);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_PARTICLE)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_PARTICLE);
	}
	else if(id ==  feID_MENU_EFFECT_OBJ_NEW_MESH)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_MESH);
	}
	else if( id == feID_MENU_EFFECT_OBJ_NEW_BILLBOARDSET)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_BILLBOARDSET);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_PROJECTOR)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_PROJECTOR);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_BEAM)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_BEAM);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_POINTLIGHT)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_POINTLIGHT);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_PARTICLESYSTEM)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_PARTICLE_SYSTEM);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_EMITTER)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_EMITTER);
	}
	else if(id == feID_MENU_EFFECT_OBJ_NEW_AFFECTOR)
	{
		parent->OnNotifyToolBarCtrl(EffectObjectEditor::TC_NEW_AFFECTOR);
	}
}
//复制粒子数据
void EffectObjectTree::OnCopyParticleSystem(wxCommandEvent &e)
{
	if(!mSelectEffectObject)
		return;
	if(!mCopyParticleSystem)
	{
		mCopyParticleSystem = Ogre::ParticleSystemManager::getSingleton().getTemplate("temp_copy_particlesystem");
		if(!mCopyParticleSystem)
			mCopyParticleSystem = Ogre::ParticleSystemManager::getSingleton().createTemplate(
			"temp_copy_particlesystem",Ogre::String("General"));

	}
	Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(mSelectEffectObject);
	if(pParticleSystem)
	{
		*mCopyParticleSystem = *pParticleSystem;
	}
}
//粘贴粒子数据
void EffectObjectTree::OnPasteParticleSystem(wxCommandEvent &e)
{
	if(!mCopyParticleSystem || !mSelectEffectObject)
		return;
	Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(mSelectEffectObject);
	if(pParticleSystem)
	{
		*pParticleSystem = *mCopyParticleSystem;
		EffectObjectEditor* pEditor = dynamic_cast<EffectObjectEditor*>(m_parent);
		pEditor->ReInitEffectEditor();
		SelectParticleSystem(pParticleSystem,true);
	}

}
//复制效果元素
void EffectObjectTree::OnCopyEffectElement(wxCommandEvent &e)
{
	if(!mSelectEffectObject)
		return;

	Fairy::EffectElement* pEffectElement = dynamic_cast<Fairy::EffectElement*>(mSelectEffectObject);
	if(pEffectElement)
	{
		const Ogre::String EffectType = pEffectElement->getType();
		if(mCopyEffectElement)
		{
			if(mCopyEffectElementType != EffectType)
			{
				delete mCopyEffectElement;
				mCopyEffectElement = NULL;
				mCopyEffectElement = Fairy::EffectManager::getSingleton().createElement(EffectType);
			}
		}
		else
		{
			mCopyEffectElement = Fairy::EffectManager::getSingleton().createElement(EffectType);
		}
		pEffectElement->copyParametersTo(mCopyEffectElement);
		mCopyEffectElementType = EffectType;
		

	}

}
//粘贴效果元素
void EffectObjectTree::OnPasteEffectElement(wxCommandEvent &e)
{
	if(!mCopyEffectElement || !mSelectEffectObject)
		return;
	Fairy::EffectElement* pEffectElement = dynamic_cast<Fairy::EffectElement*>(mSelectEffectObject);
	if(pEffectElement)
	{
		if(mCopyEffectElementType != pEffectElement->getType())
			return;
		mCopyEffectElement->copyParametersTo(pEffectElement);
		EffectObjectEditor* pEditor = dynamic_cast<EffectObjectEditor*>(m_parent);
		pEditor->ReInitEffectEditor();
		SelectElementItem(pEffectElement);
	}
}
void EffectObjectTree::AddEffectToTree(Fairy::Effect *pEffect,const Ogre::String &templateName)
{
	int iEffectImage = EffectObjectTree::TreeCtrlIcon_Effect;
	wxTreeItemId EffectRootId = AddRoot(templateName,
		iEffectImage, iEffectImage,
		new EffectTreeItemData(wxT("Effect root item"),templateName,pEffect));

	int iPorpertyImage = EffectObjectTree::TreeCtrlIcon_EffectProperty;

	if ( EffectRootId.IsOk() )
	{
		const Ogre::ParameterList &paraList = pEffect->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String name = paraList[paraCount].name;
			Ogre::String value = pEffect->getParameter(paraList[paraCount].name);

			wxTreeItemId itemId =AppendItem( EffectRootId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
				new EffectTreeItemData(wxT("Effect property item"),name,pEffect));
		}
	}

	int iPointLightImage = EffectObjectTree::TreeCtrlIcon_PointLight;
	int iBillboardSetImage = EffectObjectTree::TreeCtrlIcon_BillboardSet;
	int iParticleImage = EffectObjectTree::TreeCtrlIcon_ParticleItem;
	int iProjectorImage = EffectObjectTree::TreeCtrlIcon_Projector;
	int iMeshImage = EffectObjectTree::TreeCtrlIcon_Mesh;
	int iBeamImage = EffectObjectTree::TreeCtrlIcon_Beam;
	int iCylinderImage = EffectObjectTree::TreeCtrlIcon_Cylinder;


	for ( unsigned short i=0; i<pEffect->getNumElements(); ++i )
	{
		Fairy::EffectElement *pElement = pEffect->getElement(i);
		assert (pElement);

		wxTreeItemId ElementId;
		Ogre::String ElementType = pElement->getType();
		Ogre::String ElementName;
		int iElementImage = iPointLightImage;
		if(pElement->getType() == "Particle")
		{
			iElementImage = iParticleImage;
			Fairy::ParticleElement* pParticle = dynamic_cast<Fairy::ParticleElement*>(pElement);
			if (pParticle)
			{
				ElementName =pParticle->getParticleSystemName();
			}
		}
		else if(pElement->getType() == "Mesh")
		{
			iElementImage = iMeshImage;
			Fairy::MeshElement* pMesh = dynamic_cast<Fairy::MeshElement*>(pElement);
			if (pMesh)
			{
				ElementName =pMesh->getMaterialName();
			}
		}
		else if(pElement->getType() == "BillboardSet")
		{
			iElementImage = iBillboardSetImage;
			Fairy::BillboardSetElement* pBillboardSet = dynamic_cast<Fairy::BillboardSetElement*>(pElement);
			if (pBillboardSet)
			{
				ElementName =pBillboardSet->getBillboardMaterial();
			}
		}
		else if(pElement->getType() == "Projector")
		{
			iElementImage = iProjectorImage;
			Fairy::ProjectorElement* pProjector = dynamic_cast<Fairy::ProjectorElement*>(pElement);
			if (pProjector)
			{
				ElementName =pProjector->getMaterialName();
			}
		}
		else if(pElement->getType() == "Beam")
		{
			iElementImage = iBeamImage;
			Fairy::BeamElement* pBeam = dynamic_cast<Fairy::BeamElement*>(pElement);
			if (pBeam)
			{
				ElementName =pBeam->getMaterialName();
			}
		}
		else if(pElement->getType() == "PointLight")
		{
			iElementImage = iPointLightImage;
		}
		else if(pElement->getType() == "Cylinder")
		{
			iElementImage = iCylinderImage;
			Fairy::CylinderElement* pCylinder = dynamic_cast<Fairy::CylinderElement*>(pElement);
			if (pCylinder)
			{
				ElementName =pCylinder->getMaterialName();
			}
		}
		Ogre::String ElementItemName = ElementType + ":" + ElementName;
		ElementId= AppendItem(EffectRootId,ElementItemName.c_str(),
			iElementImage, iElementImage,
			new EffectTreeItemData(wxT("Effect element"),pElement->getType().c_str(),pElement));

		if ( ElementId.IsOk() )
		{
			const Ogre::ParameterList &paraList = pElement->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = pElement->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( ElementId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new EffectTreeItemData(wxT("Effect element item"),name,pElement));

			}
		}		
	}
	Expand(EffectRootId);

}

void EffectObjectTree::AddParticleToTree(Ogre::ParticleSystem* pParticleSystem,const Ogre::String &templateName)
{
	wxTreeItemId EffectRootId = GetRootItem();
	if(!EffectRootId.IsOk())
		return;

	int iParticleImage = EffectObjectTree::TreeCtrlIcon_Particle;
	wxTreeItemId particleId = AppendItem(EffectRootId,templateName,
		iParticleImage, iParticleImage,
		new EffectTreeItemData(wxT("Particle item"),templateName,pParticleSystem));

	int iPorpertyImage = EffectObjectTree::TreeCtrlIcon_EffectProperty;

	int iMaterialImage = EffectObjectTree::TreeCtrlIcon_Material;
	int iRenderItemImage = EffectObjectTree::TreeCtrlIcon_ParticleRenderItem;



	wxTreeItemId BasicSettingId= AppendItem(particleId,wxT("BasicSetting"),
		iPorpertyImage, iPorpertyImage,
		new EffectTreeItemData(wxT("Particle BasicSetting"),wxT("Particle BasicSetting"),pParticleSystem));

	if ( BasicSettingId.IsOk() )
	{
		const Ogre::ParameterList &paraList = pParticleSystem->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String name = paraList[paraCount].name;
			Ogre::String value = pParticleSystem->getParameter(paraList[paraCount].name);
			if(name == "material")
			{
				AppendItem( BasicSettingId,(name + ":" + value).c_str(),iMaterialImage,iMaterialImage,
					new EffectTreeItemData(wxT("Particle BasicSetting item material"),name,pParticleSystem));
			}
			else if(name == "renderer")
			{
				AppendItem( BasicSettingId,(name + ":" + value).c_str(),iRenderItemImage,iRenderItemImage,
					new EffectTreeItemData(wxT("Particle BasicSetting item renderer"),name,pParticleSystem));
			}
			else
			{
				AppendItem( BasicSettingId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new EffectTreeItemData(wxT("Particle BasicSetting item"),name,pParticleSystem));
			}
		}
	}

	int iRenderImage = EffectObjectTree::TreeCtrlIcon_ParticleRender;

	Ogre::ParticleSystemRenderer *renderer = pParticleSystem->getRenderer();

	assert (renderer);

	wxTreeItemId RenderId = AppendItem(particleId,renderer->getType().c_str(),
		iRenderImage, iRenderImage,
		new EffectTreeItemData(wxT("Particle render"),renderer->getType().c_str(),renderer));

	if ( RenderId.IsOk() )
	{

		const Ogre::ParameterList &paraList = renderer->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String name = paraList[paraCount].name;
			Ogre::String value = renderer->getParameter(paraList[paraCount].name);

			wxTreeItemId itemId =AppendItem( RenderId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
				new EffectTreeItemData(wxT("Particle render item"),name,renderer));

		}
	}
	int iEmitterImage = EffectObjectTree::TreeCtrlIcon_ParticleEmitter;

	for ( unsigned short i=0; i<pParticleSystem->getNumEmitters(); ++i )
	{
		Ogre::ParticleEmitter *emitter = pParticleSystem->getEmitter(i);
		assert (emitter);


		wxTreeItemId EimtterId = AppendItem(particleId,emitter->getType().c_str(),
			iEmitterImage, iEmitterImage,
			new EffectTreeItemData(wxT("Particle emitter"),emitter->getType().c_str(),emitter));

		if ( EimtterId.IsOk() )
		{
			const Ogre::ParameterList &paraList = emitter->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = emitter->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( EimtterId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new EffectTreeItemData(wxT("Particle emitter item"),name,emitter));

			}
		}		
	}

	int iAffectorImage = EffectObjectTree::TreeCtrlIcon_ParticleAffector;

	for ( unsigned short i=0; i<pParticleSystem->getNumAffectors(); ++i )
	{
		Ogre::ParticleAffector *affector = pParticleSystem->getAffector(i);
		assert (affector);

		wxTreeItemId AffectorId = AppendItem(particleId,affector->getType().c_str(),
			iAffectorImage, iAffectorImage,
			new EffectTreeItemData(wxT("Particle affector"),affector->getType(),affector));

		if ( AffectorId.IsOk() )
		{

			const Ogre::ParameterList &paraList = affector->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = affector->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( AffectorId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new EffectTreeItemData(wxT("Particle affector item"),name,affector));


			}
		}		
	}
	Expand(EffectRootId);

}

void EffectObjectTree::SelectElementItem(Fairy::EffectElement* element)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
		Fairy::EffectElement* pElement = (Fairy::EffectElement*)pItemData->GetData();
		if(pItemData->GetDesc() == "Effect element" && pElement == element)
		{
			this->SelectItem(childId);
			this->Expand(childId);
			return;
		}
		childId = GetNextSibling(childId);
	}

}
void EffectObjectTree::CreateImageList(int size)
{
	if ( size == -1 )
	{
		SetImageList(NULL);
		return;
	}
	if ( size == 0 )
		size = m_imageSize;
	else
		m_imageSize = size;

	// Make an image list containing small icons
	wxImageList *images = new wxImageList(size, size, true);

	// should correspond to TreeCtrlIcon_xxx enum
	wxBusyCursor wait;
	wxBitmap bitmaps[15];
	bitmaps[0] = wxBITMAP(EFFECT);
	bitmaps[1] = wxBITMAP(POINTLIGHT);
	bitmaps[2] = wxBITMAP(BILLBOARDSET);
	bitmaps[3] = wxBITMAP(PARTICLEITEM);
	bitmaps[4] = wxBITMAP(PROJECTOR);
	bitmaps[5] = wxBITMAP(MESH);
	bitmaps[6] = wxBITMAP(CYLINDER);
	bitmaps[7] = wxBITMAP(EFFECTPROPERTY);
	bitmaps[8] = wxBITMAP(BEAM);
	bitmaps[9] = wxBITMAP(MATERIAL);
	bitmaps[10] = wxBITMAP(PARTICLE);
	bitmaps[11] = wxBITMAP(PARTICLERENDER);
	bitmaps[12] = wxBITMAP(PARTICLERENDERITEM);
	bitmaps[13] = wxBITMAP(PARTICLEEMITTER);
	bitmaps[14] = wxBITMAP(PARTICLEAFFECTOR);
	

	int sizeOrig = bitmaps[0].GetWidth();
	for ( size_t i = 0; i < WXSIZEOF(bitmaps); i++ )
	{
		if ( size == sizeOrig )
		{
			images->Add(bitmaps[i]);
		}
		else
		{
			images->Add(wxBitmap(bitmaps[i].ConvertToImage().Rescale(size, size)));
		}
	}

	AssignImageList(images);

}


#define TREE_EVENT_HANDLER(name)                                 \
	void EffectObjectTree::name(wxTreeEvent& event)                        \
{                                                                \
	LogEvent(_T(#name), event);                                  \
	SetLastItem(wxTreeItemId());                                 \
	event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnDeleteItem)
TREE_EVENT_HANDLER(OnGetInfo)
TREE_EVENT_HANDLER(OnSetInfo)
TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)

#undef TREE_EVENT_HANDLER

void LogKeyEvent(const wxChar *name, const wxKeyEvent& event)
{
	wxString key;
	long keycode = event.GetKeyCode();
	{
		switch ( keycode )
		{
		case WXK_BACK: key = wxT("BACK"); break;
		case WXK_TAB: key = wxT("TAB"); break;
		case WXK_RETURN: key = wxT("RETURN"); break;
		case WXK_ESCAPE: key = wxT("ESCAPE"); break;
		case WXK_SPACE: key = wxT("SPACE"); break;
		case WXK_DELETE: key = wxT("DELETE"); break;
		case WXK_START: key = wxT("START"); break;
		case WXK_LBUTTON: key = wxT("LBUTTON"); break;
		case WXK_RBUTTON: key = wxT("RBUTTON"); break;
		case WXK_CANCEL: key = wxT("CANCEL"); break;
		case WXK_MBUTTON: key = wxT("MBUTTON"); break;
		case WXK_CLEAR: key = wxT("CLEAR"); break;
		case WXK_SHIFT: key = wxT("SHIFT"); break;
		case WXK_ALT: key = wxT("ALT"); break;
		case WXK_CONTROL: key = wxT("CONTROL"); break;
		case WXK_MENU: key = wxT("MENU"); break;
		case WXK_PAUSE: key = wxT("PAUSE"); break;
		case WXK_CAPITAL: key = wxT("CAPITAL"); break;
		case WXK_END: key = wxT("END"); break;
		case WXK_HOME: key = wxT("HOME"); break;
		case WXK_LEFT: key = wxT("LEFT"); break;
		case WXK_UP: key = wxT("UP"); break;
		case WXK_RIGHT: key = wxT("RIGHT"); break;
		case WXK_DOWN: key = wxT("DOWN"); break;
		case WXK_SELECT: key = wxT("SELECT"); break;
		case WXK_PRINT: key = wxT("PRINT"); break;
		case WXK_EXECUTE: key = wxT("EXECUTE"); break;
		case WXK_SNAPSHOT: key = wxT("SNAPSHOT"); break;
		case WXK_INSERT: key = wxT("INSERT"); break;
		case WXK_HELP: key = wxT("HELP"); break;
		case WXK_NUMPAD0: key = wxT("NUMPAD0"); break;
		case WXK_NUMPAD1: key = wxT("NUMPAD1"); break;
		case WXK_NUMPAD2: key = wxT("NUMPAD2"); break;
		case WXK_NUMPAD3: key = wxT("NUMPAD3"); break;
		case WXK_NUMPAD4: key = wxT("NUMPAD4"); break;
		case WXK_NUMPAD5: key = wxT("NUMPAD5"); break;
		case WXK_NUMPAD6: key = wxT("NUMPAD6"); break;
		case WXK_NUMPAD7: key = wxT("NUMPAD7"); break;
		case WXK_NUMPAD8: key = wxT("NUMPAD8"); break;
		case WXK_NUMPAD9: key = wxT("NUMPAD9"); break;
		case WXK_MULTIPLY: key = wxT("MULTIPLY"); break;
		case WXK_ADD: key = wxT("ADD"); break;
		case WXK_SEPARATOR: key = wxT("SEPARATOR"); break;
		case WXK_SUBTRACT: key = wxT("SUBTRACT"); break;
		case WXK_DECIMAL: key = wxT("DECIMAL"); break;
		case WXK_DIVIDE: key = wxT("DIVIDE"); break;
		case WXK_F1: key = wxT("F1"); break;
		case WXK_F2: key = wxT("F2"); break;
		case WXK_F3: key = wxT("F3"); break;
		case WXK_F4: key = wxT("F4"); break;
		case WXK_F5: key = wxT("F5"); break;
		case WXK_F6: key = wxT("F6"); break;
		case WXK_F7: key = wxT("F7"); break;
		case WXK_F8: key = wxT("F8"); break;
		case WXK_F9: key = wxT("F9"); break;
		case WXK_F10: key = wxT("F10"); break;
		case WXK_F11: key = wxT("F11"); break;
		case WXK_F12: key = wxT("F12"); break;
		case WXK_F13: key = wxT("F13"); break;
		case WXK_F14: key = wxT("F14"); break;
		case WXK_F15: key = wxT("F15"); break;
		case WXK_F16: key = wxT("F16"); break;
		case WXK_F17: key = wxT("F17"); break;
		case WXK_F18: key = wxT("F18"); break;
		case WXK_F19: key = wxT("F19"); break;
		case WXK_F20: key = wxT("F20"); break;
		case WXK_F21: key = wxT("F21"); break;
		case WXK_F22: key = wxT("F22"); break;
		case WXK_F23: key = wxT("F23"); break;
		case WXK_F24: key = wxT("F24"); break;
		case WXK_NUMLOCK: key = wxT("NUMLOCK"); break;
		case WXK_SCROLL: key = wxT("SCROLL"); break;
		case WXK_PAGEUP: key = wxT("PAGEUP"); break;
		case WXK_PAGEDOWN: key = wxT("PAGEDOWN"); break;
		case WXK_NUMPAD_SPACE: key = wxT("NUMPAD_SPACE"); break;
		case WXK_NUMPAD_TAB: key = wxT("NUMPAD_TAB"); break;
		case WXK_NUMPAD_ENTER: key = wxT("NUMPAD_ENTER"); break;
		case WXK_NUMPAD_F1: key = wxT("NUMPAD_F1"); break;
		case WXK_NUMPAD_F2: key = wxT("NUMPAD_F2"); break;
		case WXK_NUMPAD_F3: key = wxT("NUMPAD_F3"); break;
		case WXK_NUMPAD_F4: key = wxT("NUMPAD_F4"); break;
		case WXK_NUMPAD_HOME: key = wxT("NUMPAD_HOME"); break;
		case WXK_NUMPAD_LEFT: key = wxT("NUMPAD_LEFT"); break;
		case WXK_NUMPAD_UP: key = wxT("NUMPAD_UP"); break;
		case WXK_NUMPAD_RIGHT: key = wxT("NUMPAD_RIGHT"); break;
		case WXK_NUMPAD_DOWN: key = wxT("NUMPAD_DOWN"); break;
		case WXK_NUMPAD_PAGEUP: key = wxT("NUMPAD_PAGEUP"); break;
		case WXK_NUMPAD_PAGEDOWN: key = wxT("NUMPAD_PAGEDOWN"); break;
		case WXK_NUMPAD_END: key = wxT("NUMPAD_END"); break;
		case WXK_NUMPAD_BEGIN: key = wxT("NUMPAD_BEGIN"); break;
		case WXK_NUMPAD_INSERT: key = wxT("NUMPAD_INSERT"); break;
		case WXK_NUMPAD_DELETE: key = wxT("NUMPAD_DELETE"); break;
		case WXK_NUMPAD_EQUAL: key = wxT("NUMPAD_EQUAL"); break;
		case WXK_NUMPAD_MULTIPLY: key = wxT("NUMPAD_MULTIPLY"); break;
		case WXK_NUMPAD_ADD: key = wxT("NUMPAD_ADD"); break;
		case WXK_NUMPAD_SEPARATOR: key = wxT("NUMPAD_SEPARATOR"); break;
		case WXK_NUMPAD_SUBTRACT: key = wxT("NUMPAD_SUBTRACT"); break;
		case WXK_NUMPAD_DECIMAL: key = wxT("NUMPAD_DECIMAL"); break;

		default:
			{
				if ( keycode < 128 && wxIsprint((int)keycode) )
					key.Printf(wxT("'%c'"), (char)keycode);
				else if ( keycode > 0 && keycode < 27 )
					key.Printf(_("Ctrl-%c"), wxT('A') + keycode - 1);
				else
					key.Printf(wxT("unknown (%ld)"), keycode);
			}
		}
	}

	wxLogMessage( wxT("%s event: %s (flags = %c%c%c%c)"),
		name,
		key.c_str(),
		event.ControlDown() ? wxT('C') : wxT('-'),
		event.AltDown() ? wxT('A') : wxT('-'),
		event.ShiftDown() ? wxT('S') : wxT('-'),
		event.MetaDown() ? wxT('M') : wxT('-'));
}

void EffectObjectTree::OnTreeKeyDown(wxTreeEvent& event)
{
	event.Skip();
}



void EffectObjectTree::OnBeginLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnBeginLabelEdit"));

	// for testing, prevent this item's label editing
	wxTreeItemId itemId = event.GetItem();
	if ( IsTestItem(itemId) )
	{
		wxMessageBox(wxT("You can't edit this item."));

		event.Veto();
	}
	else if ( itemId == GetRootItem() )
	{
		// test that it is possible to change the text of the item being edited
		SetItemText(itemId, _T("Editing root item"));
	}
}

void EffectObjectTree::OnEndLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnEndLabelEdit"));

	// don't allow anything except letters in the labels
	if ( !event.GetLabel().IsWord() )
	{
		wxMessageBox(wxT("The new label should be a single word."));

		event.Veto();
	}
}

void EffectObjectTree::OnItemCollapsing(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnItemCollapsing"));

	// for testing, prevent the user from collapsing the first child folder
	wxTreeItemId itemId = event.GetItem();
}

void EffectObjectTree::OnItemActivated(wxTreeEvent& event)
{
	// show some info about this item
	wxTreeItemId itemId = event.GetItem();
	EffectTreeItemData *item = (EffectTreeItemData *)GetItemData(itemId);

	if ( item != NULL )
	{
		item->ShowInfo(this);
	}

	wxLogMessage(wxT("OnItemActivated"));
}

void EffectObjectTree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();	
	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);

	//if(item->GetDesc() == "Particle item")
	ShowMenu(itemId, clientpt);
	event.Skip();
}
void EffectObjectTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	EffectTreeItemData *item = id.IsOk() ? (EffectTreeItemData *)GetItemData(id): NULL;
	if (item == NULL)
	{
		return;
	}

	if (item->GetDesc() == "Particle item")
	{	
		wxMenu menu("");
		menu.Append(feID_COPY_PARTICLE, wxT("&C复制"));
		if(mCopyParticleSystem)
		{
			menu.AppendSeparator();
			menu.Append(feID_PASET_PARTICLE, wxT("&P粘贴"));
		}
		menu.AppendSeparator();
		menu.Append(feID_MENU_EFFECT_OBJ_DELETE_ELEMENT, wxT("删除"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_EMITTER, wxT("添加发射器"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_AFFECTOR, wxT("添加影响器"));
		
		//menu.Append(TreeTest_Dump, wxT("&Dump"));

		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "Effect element" 
		|| item->GetDesc() == "Particle emitter"
		|| item->GetDesc() == "Particle affector")
	{
		wxMenu menu("");	
		menu.Append(feID_COPY_EFFECT_ELEMENT, wxT("&C复制"));
		if(mCopyEffectElement && mSelectEffectObject)
		{
			Fairy::EffectElement* pEffectElement = dynamic_cast<Fairy::EffectElement*>(mSelectEffectObject);
			if(pEffectElement)
			{
				if(mCopyEffectElementType == pEffectElement->getType())
				{
					menu.AppendSeparator();
					menu.Append(feID_PASET_EFFECT_ELEMENT, wxT("&P粘贴"));
				}
			}
		}
		menu.AppendSeparator();
		menu.Append(feID_MENU_EFFECT_OBJ_DELETE_ELEMENT, wxT("删除"));
		//menu.Append(TreeTest_Dump, wxT("&Dump"));
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "Effect root item")
	{
		wxMenu menu("");		
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_PARTICLE, wxT("添加粒子效果"));	
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_MESH, wxT("添加模型效果"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_BILLBOARDSET, wxT("添加广告板效果"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_PROJECTOR, wxT("添加投射效果"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_BEAM, wxT("添加光柱效果"));	
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_POINTLIGHT, wxT("添加点光源效果"));
		menu.Append(feID_MENU_EFFECT_OBJ_NEW_PARTICLESYSTEM, wxT("添加粒子数据"));

		//menu.Append(TreeTest_Dump, wxT("&Dump"));

		PopupMenu(&menu, pt);
	}
}


void EffectObjectTree::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint pt = event.GetPosition();

	wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);
}


void EffectObjectTree::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	EffectTreeItemData *item = itemId.IsOk() ? (EffectTreeItemData *)GetItemData(itemId)
		: NULL;

	wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc()
		: _T(""));

	event.Skip();
}

void EffectObjectTree::OnRMouseDown(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button down"));

	event.Skip();
}

void EffectObjectTree::OnRMouseUp(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button up"));

	event.Skip();
}

void EffectObjectTree::OnRMouseDClick(wxMouseEvent& event)
{
	wxTreeItemId id = HitTest(event.GetPosition());
	if ( !id )
		wxLogMessage(wxT("No item under mouse"));
	else
	{
		EffectTreeItemData *item = (EffectTreeItemData *)GetItemData(id);
		if ( item )
			wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
	}

	event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
	return b ? wxT("") : wxT("not ");
}
void EffectTreeItemData::ShowInfo(wxTreeCtrl *tree)
{
	wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
		wxT("%u children (%u immediately under this item)."),
		m_desc.c_str(),
		Bool2String(tree->IsSelected(GetId())),
		Bool2String(tree->IsExpanded(GetId())),
		Bool2String(tree->IsBold(GetId())),
		unsigned(tree->GetChildrenCount(GetId())),
		unsigned(tree->GetChildrenCount(GetId(), false)));
}

void EffectObjectTree::OnSelChanging(wxTreeEvent& event)
{

}

void EffectObjectTree::OnSelChanged(wxTreeEvent& event)
{

	wxTreeItemId id = event.GetItem();
	if ( !id )
	{
		wxLogMessage(wxT("No item under mouse"));
	}
	EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(id)); 
	if(pItemData)
	{

		EffectObjectEditor* pEditor = dynamic_cast<EffectObjectEditor*>(m_parent);
		pEditor->UpdateLocatorNode(false);
		EffectObjectPropertyEditor * pPropertyEditor = pEditor->GetParentFrame()->GetEffectObjectProperty();

		if(pItemData->GetDesc() == "Effect property item" || 
			pItemData->GetDesc() == "Effect root item")
		{
			pPropertyEditor->InitEffectSettingProperty();
			mSelectEffectObject = NULL;
		}
		else if(pItemData->GetDesc() == "Effect element" ||
			pItemData->GetDesc() == "Effect element item")
		{
			Fairy::EffectElement* pElement = (Fairy::EffectElement*)pItemData->GetData();
			pPropertyEditor->InitEffectElementProperty(pElement);
			mSelectEffectObject = pElement;

			pEditor->UpdateLocatorNode(true);
			pEditor->GetSceneManipulator()->getLocatorNode()->setPosition(pElement->getRelPos());
		}
		else if(pItemData->GetDesc() == "Particle item" ||
			pItemData->GetDesc() == "Particle BasicSetting" ||
			pItemData->GetDesc() == "Particle BasicSetting item" ||
			pItemData->GetDesc() == "Particle BasicSetting item renderer" ||
			pItemData->GetDesc() == "Particle BasicSetting item material")
		{
			Ogre::ParticleSystem* pParticleSystem = (Ogre::ParticleSystem*)pItemData->GetData();
			pPropertyEditor->InitParticleSettingProperty(pParticleSystem);
			mSelectEffectObject = pParticleSystem;
		}
		else if(pItemData->GetDesc() == "Particle render" ||
			pItemData->GetDesc() == "Particle render item")
		{
			Ogre::ParticleSystemRenderer *renderer  = (Ogre::ParticleSystemRenderer*)pItemData->GetData();
			pPropertyEditor->InitParticleRenderProperty(renderer);
			mSelectEffectObject = NULL;
		}
		else if(pItemData->GetDesc() == "Particle emitter" || pItemData->GetDesc() == "Particle emitter item")
		{
			Ogre::ParticleEmitter* pEmitter = (Ogre::ParticleEmitter*)pItemData->GetData();
			pPropertyEditor->InitParticleEmitterProperty(pEmitter);
			mSelectEffectObject = pEmitter;
		}
		else if(pItemData->GetDesc() == "Particle affector" || pItemData->GetDesc() == "Particle affector item")
		{
			Ogre::ParticleAffector* pAffector = (Ogre::ParticleAffector*)pItemData->GetData();
			pPropertyEditor->InitParticleAffectorProperty(pAffector);
			mSelectEffectObject = pAffector;
		}
		
	}

	event.Skip();
}
void EffectObjectTree::refrashSelected()
{
	wxTreeItemId selectedId = this->GetSelection();
	if(selectedId)
	{
		this->SelectItem(selectedId);
	}
}
void EffectObjectTree::refreshItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);

	while(itemId.IsOk())
	{
		EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
		{
			Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
			Ogre::String value = pObject->getParameter(propertyName.c_str());
			this->SetItemText(itemId,(propertyName + ":" + value).c_str());
			return;
		}

		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{
			EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
			{
				Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
				Ogre::String value = pObject->getParameter(propertyName.c_str());
				this->SetItemText(childId,(propertyName + ":" + value).c_str());
				return;
			}

			subId = GetFirstChild(childId,idValue);
			while(subId.IsOk())
			{
				EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(subId)); 
				if(pItemData->GetDesc() == Desc)
				{
					if(pItemData->GetData() == data)
					{
						if(pItemData->GetValue() == propertyName)
						{
							Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
							Ogre::String value = pObject->getParameter(propertyName.c_str());
							this->SetItemText(subId,(propertyName + ":" + value).c_str());
							return;
						}
					}
				}
				subId = GetNextSibling(subId);
			}

			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}

}
void EffectObjectTree::SelectParticleSystem(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{

		EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(itemId)); 
		if(pItemData->GetDesc() == "Particle item" && pItemData->GetData() == pParticleSystem)
		{

			if(select)
				SelectItem(itemId);
			Expand(itemId);
			break;
		}
	
		itemId = GetNextSibling(itemId);
	}

}

//选择粒子基本参数Item
void EffectObjectTree::SelectParticleSetting(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{
			EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == "Particle BasicSetting" && pItemData->GetData() == pParticleSystem)
			{
				if(select)
					SelectItem(childId);
				Expand(childId);
				break;
			}
			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}

}
//选择粒子渲染器item
void EffectObjectTree::SelectParticleRenderer(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	if(!pParticleSystem)
		return;

	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{
			EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == "Particle render" && pItemData->GetData() == pParticleSystem)
			{
				if(select)
					SelectItem(childId);
				Expand(childId);
				break;
			}
			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}


}
//选择粒子发射器item
void EffectObjectTree::SelectParticleEmitter(Ogre::ParticleEmitter* emitter,bool select)
{
	if(!emitter)
		return;

	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{
			EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == "Particle emitter")
			{
				if(emitter == (Ogre::ParticleEmitter*)pItemData->GetData())
				{
					if(select)
						SelectItem(childId);
					Expand(childId);
					break;
				}
			}
			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}

}
//选择粒子影响器item
void EffectObjectTree::SelectParticleAffector(Ogre::ParticleAffector* affector,bool select)
{
	if(!affector)
		return;

	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId;
	wxTreeItemId itemId = GetFirstChild(rootId,idValue);
	while(itemId.IsOk())
	{
		childId = GetFirstChild(itemId,idValue);
		while(childId.IsOk())
		{
			EffectTreeItemData* pItemData = dynamic_cast<EffectTreeItemData*>(GetItemData(childId)); 
			if(pItemData->GetDesc() == "Particle affector")
			{
				if(affector == (Ogre::ParticleAffector*)pItemData->GetData())
				{
					if(select)
						SelectItem(childId);
					Expand(childId);
					break;
				}
			}
			childId = GetNextSibling(childId);
		}
		itemId = GetNextSibling(itemId);
	}

}