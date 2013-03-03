

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
#include "BulletTree.h"
#include "BulletEditor.h"
#include "EffectObjectPropertyEditor.h"
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreStringConverter.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairyMeshElement.h"
#include "EffectSystem/FairyBillboardSetElement.h"
#include "EffectSystem/FairyBeamElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyBulletSystemManager.h"
#include "EffectSystem/FairyBulletSystem.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationCameraShake.h"
#include "EffectSystem/FairyAnimationLine.h"

static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TV_HITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return (HTREEITEM)TreeView_HitTest(hwndTV, &tvht);
}

// Menu for toolbar of bullet editor
const static int feID_MENU_BULLET_DELEMENT_ELEMENT		= wxNewId();
const static int feID_MENU_BULLET_NEW_EVENT				= wxNewId();
const static int feID_MENU_BULLET_NEW_OPERATOR			= wxNewId();
const static int feID_MENU_BULLET_NEW_BULLETSYSTEM		= wxNewId();
const static int feID_MENU_BULLET_NEW_EFFECTINFO		= wxNewId();
const static int feID_MENU_BULLET_NEW_CAMERASHAKE		= wxNewId();
const static int feID_MENU_BULLET_NEW_RIBBON			= wxNewId();
const static int feID_MENU_BULLET_NEW_SOUND				= wxNewId();
const static int feID_MENU_BULLET_NEW_LIGHT				= wxNewId();
const static int feID_MENU_BULLET_NEW_LINE				= wxNewId();

IMPLEMENT_DYNAMIC_CLASS(BulletTree, wxTreeCtrl)

BEGIN_EVENT_TABLE(BulletTree, wxTreeCtrl)
EVT_TREE_BEGIN_DRAG(ID_BULLET_OBJECT_TREE, BulletTree::OnBeginDrag)
EVT_TREE_BEGIN_RDRAG(ID_BULLET_OBJECT_TREE, BulletTree::OnBeginRDrag)
EVT_TREE_END_DRAG(ID_BULLET_OBJECT_TREE, BulletTree::OnEndDrag)
EVT_TREE_BEGIN_LABEL_EDIT(ID_BULLET_OBJECT_TREE, BulletTree::OnBeginLabelEdit)
EVT_TREE_END_LABEL_EDIT(ID_BULLET_OBJECT_TREE, BulletTree::OnEndLabelEdit)
EVT_TREE_DELETE_ITEM(ID_BULLET_OBJECT_TREE, BulletTree::OnDeleteItem)
EVT_TREE_SET_INFO(ID_BULLET_OBJECT_TREE, BulletTree::OnSetInfo)
EVT_TREE_ITEM_EXPANDED(ID_BULLET_OBJECT_TREE, BulletTree::OnItemExpanded)
EVT_TREE_ITEM_EXPANDING(ID_BULLET_OBJECT_TREE, BulletTree::OnItemExpanding)
EVT_TREE_ITEM_COLLAPSED(ID_BULLET_OBJECT_TREE, BulletTree::OnItemCollapsed)
EVT_TREE_ITEM_COLLAPSING(ID_BULLET_OBJECT_TREE, BulletTree::OnItemCollapsing)

EVT_TREE_SEL_CHANGED(ID_BULLET_OBJECT_TREE, BulletTree::OnSelChanged)
EVT_TREE_SEL_CHANGING(ID_BULLET_OBJECT_TREE, BulletTree::OnSelChanging)
EVT_TREE_KEY_DOWN(ID_BULLET_OBJECT_TREE, BulletTree::OnTreeKeyDown)
EVT_TREE_ITEM_ACTIVATED(ID_BULLET_OBJECT_TREE, BulletTree::OnItemActivated)

// so many differents ways to handle right mouse button clicks...
EVT_CONTEXT_MENU(BulletTree::OnContextMenu)
// EVT_TREE_ITEM_MENU is the preferred event for creating context menus
// on a tree control, because it includes the point of the click or item,
// meaning that no additional placement calculations are required.
EVT_TREE_ITEM_MENU(ID_BULLET_OBJECT_TREE, BulletTree::OnItemMenu)
EVT_TREE_ITEM_RIGHT_CLICK(ID_BULLET_OBJECT_TREE, BulletTree::OnItemRClick)

EVT_MENU( feID_MENU_BULLET_DELEMENT_ELEMENT, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_DELEMENT_ELEMENT, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_EVENT, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_OPERATOR	, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_BULLETSYSTEM, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_EFFECTINFO, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_CAMERASHAKE, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_RIBBON, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_SOUND, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_LIGHT, BulletTree::OnMenu)
EVT_MENU( feID_MENU_BULLET_NEW_LINE, BulletTree::OnMenu)

EVT_RIGHT_DOWN(BulletTree::OnRMouseDown)
EVT_RIGHT_UP(BulletTree::OnRMouseUp)
EVT_RIGHT_DCLICK(BulletTree::OnRMouseDClick)
END_EVENT_TABLE()

int BulletTree::OnCompareItems(const wxTreeItemId& item1,
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

void BulletTree::GetItemsRecursively(const wxTreeItemId& idParent,
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


void BulletTree::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	wxString text;
	if ( item.IsOk() )
		text << _T('"') << GetItemText(item).c_str() << _T('"');
	else
		text = _T("invalid item");
	wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

long BulletTree::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
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
void BulletTree::AddBulletSystemToTree(Fairy::BulletSystem *pBulletSystem,const Ogre::String &templateName)
{
	int iBulletSystemImage = BulletTree::TreeCtrlIcon_BulletSystem;

	wxTreeItemId rootId = this->GetRootItem();
	if(!rootId.IsOk())
		return;
	wxTreeItemId BulletSystemRootId = AppendItem(rootId,templateName,
		iBulletSystemImage, iBulletSystemImage,
		new BulletTreeItemData(wxT("BulletSystem item"),templateName,pBulletSystem));

	int iAnimEfectImage = BulletTree::TreeCtrlIcon_AnimEffect;
	int iRibbinImage = BulletTree::TreeCtrlIcon_Ribbon;
	int iCameraShakeImage = BulletTree::TreeCtrlIcon_CameraShake;
	int iSoundImage = BulletTree::TreeCtrlIcon_Sound;
	int iSceneLightImage = BulletTree::TreeCtrlIcon_SceneLight;
	int iLineImage = BulletTree::TreeCtrlIcon_Line;
	int iPorpertyImage = BulletTree::TreeCtrlIcon_BulletProperty;


	// 设置effect属性
	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationEffectInfos(); ++i )
	{
		Fairy::AnimationEffectInfo *effectInfo = pBulletSystem->getAnimationEffectInfo(i);
		assert (effectInfo);

		Ogre::String effectIndexStr = "Effect Info " + Ogre::StringConverter::toString(i);
		wxTreeItemId AnimEffectId= AppendItem(BulletSystemRootId,effectIndexStr.c_str(),
			iAnimEfectImage, iAnimEfectImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("AnimEffect"),effectInfo));

		if ( AnimEffectId.IsOk() )
		{

			const Ogre::ParameterList &paraList = effectInfo->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = effectInfo->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( AnimEffectId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,effectInfo));

			}
		}
	}

	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationRibbons(); ++i )
	{
		Fairy::AnimationRibbon *ribbon = pBulletSystem->getAnimationRibbon(i);

		assert (ribbon);

		Ogre::String ribbonIndexStr = "Ribbon " + Ogre::StringConverter::toString(i);

		wxTreeItemId RibbonId= AppendItem(BulletSystemRootId,ribbonIndexStr.c_str(),
			iRibbinImage, iRibbinImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("Ribbon"),ribbon));

		if ( RibbonId.IsOk() )
		{

			const Ogre::ParameterList &paraList = ribbon->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = ribbon->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( RibbonId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,ribbon));

			}
		}		
	}
	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationCameraShake(); ++i )
	{
		Fairy::AnimationCameraShake *cameraShake = pBulletSystem->getAnimationCameraShake(i);

		assert (cameraShake);

		Ogre::String soundIndexStr = "CameraShake " + Ogre::StringConverter::toString(i);

		wxTreeItemId CamaerShakeId= AppendItem(BulletSystemRootId,soundIndexStr.c_str(),
			iCameraShakeImage, iCameraShakeImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("CameraShake"),cameraShake));

		if ( CamaerShakeId.IsOk() )
		{

			const Ogre::ParameterList &paraList = cameraShake->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = cameraShake->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( CamaerShakeId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,cameraShake));

			}
		}	
	}

	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationSounds(); ++i )
	{
		Fairy::AnimationSound *sound = pBulletSystem->getAnimationSound(i);

		assert (sound);

		Ogre::String soundIndexStr = "Sound " + Ogre::StringConverter::toString(i);

		wxTreeItemId SoundId= AppendItem(BulletSystemRootId,soundIndexStr.c_str(),
			iSoundImage, iSoundImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("Sound"),sound));

		if ( SoundId.IsOk() )
		{

			const Ogre::ParameterList &paraList = sound->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = sound->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( SoundId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,sound));

			}
		}	
	}

	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationSceneLightInfos(); ++i )
	{
		Fairy::AnimationSceneLightInfo *light = pBulletSystem->getAnimationSceneLightInfo(i);

		assert (light);

		Ogre::String lightIndexStr = "SceneLight " + Ogre::StringConverter::toString(i);

		wxTreeItemId LightId= AppendItem(BulletSystemRootId,lightIndexStr.c_str(),
			iSceneLightImage, iSceneLightImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("SceneLight"),light));

		if ( LightId.IsOk() )
		{

			const Ogre::ParameterList &paraList = light->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = light->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( LightId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,light));

			}
		}		
	}

	for ( unsigned short i=0; i < pBulletSystem->getNumAnimationLines(); ++i )
	{
		Fairy::AnimationLine *line = pBulletSystem->getAnimationLine(i);

		assert (line);

		Ogre::String lineIndexStr = "Line " + Ogre::StringConverter::toString(i);

		wxTreeItemId LineId= AppendItem(BulletSystemRootId,lineIndexStr.c_str(),
			iLineImage, iLineImage,
			new BulletTreeItemData(wxT("BulletSystem Element"),wxT("Line"),line));

		if ( LineId.IsOk() )
		{

			const Ogre::ParameterList &paraList = line->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = line->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( LineId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new BulletTreeItemData(wxT("BulletSystem Element item"),name,line));

			}
		}		
	}

	Expand(BulletSystemRootId);
}
void BulletTree::AddBulletToTree(Fairy::BulletFlowSystem *pBulletFlow,const Ogre::String &templateName)
{
	int iBulletFlowImage = BulletTree::TreeCtrlIcon_BulletFlow;
	wxTreeItemId BulletFlowRootId = AddRoot(templateName,
		iBulletFlowImage, iBulletFlowImage,
		new BulletTreeItemData(wxT("BulletFlow root item"),templateName,pBulletFlow));

	int iPorpertyImage = BulletTree::TreeCtrlIcon_BulletProperty;
	int iEventImage = BulletTree::TreeCtrlIcon_BulletEvent;
	int iOperator = BulletTree::TreeCtrlIcon_Operator;
	int iTester = BulletTree::TreeCtrlIcon_Tester;



	if ( !BulletFlowRootId.IsOk() )
		return;
	const Ogre::ParameterList &paraList = pBulletFlow->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		Ogre::String name = paraList[paraCount].name;
		Ogre::String value = pBulletFlow->getParameter(paraList[paraCount].name);

		wxTreeItemId itemId =AppendItem( BulletFlowRootId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
			new BulletTreeItemData(wxT("BulletFlow property item"),name,pBulletFlow));
	}
	Fairy::BulletFlowSystem::BulletEventMap::const_iterator it = pBulletFlow->getBulletEventSystemsMap().begin();
	while(it != pBulletFlow->getBulletEventSystemsMap().end())
	{
		Fairy::BulletEventSystem* pEvent = it->second;

		wxTreeItemId EventId;
		Ogre::String EventName = it->first;
		EventId= AppendItem(BulletFlowRootId,EventName.c_str(),
		iEventImage, iEventImage,
		new BulletTreeItemData(wxT("BulletEventSysetem item"),EventName,pEvent));

		const Ogre::ParameterList &paraList = pEvent->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String name = paraList[paraCount].name;
			Ogre::String value = pEvent->getParameter(paraList[paraCount].name);

			wxTreeItemId itemId =AppendItem( EventId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
				new BulletTreeItemData(wxT("BulletEventSysetem property item"),name,pEvent));
		}

		for(int index = 0; index < pEvent->getNumBulletOperator(); index++)
		{
			Fairy::BulletOperator* pOperator= pEvent->getBulletOperator(index);
			Fairy::BulletTester* pTester = dynamic_cast<Fairy::BulletTester*>(pOperator);
			if(pTester)
			{
				wxTreeItemId OperatorId= AppendItem(EventId,pTester->getType(),iTester,iTester,
					new BulletTreeItemData(wxT("BulletOperator item"),pTester->getType(),pTester));

				const Ogre::ParameterList &paraList = pOperator->getParameters();

				for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
				{
					Ogre::String name = paraList[paraCount].name;
					Ogre::String value = pOperator->getParameter(paraList[paraCount].name);

					wxTreeItemId itemId =AppendItem( OperatorId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
						new BulletTreeItemData(wxT("BulletOperator property item") ,name,pTester));

				}

			}
			else
			{

				wxTreeItemId OperatorId= AppendItem(EventId,pOperator->getType(),
					iOperator, iOperator,
					new BulletTreeItemData(wxT("BulletOperator item"),pOperator->getType(),pOperator));

				const Ogre::ParameterList &paraList = pOperator->getParameters();

				for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
				{
					Ogre::String name = paraList[paraCount].name;
					Ogre::String value = pOperator->getParameter(paraList[paraCount].name);

					wxTreeItemId itemId =AppendItem( OperatorId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
						new BulletTreeItemData(wxT("BulletOperator property item"),name,pOperator));

				}
			}
		
		}
		it++;

		Expand(EventId);
	}//while(it != pBulletFlow->getBulletEventSystemsMap().end())


	Expand(BulletFlowRootId);

}

void BulletTree::SelectElementItem(Fairy::EffectElement* element)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(childId)); 
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
void BulletTree::SelectBulletEvent(Fairy::BulletEventSystem* pEvent)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(childId)); 
		
		if(pItemData->GetDesc() == "BulletEventSysetem item")
		{
			Fairy::BulletEventSystem* pBulletEvent = (Fairy::BulletEventSystem*)pItemData->GetData();
			if(pBulletEvent == pEvent)
			{
				this->SelectItem(childId);
				this->Expand(childId);
				return;

			}
		}
		childId = GetNextSibling(childId);
	}

}
void BulletTree::SelectBulletOperator(Fairy::BulletOperator* pOperator)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		subId = GetFirstChild(childId,idValue);
		while(subId.IsOk())
		{
			BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(subId)); 
			
			if(pItemData->GetDesc() == "BulletOperator item")
			{
				Fairy::BulletOperator* pBulletOperator = (Fairy::BulletOperator*)pItemData->GetData();
				if(pBulletOperator == pOperator)
				{
					this->SelectItem(subId);
					this->Expand(subId);
					return;

				}
			}

			subId = GetNextSibling(subId);
		}
		childId = GetNextSibling(childId);
	}

}
void BulletTree::SelectBulletSystem(Fairy::BulletSystem* pSystem)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(childId)); 

		if(pItemData->GetDesc() == "BulletSystem item")
		{
			Fairy::BulletSystem* pBulletSystem = (Fairy::BulletSystem*)pItemData->GetData();
			if(pBulletSystem == pSystem)
			{
				this->SelectItem(childId);
				this->Expand(childId);
				return;

			}
		}
		childId = GetNextSibling(childId);
	}
}
void BulletTree::SelectBulletSystemElement(Ogre::StringInterface* pElement)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		subId = GetFirstChild(childId,idValue);
		while(subId.IsOk())
		{
			BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(subId)); 

			if(pItemData->GetDesc() == "BulletSystem Element")
			{
				Ogre::StringInterface* pBulletSystemElement = (Ogre::StringInterface*)pItemData->GetData();
				if(pBulletSystemElement == pElement)
				{
					this->SelectItem(subId);
					this->Expand(subId);
					return;

				}
			}

			subId = GetNextSibling(subId);
		}
		childId = GetNextSibling(childId);
	}

}
void BulletTree::CreateImageList(int size)
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
	wxBitmap bitmaps[12];
	bitmaps[0] = wxBITMAP(ANIMBULLETFLOW);
	bitmaps[1] = wxBITMAP(BULLETSYSTEM);
	bitmaps[2] = wxBITMAP(EVENT);
	bitmaps[3] = wxBITMAP(ANIMEFFECT);
	bitmaps[4] = wxBITMAP(CAMERASHAKE);
	bitmaps[5] = wxBITMAP(RIBBON);
	bitmaps[6] = wxBITMAP(SOUND);
	bitmaps[7] = wxBITMAP(SCENELIGHT);
	bitmaps[8] = wxBITMAP(LINE);
	bitmaps[9] = wxBITMAP(EFFECTPROPERTY);
	bitmaps[10] = wxBITMAP(OPERATOR);
	bitmaps[11] = wxBITMAP(TESTER);


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
	void BulletTree::name(wxTreeEvent& event)                        \
{                                                                \
	LogEvent(_T(#name), event);                                  \
	SetLastItem(wxTreeItemId());                                 \
	event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnBeginRDrag)
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

void BulletTree::OnTreeKeyDown(wxTreeEvent& event)
{
	event.Skip();
}

void BulletTree::OnBeginDrag(wxTreeEvent& event)
{
	// need to explicitly allow drag
	if ( event.GetItem() != GetRootItem() )
	{
		m_draggedItem = event.GetItem();

		wxPoint clientpt = event.GetPoint();
		wxPoint screenpt = ClientToScreen(clientpt);

		wxLogMessage(wxT("OnBeginDrag: started dragging %s at screen coords (%i,%i)"),
			GetItemText(m_draggedItem).c_str(),
			screenpt.x, screenpt.y);

		event.Allow();
	}
	else
	{
		wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
	}
}

void BulletTree::OnEndDrag(wxTreeEvent& event)
{
	wxTreeItemId itemSrc = m_draggedItem,
		itemDst = event.GetItem();
	m_draggedItem = (wxTreeItemId)0l;

	// where to copy the item?
	if ( itemDst.IsOk() && !ItemHasChildren(itemDst) )
	{
		// copy to the parent then
		itemDst = GetItemParent(itemDst);
	}

	if ( !itemDst.IsOk() )
	{
		wxLogMessage(wxT("OnEndDrag: can't drop here."));

		return;
	}

	wxString text = GetItemText(itemSrc);
	wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
		text.c_str(), GetItemText(itemDst).c_str());

	// just do append here - we could also insert it just before/after the item
	// on which it was dropped, but this requires slightly more work... we also
	// completely ignore the client data and icon of the old item but could
	// copy them as well.
	//
	// Finally, we only copy one item here but we might copy the entire tree if
	// we were dragging a folder.
	//int image = wxGetApp().ShowImages() ? TreeCtrlIcon_File : -1;
	//AppendItem(itemDst, text, image);
}

void BulletTree::OnBeginLabelEdit(wxTreeEvent& event)
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

void BulletTree::OnEndLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnEndLabelEdit"));

	// don't allow anything except letters in the labels
	if ( !event.GetLabel().IsWord() )
	{
		wxMessageBox(wxT("The new label should be a single word."));

		event.Veto();
	}
}

void BulletTree::OnItemCollapsing(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnItemCollapsing"));

	// for testing, prevent the user from collapsing the first child folder
	wxTreeItemId itemId = event.GetItem();
}

void BulletTree::OnItemActivated(wxTreeEvent& event)
{
	// show some info about this item
	wxTreeItemId itemId = event.GetItem();
	BulletTreeItemData *item = (BulletTreeItemData *)GetItemData(itemId);

	if ( item != NULL )
	{
		item->ShowInfo(this);
	}

	wxLogMessage(wxT("OnItemActivated"));
}

void BulletTree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	
	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);

	ShowMenu(itemId, clientpt);
	event.Skip();
}
void BulletTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	BulletTreeItemData *item = id.IsOk() ? (BulletTreeItemData *)GetItemData(id): NULL;

	if (item == NULL)
	{
		return;
	}

	if (item->GetDesc() == "BulletFlow root item")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_BULLET_NEW_EVENT, wxT("添加事件"));		
		menu.Append(feID_MENU_BULLET_NEW_BULLETSYSTEM, wxT("添加子弹系统"));	
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "BulletEventSysetem item")
	{
		wxMenu menu("");				
		menu.Append(feID_MENU_BULLET_NEW_OPERATOR, wxT("添加操作器"));		
		menu.Append(feID_MENU_BULLET_DELEMENT_ELEMENT, wxT("删除"));
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "BulletOperator item")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_BULLET_DELEMENT_ELEMENT, wxT("删除"));
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "BulletSystem item")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_BULLET_NEW_EFFECTINFO, wxT("添加效果"));		
		menu.Append(feID_MENU_BULLET_NEW_CAMERASHAKE, wxT("添加相机震动"));		
		menu.Append(feID_MENU_BULLET_NEW_RIBBON, wxT("添加刀光"));		
		menu.Append(feID_MENU_BULLET_NEW_SOUND, wxT("添加声音"));		
		menu.Append(feID_MENU_BULLET_NEW_LIGHT, wxT("添加灯光"));
		menu.Append(feID_MENU_BULLET_NEW_LINE, wxT("添加连线"));
		menu.Append(feID_MENU_BULLET_DELEMENT_ELEMENT, wxT("删除"));
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "BulletSystem Element")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_BULLET_DELEMENT_ELEMENT, wxT("删除"));		
		PopupMenu(&menu, pt);
	}
}

void BulletTree::OnMenu(wxCommandEvent & event)
{
	int id = event.GetId();
	BulletEditor * parent = static_cast <BulletEditor*>(GetParent());
	if (parent == NULL)
	{
		return;
	}

	if (id == feID_MENU_BULLET_DELEMENT_ELEMENT)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_DELETE_ElEMENT);
	}
	else if (id == feID_MENU_BULLET_NEW_EVENT)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_EVENT);
	}
	else if (id == feID_MENU_BULLET_NEW_OPERATOR)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_OPERATOR);
	}
	else if (id == feID_MENU_BULLET_NEW_BULLETSYSTEM)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM);
	}
	else if (id == feID_MENU_BULLET_NEW_EFFECTINFO)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_EFFECTINFO);
	}
	else if (id == feID_MENU_BULLET_NEW_CAMERASHAKE)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_CAMERASHAKE);
	}
	else if (id == feID_MENU_BULLET_NEW_RIBBON)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_RIBBON);
	}
	else if (id == feID_MENU_BULLET_NEW_SOUND)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_SOUND);
	}
	else if (id == feID_MENU_BULLET_NEW_LIGHT)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_LIGHT);
	}
	else if (id == feID_MENU_BULLET_NEW_LINE)
	{
		parent->OnNotifyToolBarCtrl(BulletEditor::TC_NEW_BULLETSYSTEM_LINE);
	}
}

void BulletTree::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint pt = event.GetPosition();

	wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);
}


void BulletTree::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	BulletTreeItemData *item = itemId.IsOk() ? (BulletTreeItemData *)GetItemData(itemId)
		: NULL;

	wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc()
		: _T(""));

	event.Skip();
}

void BulletTree::OnRMouseDown(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button down"));

	event.Skip();
}

void BulletTree::OnRMouseUp(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button up"));

	event.Skip();
}

void BulletTree::OnRMouseDClick(wxMouseEvent& event)
{
	wxTreeItemId id = HitTest(event.GetPosition());
	if ( !id )
		wxLogMessage(wxT("No item under mouse"));
	else
	{
		BulletTreeItemData *item = (BulletTreeItemData *)GetItemData(id);
		if ( item )
			wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
	}

	event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
	return b ? wxT("") : wxT("not ");
}
void BulletTreeItemData::ShowInfo(wxTreeCtrl *tree)
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

void BulletTree::OnSelChanging(wxTreeEvent& event)
{

}

void BulletTree::OnSelChanged(wxTreeEvent& event)
{

	wxTreeItemId id = event.GetItem();
	if ( !id )
	{
		wxLogMessage(wxT("No item under mouse"));
	}

	BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(id)); 
	if(pItemData)
	{

		BulletEditor* pEditor = dynamic_cast<BulletEditor*>(m_parent);
		EffectObjectPropertyEditor * pPropertyEditor = pEditor->GetParentFrame()->GetEffectObjectProperty();

		if(pItemData->GetDesc() == "BulletFlow root item" || 
			pItemData->GetDesc() == "BulletFlow property item")
		{
			pPropertyEditor->InitBulletFlowProperty();
			mSelectBulletObject = NULL;
		}
		else if(pItemData->GetDesc() == "BulletOperator item" ||
			pItemData->GetDesc() == "BulletOperator property item")
		{
			Fairy::BulletOperator* pOperator = (Fairy::BulletOperator*)pItemData->GetData();
			pPropertyEditor->InitBulletOperator(pOperator);
			mSelectBulletObject = pOperator;
		}
		else if(pItemData->GetDesc() == "BulletEventSysetem item" )
		{
			Fairy::BulletEventSystem* pEvent = (Fairy::BulletEventSystem*)pItemData->GetData();
			mSelectBulletObject = pEvent;
		}
		else if(pItemData->GetDesc() == "BulletSystem item" )
		{
			Fairy::BulletSystem* pSystem = (Fairy::BulletSystem*)pItemData->GetData();
			mSelectBulletObject = pSystem;
		}
		else if(pItemData->GetDesc() == "BulletSystem Element" ||
			pItemData->GetDesc() == "BulletSystem Element item")
		{
			Ogre::StringInterface* pElement = (Ogre::StringInterface*)pItemData->GetData();
			pPropertyEditor->InitBulletSystemElement(pElement);
			mSelectBulletObject = pElement;
		}

	}

	event.Skip();
}
void BulletTree::refrashSelected()
{
	wxTreeItemId selectedId = this->GetSelection();
	if(selectedId)
	{
		this->SelectItem(selectedId);
	}
}
void BulletTree::refreshItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId itemId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(childId)); 
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
			BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(subId)); 
			if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
			{
				Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
				Ogre::String value = pObject->getParameter(propertyName.c_str());
				this->SetItemText(subId,(propertyName + ":" + value).c_str());
				return;
			}

			itemId = GetFirstChild(subId,idValue);
			while(itemId.IsOk())
			{
				BulletTreeItemData* pItemData = dynamic_cast<BulletTreeItemData*>(GetItemData(itemId)); 
				if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
				{
					Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
					Ogre::String value = pObject->getParameter(propertyName.c_str());
					this->SetItemText(itemId,(propertyName + ":" + value).c_str());
					return;
				}
				itemId = GetNextSibling(itemId);
			}
			subId = GetNextSibling(subId);
		}
		childId = GetNextSibling(childId);
	}

}