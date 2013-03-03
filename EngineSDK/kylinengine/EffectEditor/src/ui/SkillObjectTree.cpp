

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
#include "SkillObjectTree.h"
#include "SkillObjectEditor.h"
#include "EffectObjectPropertyEditor.h"
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreStringConverter.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TV_HITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return (HTREEITEM)TreeView_HitTest(hwndTV, &tvht);
}

// Menu for toolbar of skill editor
const static int feID_MENU_SKILL_DELEMENT_ELEMENT	= wxNewId();
const static int feID_MENU_SKILL_NEW_EFFECT			= wxNewId();
const static int feID_MENU_SKILL_NEW_CAMERASHAKE	= wxNewId();
const static int feID_MENU_SKILL_NEW_RIBBON			= wxNewId();
const static int feID_MENU_SKILL_NEW_SOUND			= wxNewId();
const static int feID_MENU_SKILL_NEW_SCENELIGHT		= wxNewId();
const static int FEID_MENU_SKILL_NEW_BULLETFLOW		= wxNewId();

IMPLEMENT_DYNAMIC_CLASS(SkillObjectTree, wxTreeCtrl)

BEGIN_EVENT_TABLE(SkillObjectTree, wxTreeCtrl)
EVT_TREE_BEGIN_DRAG(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnBeginDrag)
EVT_TREE_BEGIN_RDRAG(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnBeginRDrag)
EVT_TREE_END_DRAG(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnEndDrag)
EVT_TREE_BEGIN_LABEL_EDIT(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnBeginLabelEdit)
EVT_TREE_END_LABEL_EDIT(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnEndLabelEdit)
EVT_TREE_DELETE_ITEM(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnDeleteItem)
EVT_TREE_SET_INFO(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnSetInfo)
EVT_TREE_ITEM_EXPANDED(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemExpanded)
EVT_TREE_ITEM_EXPANDING(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemExpanding)
EVT_TREE_ITEM_COLLAPSED(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemCollapsed)
EVT_TREE_ITEM_COLLAPSING(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemCollapsing)

EVT_TREE_SEL_CHANGED(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnSelChanged)
EVT_TREE_SEL_CHANGING(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnSelChanging)
EVT_TREE_KEY_DOWN(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnTreeKeyDown)
EVT_TREE_ITEM_ACTIVATED(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemActivated)

// so many differents ways to handle right mouse button clicks...
EVT_CONTEXT_MENU(SkillObjectTree::OnContextMenu)
// EVT_TREE_ITEM_MENU is the preferred event for creating context menus
// on a tree control, because it includes the point of the click or item,
// meaning that no additional placement calculations are required.
EVT_TREE_ITEM_MENU(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemMenu)
EVT_TREE_ITEM_RIGHT_CLICK(ID_SKILL_OBJECT_TREE, SkillObjectTree::OnItemRClick)

EVT_MENU(feID_MENU_SKILL_DELEMENT_ELEMENT, SkillObjectTree::OnMenu)
EVT_MENU(feID_MENU_SKILL_NEW_EFFECT, SkillObjectTree::OnMenu)
EVT_MENU(feID_MENU_SKILL_NEW_CAMERASHAKE, SkillObjectTree::OnMenu)
EVT_MENU(feID_MENU_SKILL_NEW_RIBBON, SkillObjectTree::OnMenu)
EVT_MENU(feID_MENU_SKILL_NEW_SOUND, SkillObjectTree::OnMenu)
EVT_MENU(feID_MENU_SKILL_NEW_SCENELIGHT, SkillObjectTree::OnMenu)
EVT_MENU(FEID_MENU_SKILL_NEW_BULLETFLOW, SkillObjectTree::OnMenu)

EVT_RIGHT_DOWN(SkillObjectTree::OnRMouseDown)
EVT_RIGHT_UP(SkillObjectTree::OnRMouseUp)
EVT_RIGHT_DCLICK(SkillObjectTree::OnRMouseDClick)
END_EVENT_TABLE()

int SkillObjectTree::OnCompareItems(const wxTreeItemId& item1,
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

void SkillObjectTree::GetItemsRecursively(const wxTreeItemId& idParent,
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


void SkillObjectTree::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	wxString text;
	if ( item.IsOk() )
		text << _T('"') << GetItemText(item).c_str() << _T('"');
	else
		text = _T("invalid item");
	wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

long SkillObjectTree::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
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
void SkillObjectTree::AddSkillToTree(Fairy::Skill *pSkill)
{
	// 更新模板
	Fairy::Skill *skill = Fairy::EffectManager::getSingleton().getSkill(pSkill->getSkillName());
    skill = pSkill;

	int iSkillImage = SkillObjectTree::TreeCtrlIcon_Skill;
	wxTreeItemId SkillRootId = AddRoot(skill->getSkillName(),
		iSkillImage, iSkillImage,
		new SkillTreeItemData(wxT("Skill root item"),skill->getSkillName(),skill));

	int iPorpertyImage = SkillObjectTree::TreeCtrlIcon_SkilltProperty;

	wxTreeItemId BasicSettingId= AppendItem(SkillRootId,wxT("BasicSetting"),
		iPorpertyImage, iPorpertyImage,
		new SkillTreeItemData(wxT("Skill BasicSetting"),wxT("Skill BasicSetting"),pSkill));

	if ( BasicSettingId.IsOk() )
	{
		const Ogre::ParameterList &paraList = pSkill->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String name = paraList[paraCount].name;
			Ogre::String value = pSkill->getParameter(paraList[paraCount].name);

			wxTreeItemId itemId =AppendItem( BasicSettingId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
				new SkillTreeItemData(wxT("Skill property item"),name,skill));
		}
	}

	int iAnimEfectImage = SkillObjectTree::TreeCtrlIcon_AnimEffect;
	int iCameraShakeImage = SkillObjectTree::TreeCtrlIcon_CameraShake;
	int iRibbinImage = SkillObjectTree::TreeCtrlIcon_Ribbon;
	int iSoundImage = SkillObjectTree::TreeCtrlIcon_Sound;
	int iSceneLightImage = SkillObjectTree::TreeCtrlIcon_SceneLight;
	int iBulletFlowImage = SkillObjectTree::TreeCtrlIcon_BulletFlow;


	// 设置effect属性
	for ( unsigned short i=0; i < skill->getNumAnimationEffectInfos(); ++i )
	{
		Fairy::AnimationEffectInfo *effectInfo = skill->getAnimationEffectInfo(i);
		assert (effectInfo);

		Ogre::String effectIndexStr = "Effect Info " + Ogre::StringConverter::toString(i);
		wxTreeItemId AnimEffectId= AppendItem(SkillRootId,effectIndexStr.c_str(),
			iAnimEfectImage, iAnimEfectImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("AnimEffect"),effectInfo));

		if ( AnimEffectId.IsOk() )
		{

			const Ogre::ParameterList &paraList = effectInfo->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = effectInfo->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( AnimEffectId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,effectInfo));

			}
		}
	}

	for ( unsigned short i=0; i < skill->getNumAnimationCameraShake(); ++i )
	{
		Fairy::AnimationCameraShake *cameraShake = skill->getAnimationCameraShake(i);

		assert (cameraShake);

		Ogre::String cameraShakeIndexStr = "CameraShake " + Ogre::StringConverter::toString(i);

		wxTreeItemId CameraShakeId= AppendItem(SkillRootId,cameraShakeIndexStr.c_str(),
			iCameraShakeImage, iCameraShakeImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("CameraShake"),cameraShake));

		if ( CameraShakeId.IsOk() )
		{

			const Ogre::ParameterList &paraList = cameraShake->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = cameraShake->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( CameraShakeId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,cameraShake));

			}
		}		
	}

	for ( unsigned short i=0; i < skill->getNumAnimationRibbons(); ++i )
	{
		Fairy::AnimationRibbon *ribbon = skill->getAnimationRibbon(i);

		assert (ribbon);

		Ogre::String ribbonIndexStr = "Ribbon " + Ogre::StringConverter::toString(i);

		wxTreeItemId RibbonId= AppendItem(SkillRootId,ribbonIndexStr.c_str(),
			iRibbinImage, iRibbinImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("Ribbon"),ribbon));

		if ( RibbonId.IsOk() )
		{

			const Ogre::ParameterList &paraList = ribbon->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = ribbon->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( RibbonId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,ribbon));

			}
		}		
	}

	for ( unsigned short i=0; i < skill->getNumAnimationSounds(); ++i )
	{
		Fairy::AnimationSound *sound = skill->getAnimationSound(i);

		assert (sound);

		Ogre::String soundIndexStr = "Sound " + Ogre::StringConverter::toString(i);

		wxTreeItemId SoundId= AppendItem(SkillRootId,soundIndexStr.c_str(),
			iSoundImage, iSoundImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("Sound"),sound));

		if ( SoundId.IsOk() )
		{

			const Ogre::ParameterList &paraList = sound->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = sound->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( SoundId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,sound));

			}
		}	
	}

	for ( unsigned short i=0; i < skill->getNumAnimationSceneLightInfos(); ++i )
	{
		Fairy::AnimationSceneLightInfo *light = skill->getAnimationSceneLightInfo(i);

		assert (light);

		Ogre::String lightIndexStr = "SceneLight " + Ogre::StringConverter::toString(i);

		wxTreeItemId LightId= AppendItem(SkillRootId,lightIndexStr.c_str(),
			iSceneLightImage, iSceneLightImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("SceneLight"),light));

		if ( LightId.IsOk() )
		{

			const Ogre::ParameterList &paraList = light->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = light->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( LightId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,light));

			}
		}		
	}

	for ( unsigned short i=0; i < skill->getNumAnimationBulletFlows(); ++i )
	{
		Fairy::AnimationBulletFlow *bulletFlow = skill->getAnimationBulletFlow(i);

		assert (bulletFlow);

		Ogre::String bulletFlowIndexStr = "BulletFlow " + Ogre::StringConverter::toString(i);

		wxTreeItemId BulletFlowId= AppendItem(SkillRootId,bulletFlowIndexStr.c_str(),
			iBulletFlowImage, iBulletFlowImage,
			new SkillTreeItemData(wxT("Skill Element"),wxT("BulletFlow"),bulletFlow));

		if ( BulletFlowId.IsOk() )
		{

			const Ogre::ParameterList &paraList = bulletFlow->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String name = paraList[paraCount].name;
				Ogre::String value = bulletFlow->getParameter(paraList[paraCount].name);

				wxTreeItemId itemId =AppendItem( BulletFlowId,(name + ":" + value).c_str(),iPorpertyImage,iPorpertyImage,
					new SkillTreeItemData(wxT("Skill Element item"),name,bulletFlow));

			}
		}		
	}

	Expand(SkillRootId);

}
void SkillObjectTree::CreateImageList(int size)
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
	wxBitmap bitmaps[8];
	bitmaps[0] = wxBITMAP(SKILL);
	bitmaps[1] = wxBITMAP(ANIMEFFECT);
	bitmaps[2] = wxBITMAP(CAMERASHAKE);
	bitmaps[3] = wxBITMAP(RIBBON);
	bitmaps[4] = wxBITMAP(SOUND);
	bitmaps[5] = wxBITMAP(SCENELIGHT);
	bitmaps[6] = wxBITMAP(ANIMBULLETFLOW);
	bitmaps[7] = wxBITMAP(SKILLPROPERTY);


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
	void SkillObjectTree::name(wxTreeEvent& event)                        \
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


//选择粒子基本参数Item
void SkillObjectTree::SelectSkillSetting()
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);
	while(childId.IsOk())
	{
		SkillTreeItemData* pItemData = dynamic_cast<SkillTreeItemData*>(GetItemData(childId)); 
		if(pItemData->GetDesc() == "Skill BasicSetting")
		{

			//SelectItem(childId);
			Expand(childId);
			mSelectSkillObject = NULL;
			break;
		}
		childId = GetNextSibling(childId);
	}

}
void SkillObjectTree::SelectElementItem(Ogre::StringInterface* element)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		SkillTreeItemData* pItemData = dynamic_cast<SkillTreeItemData*>(GetItemData(childId)); 
		Ogre::StringInterface* pElement = (Ogre::StringInterface*)pItemData->GetData();
		if(pItemData->GetDesc() == "Skill Element" && pElement == element)
		{
			this->SelectItem(childId);
			this->Expand(childId);
			return;
		}
		childId = GetNextSibling(childId);
	}
}
void SkillObjectTree::OnTreeKeyDown(wxTreeEvent& event)
{
	
	event.Skip();
}

void SkillObjectTree::OnBeginDrag(wxTreeEvent& event)
{
	// need to explicitly allow drag
	if ( event.GetItem() != GetRootItem() )
	{
		m_draggedItem = event.GetItem();

		wxPoint clientpt = event.GetPoint();
		wxPoint screenpt = ClientToScreen(clientpt);

		//wxLogMessage(wxT("OnBeginDrag: started dragging %s at screen coords (%i,%i)"),
		//	GetItemText(m_draggedItem).c_str(),
		//	screenpt.x, screenpt.y);

		event.Allow();
	}
	else
	{
		//wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
	}
}

void SkillObjectTree::OnEndDrag(wxTreeEvent& event)
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

void SkillObjectTree::OnBeginLabelEdit(wxTreeEvent& event)
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

void SkillObjectTree::OnEndLabelEdit(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnEndLabelEdit"));

	// don't allow anything except letters in the labels
	if ( !event.GetLabel().IsWord() )
	{
		wxMessageBox(wxT("The new label should be a single word."));

		event.Veto();
	}
}

void SkillObjectTree::OnItemCollapsing(wxTreeEvent& event)
{
	wxLogMessage(wxT("OnItemCollapsing"));

	// for testing, prevent the user from collapsing the first child folder
	wxTreeItemId itemId = event.GetItem();
}

void SkillObjectTree::OnItemActivated(wxTreeEvent& event)
{
	// show some info about this item
	wxTreeItemId itemId = event.GetItem();
	SkillTreeItemData *item = (SkillTreeItemData *)GetItemData(itemId);

	if ( item != NULL )
	{
		item->ShowInfo(this);
	}

	wxLogMessage(wxT("OnItemActivated"));
}

void SkillObjectTree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	
	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);

	ShowMenu(itemId, clientpt);
	event.Skip();
}
void SkillObjectTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	SkillTreeItemData *item = id.IsOk() ? (SkillTreeItemData *)GetItemData(id): NULL;
	if (item == NULL)
	{
		return;
	}
	
	if (item->GetDesc() == "Skill root item")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_SKILL_NEW_EFFECT, wxT("添加效果"));		
		menu.Append(feID_MENU_SKILL_NEW_CAMERASHAKE, wxT("添加相机震动"));		
		menu.Append(feID_MENU_SKILL_NEW_RIBBON, wxT("添加刀光"));		
		menu.Append(feID_MENU_SKILL_NEW_SOUND, wxT("添加声音"));		
		menu.Append(feID_MENU_SKILL_NEW_SCENELIGHT, wxT("添加场景灯光"));		
		menu.Append(FEID_MENU_SKILL_NEW_BULLETFLOW, wxT("添加导弹"));		
		PopupMenu(&menu, pt);
	}
	else if (item->GetDesc() == "Skill Element")
	{
		wxMenu menu("");
		menu.Append(feID_MENU_SKILL_DELEMENT_ELEMENT, wxT("删除元素"));		
		PopupMenu(&menu, pt);
	}
}

void SkillObjectTree::OnMenu(wxCommandEvent & event)
{
	int id = event.GetId();
	SkillObjectEditor * parent = static_cast <SkillObjectEditor*>(GetParent());
	if (parent == NULL)
	{
		return;
	}

	if (id == feID_MENU_SKILL_DELEMENT_ELEMENT)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_DELETE_ElEMENT);
	}
	else if (id == feID_MENU_SKILL_NEW_EFFECT)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_EFFECT);
	}
	else if (id == feID_MENU_SKILL_NEW_CAMERASHAKE)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_CAMERASHAKE);
	}
	else if (id == feID_MENU_SKILL_NEW_RIBBON)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_RIBBON);
	}
	else if (id == feID_MENU_SKILL_NEW_SOUND)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_SOUND);
	}
	else if (id == feID_MENU_SKILL_NEW_SCENELIGHT)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_SCENELIGHT);
	}
	else if (id == FEID_MENU_SKILL_NEW_BULLETFLOW)
	{
		parent->OnNotifyToolBarCtrl(SkillObjectEditor::TC_NEW_BULLETFLOW);
	}
}

void SkillObjectTree::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint pt = event.GetPosition();

	wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);
}


void SkillObjectTree::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	SkillTreeItemData *item = itemId.IsOk() ? (SkillTreeItemData *)GetItemData(itemId)
		: NULL;

	wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc()
		: _T(""));

	event.Skip();
}

void SkillObjectTree::OnRMouseDown(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button down"));

	event.Skip();
}

void SkillObjectTree::OnRMouseUp(wxMouseEvent& event)
{
	wxLogMessage(wxT("Right mouse button up"));

	event.Skip();
}

void SkillObjectTree::OnRMouseDClick(wxMouseEvent& event)
{
	wxTreeItemId id = HitTest(event.GetPosition());
	if ( !id )
		wxLogMessage(wxT("No item under mouse"));
	else
	{
		SkillTreeItemData *item = (SkillTreeItemData *)GetItemData(id);
		if ( item )
			wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
	}

	event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
	return b ? wxT("") : wxT("not ");
}
void SkillTreeItemData::ShowInfo(wxTreeCtrl *tree)
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

void SkillObjectTree::OnSelChanging(wxTreeEvent& event)
{

}

void SkillObjectTree::OnSelChanged(wxTreeEvent& event)
{

	wxTreeItemId id = event.GetItem();
	if ( !id )
	{
		wxLogMessage(wxT("No item under mouse"));
	}

	SkillTreeItemData* pItemData = dynamic_cast<SkillTreeItemData*>(GetItemData(id)); 
	if(pItemData)
	{

		SkillObjectEditor* pEditor = dynamic_cast<SkillObjectEditor*>(m_parent);
		EffectObjectPropertyEditor * pPropertyEditor = pEditor->GetParentFrame()->GetEffectObjectProperty();

		if(pItemData->GetDesc() == "Skill BasicSetting" || 
			pItemData->GetDesc() == "Skill root item" ||
			pItemData->GetDesc() == "Skill property item")
		{
			pPropertyEditor->InitSkillSettingProperty();
			mSelectSkillObject = NULL;
		}
		else if(pItemData->GetDesc() == "Skill Element" ||
			pItemData->GetDesc() == "Skill Element item")
		{
			Ogre::StringInterface* pSkillElement = (Ogre::StringInterface*)pItemData->GetData();
			pPropertyEditor->InitSkillElementProperty(pSkillElement);
			mSelectSkillObject = pSkillElement;
		}
	}

	event.Skip();
}
void SkillObjectTree::refrashSelected()
{
	wxTreeItemId selectedId = this->GetSelection();
	if(selectedId)
	{
		this->SelectItem(selectedId);
	}
}
void SkillObjectTree::refreshItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	wxTreeItemId rootId = GetRootItem();
	wxTreeItemIdValue idValue;
	wxTreeItemId subId;
	wxTreeItemId childId = GetFirstChild(rootId,idValue);

	while(childId.IsOk())
	{
		SkillTreeItemData* pItemData = dynamic_cast<SkillTreeItemData*>(GetItemData(childId)); 
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
			SkillTreeItemData* pItemData = dynamic_cast<SkillTreeItemData*>(GetItemData(subId)); 
			if(pItemData->GetDesc() == Desc && pItemData->GetData() == data  && pItemData->GetValue() == propertyName)
			{
				Ogre::StringInterface* pObject = (Ogre::StringInterface*)data;
				Ogre::String value = pObject->getParameter(propertyName.c_str());
				this->SetItemText(subId,(propertyName + ":" + value).c_str());
				return;
			}
			subId = GetNextSibling(subId);
		}
		childId = GetNextSibling(childId);
	}

}