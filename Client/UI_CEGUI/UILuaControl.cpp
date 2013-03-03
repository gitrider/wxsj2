
#include "StdAfx.h"
#include "UIGlobal.h"
#include "UILuaControl.h"
#include "GIScriptSystem.h"
#include "GIException.h"
#include "UISystem.h"
#include "UIIconsMng.h"
#include <elements\CEGUIProgressBar.h>
#include "ceguipropertyhelper.h"
#include <CEGUIFontManager.h>
#include "UIString.h"
#include <elements\CEGUIPopupMenu.h>
#include <elements\ceguilistbox.h>
#include <elements\CEGUIScrollbar.h>
#include <elements\ceguilistboxtextitem.h>
#include <elements\ceguicombobox.h>
#include <elements\ceguimulticolumnlist.h>
#include "..\CEGUIFalagardEx\FalActionButton.h"
#include "..\CEGUIFalagardEx\FalagardComplexWindow.h"
#include "..\CEGUIFalagardEx\FalagardEditbox.h"
#include "..\CEGUIFalagardEx\FalMultiIMEEditBox.h"
#include "..\CEGUIFalagardEx\FalSuperTooltip.h"
#include "..\CEGUIFalagardEx\Falagardinfolist.h"
#include "..\CEGUIFalagardEx\Falagardminimap.h"
#include "..\CEGUIFalagardEx\Falagardradar.h"
#include "..\CEGUIFalagardEx\FalagardScenemap.h"
#include "..\CEGUIFalagardEx\FalagardAnimate.h"
#include "..\CEGUIFalagardEx\FalagardSuperProgress.h"
#include "..\CEGUIFalagardEx\FalTree.h"
#include "CEGUIPropertyHelper.h"
#include "GIObjectManager.h"
#include "GIWorldSystem.h"
#include "GIDBC_Struct.h"
#include "UIWindowMng.h"
#include "GIEventDefine.h"
#include "GIFakeObjectManager.h"
#include "..\CEGUIFalagardEx\FalChatHistory.h"
#include "..\CEGUIFalagardEx\FalChatChannel.h"
#include "..\CEGUIFalagardEx\FalagardWorldmap.h"
#include "..\CEGUIFalagardEx\FalagardSoftKeyBoard.h"
#include "..\CEGUIFalagardEx\FalagardScrollInfo.h"  // 滚动公告板类		20100624 BLL
#include "GIDataBase.h"
#include "AxTrace.h"
#include "AxProfile.h"

#include "CEGUIImageset.h"
#include "CEGUITexture.h"
#include "CeguiScriptModule.h"
#include "..\CEGUIFalagardEx\FalIMEEditBox.h"
#include "UIString_Encode.h"


namespace LUA_CONTROL
{

LuaPlus::LuaObject* Window::s_pMetaTable = NULL;


Window*	Window::CreateControl(CEGUI::Window* pWindow)
{
	CEGUI::String strType = pWindow->getWidgetType();

	if(strType == "Falagard/ActionButton")
	{
		return new ActionButton(pWindow);
	}
	else if(strType == "Falagard/ProgressBar")
	{
		return new ProgressBar(pWindow);
	}
	else if( strType == "Falagard/SuperProgress" )
	{
		return new SuperProgressBar(pWindow);
	}
	else if(strType == "Falagard/PopupMenu")
	{
		return new PopMenu(pWindow);
	}
	else if( strType == "Falagard/Radar" )
	{
		return new Minimap( pWindow );
	}
	else if( strType == "Falagard/ComplexWindow" )
	{
		return new ComplexWindow( pWindow );
	}
	else if( strType == "Falagard/Listbox" )
	{
		return new ListBox( pWindow );
	}
	else if ( strType == "Falagard/ImageListbox" )
	{
		return new ImageListBox( pWindow );
	}
	else if ( strType == "Falagard/ImageListboxEx" )
	{
		return new ImageListBoxEx( pWindow );
	}
	else if( strType =="Falagard/CheckBox" )
	{
		return new CheckButton( pWindow );
	}
	else if( strType == "Falagard/Combobox" )
	{
		return new ComboBox( pWindow );
	}
	else if( strType == "Falagard/StaticImage" )
	{
		return new StaticImage( pWindow );
	}
	else if( strType == "Falagard/Scrollbar" )
	{
		return new ScrollBar( pWindow );
	}
	else if( strType == "Falagard/SuperTooltip" )
	{
		return new SuperTooltip( pWindow );
	}
	else if( strType == "Falagard/MeshWindow" )
	{
		return new MeshWindow( pWindow );
	}
	else if(strType == "Falagard/ChatHistory")
	{
		return new ChatHistory(pWindow);
	}
	else if(strType == "Falagard/ChatChannel")
	{
		return new ChatChannel(pWindow);
	}
	else if( strType == "Falagard/SceneMap" )
	{
		return new Scenemap( pWindow );
	}
	else if( strType == "Falagard/WorldMap" )
	{
		return new Worldmap( pWindow );
	}
	else if( strType == "Falagard/InfoList" )
	{
		return new InfoList( pWindow );
	}
	else if( strType == "Falagard/MultiColumnList" )
	{
		return new CtrlList( pWindow );
	}
	else if( strType == "Falagard/SoftKeyWindow" )
	{
		return new SoftKeyWindow( pWindow );
	}
	else if( strType == "Falagard/Animate" )
	{
		return new AnimateWindow( pWindow );
	}

	else if( strType == (CEGUI::utf8*)"Falagard/IMEEditBox" )
	{
		return new EditBox( pWindow );
	}
	else if ( strType == (CEGUI::utf8*)"Falagard/MultiIMEEditBox" )
	{
		return new MultiLineEditbox( pWindow );
	}
	else if( strType == (CEGUI::utf8*)"Falagard/ScrollInfo" )  // 滚动公告板类		20100624 BLL
	{
		return new ScrollInfo( pWindow );
	}
	else if ( strType == (CEGUI::utf8*)"Falagard/Tree" )	//树型控件
	{
		return new Tree( pWindow );
	}
	else
	{
		return new Window(pWindow);
	}
}

VOID Window::DestroyControl(Window* pThis)
{
	delete pThis;
}


VOID Window::RegisterMetaTable(VOID)
{
	LuaPlus::LuaState* pLuaState = g_pScriptSys->GetLuaState();
	KLAssert(pLuaState);

	//-----------------------------------------------------------
	//0. Window
	Window::s_pMetaTable = new LuaPlus::LuaObject;
	*Window::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Window");
	Window::s_pMetaTable->SetObject("__index", *Window::s_pMetaTable);

	Window::s_pMetaTable->RegisterObjectFunctor("SetProperty", &Window::setProperty);
	Window::s_pMetaTable->RegisterObjectFunctor("GetProperty", &Window::getProperty);
	Window::s_pMetaTable->RegisterObjectFunctor("GetType"	, &Window::getType);
	Window::s_pMetaTable->RegisterObjectFunctor("GetName"	, &Window::getName);
	Window::s_pMetaTable->RegisterObjectFunctor("GetText"	, &Window::getText);
	Window::s_pMetaTable->RegisterObjectFunctor("SetText"	, &Window::setText);
	Window::s_pMetaTable->RegisterObjectFunctor("SetTextOriginal"	, &Window::setTextOriginal);
	Window::s_pMetaTable->RegisterObjectFunctor("TransText"	, &Window::TransText);
	Window::s_pMetaTable->RegisterObjectFunctor("Show"		, &Window::Show);
	Window::s_pMetaTable->RegisterObjectFunctor("Hide"		, &Window::Hide);
	Window::s_pMetaTable->RegisterObjectFunctor("Disable"	, &Window::Disable);
	Window::s_pMetaTable->RegisterObjectFunctor("Enable"	, &Window::Enable);
	Window::s_pMetaTable->RegisterObjectFunctor("IsVisible"	, &Window::IsVisible);
	Window::s_pMetaTable->RegisterObjectFunctor("SetForce"	, &Window::setForce);
	Window::s_pMetaTable->RegisterObjectFunctor("SetToolTip", &Window::SetToolTip);
	Window::s_pMetaTable->RegisterObjectFunctor("CaptureInput", &Window::CaptureInput);
	Window::s_pMetaTable->RegisterObjectFunctor("CenterWindow", &Window::CenterWindow);
	Window::s_pMetaTable->RegisterObjectFunctor("SetWindowCenter", &Window::SetWindowCenter);
	Window::s_pMetaTable->RegisterObjectFunctor("SetClippedByParent", &Window::SetClippedByParent);
	Window::s_pMetaTable->RegisterObjectFunctor("AutoMousePosition", &Window::AutoMousePosition);
	Window::s_pMetaTable->RegisterObjectFunctor("SetAlpha", &Window::setAlpha);
	Window::s_pMetaTable->RegisterObjectFunctor("SetAreaAndTexCoord", &Window::setAreaAndTexCoord);
	Window::s_pMetaTable->RegisterObjectFunctor("CreateChildWindow", &Window::CreateChildWindow);
	Window::s_pMetaTable->RegisterObjectFunctor("RemoveChildWindow", &Window::RemoveChildWindow);
	Window::s_pMetaTable->RegisterObjectFunctor("DebugWindowInfo", &Window::DebugWindowInfo);
	Window::s_pMetaTable->RegisterObjectFunctor("SetEvent", &Window::SetEvent);
	Window::s_pMetaTable->RegisterObjectFunctor("SetPosition", &Window::SetPosition);
	Window::s_pMetaTable->RegisterObjectFunctor("GetPosition", &Window::GetPosition);

	
	//-----------------------------------------------------------
	//1. Progress
	ProgressBar::s_pMetaTable = new LuaPlus::LuaObject;
	*ProgressBar::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ProgressBar");
	//*ProgressBar::s_pMetaTable = *Window::s_pMetaTable;
	ProgressBar::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ProgressBar::s_pMetaTable->SetObject("__index", *ProgressBar::s_pMetaTable);

	ProgressBar::s_pMetaTable->RegisterObjectFunctor("SetProgress"	, &ProgressBar::SetProgress);

		//-----------------------------------------------------------
	//1.1 SuperProgress
	SuperProgressBar::s_pMetaTable = new LuaPlus::LuaObject;
	*SuperProgressBar::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SuperProgressBar");
	//*ProgressBar::s_pMetaTable = *Window::s_pMetaTable;
	SuperProgressBar::s_pMetaTable->SetMetaTable( *ProgressBar::s_pMetaTable );
	SuperProgressBar::s_pMetaTable->SetObject("__index", *ProgressBar::s_pMetaTable);


	//-----------------------------------------------------------
	//2. ActionButton
	ActionButton::s_pMetaTable = new LuaPlus::LuaObject;
	*ActionButton::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ActionButton");
	//*ActionButton::s_pMetaTable = *Window::s_pMetaTable;
	ActionButton::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ActionButton::s_pMetaTable->SetObject("__index", *ActionButton::s_pMetaTable);

	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetActionItem"	, &ActionButton::Lua_SetActionItem);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("DoAction"		, &ActionButton::Lua_DoAction);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("DoSubAction"		, &ActionButton::Lua_DoSubAction);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetPushed"		, &ActionButton::Lua_SetPushed);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetFlash"		, &ActionButton::Lua_SetFlash);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("Gloom"			, &ActionButton::Lua_Gloom);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("Bright"			, &ActionButton::Lua_Bright);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetActionSystemKeyState", &ActionButton::SetActionSystemKeyState);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("GetActionItemId", &ActionButton::GetActionItemId);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("GetActionItemDefineId", &ActionButton::GetActionItemDefineId);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetLogicItemData", &ActionButton::setLogicItemData);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetActionItemIconName", &ActionButton::setActionItemIconName);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetNormalImage", &ActionButton::setNormalImage);


	//-----------------------------------------------------------
	//3. PopMenu
	PopMenu::s_pMetaTable = new LuaPlus::LuaObject;
	*PopMenu::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_PopMenu");
	//*PopMenu::s_pMetaTable = *Window::s_pMetaTable;
	PopMenu::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	PopMenu::s_pMetaTable->SetObject("__index", *PopMenu::s_pMetaTable);

	PopMenu::s_pMetaTable->RegisterObjectFunctor("OpenPopMenu"	,	&PopMenu::OpenPopMenu);
	PopMenu::s_pMetaTable->RegisterObjectFunctor("ClosePopMenu"	,	&PopMenu::ClosePopMenu);
	PopMenu::s_pMetaTable->RegisterObjectFunctor("SetPopMenuPos",	&PopMenu::SetPopMenuPos);

	//-----------------------------------------------------------
	//4. ComplexWindow
	ComplexWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*ComplexWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ComplexWindow");
	//*ComplexWindow::s_pMetaTable = *Window::s_pMetaTable;
	ComplexWindow::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ComplexWindow::s_pMetaTable->SetObject("__index", *ComplexWindow::s_pMetaTable);

	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddTextElement"	, &ComplexWindow::AddTextElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddOptionElement"	, &ComplexWindow::AddOptionElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddItemElement"	, &ComplexWindow::AddItemElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddActionElement"	, &ComplexWindow::AddActionElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddMoneyElement"	, &ComplexWindow::AddMoneyElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddImpactElement"	, &ComplexWindow::AddImpactElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddMoneyElement"	, &ComplexWindow::AddMoneyElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("ClearAllElement"	, &ComplexWindow::CleanAllElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("SetTextColour"	, &ComplexWindow::SetTextColour);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("PageEnd"	, &ComplexWindow::PageEnd);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddChatBoardElement", &ComplexWindow::AddChatBoardElement);


	//-----------------------------------------------------------
	//19. Scenemap
	Scenemap::s_pMetaTable = new LuaPlus::LuaObject;
	*Scenemap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SceneMapWindow");
	//*Scenemap::s_pMetaTable = *Window::s_pMetaTable;
	Scenemap::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	Scenemap::s_pMetaTable->SetObject("__index", *Scenemap::s_pMetaTable);

	
	Scenemap::s_pMetaTable->RegisterObjectFunctor("SetSceneFileName", &Scenemap::SetSceneFileName );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("CloseSceneMap", &Scenemap::CloseSceneMap );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("GetMouseScenePos", &Scenemap::GetMouseScenePos );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("UpdateSceneMap", &Scenemap::UpdateSceneMap );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("SetSceneZoomMode", &Scenemap::SetSceneZoomMode );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("UpdateViewRect", &Scenemap::UpdateViewRect);
	Scenemap::s_pMetaTable->RegisterObjectFunctor("moveMapByDirection", &Scenemap::moveMapByDirection);
	Scenemap::s_pMetaTable->RegisterObjectFunctor("AddTeamPlayerPos", &Scenemap::AddTeamPlayerPos);
	//Scenemap::s_pMetaTable->RegisterObjectFunctor("SceneClear", &Scenemap::SceneClear);

	//-----------------------------------------------------------
	//5.Minimap
	Minimap::s_pMetaTable = new LuaPlus::LuaObject;
	*Minimap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Minimap");
	//*Minimap::s_pMetaTable = *Window::s_pMetaTable;
	Minimap::s_pMetaTable->SetMetaTable( *Scenemap::s_pMetaTable );
	Minimap::s_pMetaTable->SetObject("__index", *Minimap::s_pMetaTable);

	Minimap::s_pMetaTable->RegisterObjectFunctor( "UpdateFlag", &Minimap::UpdateFlag);

	//-----------------------------------------------------------
	//6. ListBox
	ListBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ListBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Listbox");
	//*ListBox::s_pMetaTable = *Window::s_pMetaTable;
	ListBox::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ListBox::s_pMetaTable->SetObject("__index", *ListBox::s_pMetaTable);

	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemTooltip", &ListBox::SetItemTooltip );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemTooltipState", &ListBox::SetItemTooltipState );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ListBox::AddItem );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetTagText", &ListBox::SetTagText);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetRefIndex", &ListBox::SetRefIndex);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetListItemText", &ListBox::SetItemText );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetItem", &ListBox::GetItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "DelItem", &ListBox::DelItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "ClearListBox", &ListBox::ClearListBox);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetFirstSelectItem", &ListBox::GetFirstSelectItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetFirstSelectItemString", &ListBox::GetFirstSelectItemString);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetFirstSelectItemString", &ListBox::SetFirstSelectItemString);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemSelect", &ListBox::SetItemSelect);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemSelectByItemID", &ListBox::SetItemSelectByItemID);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "ClearAllSelections", &ListBox::ClearAllSelections);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetMouseMoveItem", &ListBox::GetMouseMoveItem);


	//-----------------------------------------------------------
	//7. ChatWindows

	//-----------------------------------------------------------
	//8. ChatInputWindow

	//-----------------------------------------------------------
	//9. ImageListBox
	ImageListBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ImageListBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageListbox");
	//*ImageListBox::s_pMetaTable = *Window::s_pMetaTable;
	ImageListBox::s_pMetaTable->SetMetaTable( *ListBox::s_pMetaTable );
	ImageListBox::s_pMetaTable->SetObject("__index", *ImageListBox::s_pMetaTable);

	ImageListBox::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ImageListBox::AddItem );

	//-----------------------------------------------------------
	//10. ImageListBoxEx
	ImageListBoxEx::s_pMetaTable = new LuaPlus::LuaObject;
	*ImageListBoxEx::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageListboxEx");
	//*ImageListBoxEx::s_pMetaTable = *Window::s_pMetaTable;
	ImageListBoxEx::s_pMetaTable->SetMetaTable( *ListBox::s_pMetaTable );
	ImageListBoxEx::s_pMetaTable->SetObject("__index", *ImageListBoxEx::s_pMetaTable);

	ImageListBoxEx::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ImageListBoxEx::AddItem );

	//-----------------------------------------------------------
	//11. CheckBox
	CheckButton::s_pMetaTable = new LuaPlus::LuaObject;
	*CheckButton::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageCheckbox");
	//*CheckButton::s_pMetaTable = *Window::s_pMetaTable;
	CheckButton::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	CheckButton::s_pMetaTable->SetObject("__index", *CheckButton::s_pMetaTable);

	CheckButton::s_pMetaTable->RegisterObjectFunctor( "SetCheck", &CheckButton::SetCheck );
	CheckButton::s_pMetaTable->RegisterObjectFunctor( "GetCheck", &CheckButton::GetCheck );

	//-----------------------------------------------------------
	//12. ComboBox
	ComboBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ComboBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ComboBox");
	//*ComboBox::s_pMetaTable = *Window::s_pMetaTable;
	ComboBox::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ComboBox::s_pMetaTable->SetObject("__index", *ComboBox::s_pMetaTable);

	ComboBox::s_pMetaTable->RegisterObjectFunctor( "ComboBoxAddItem", &ComboBox::ComboBoxAddItem);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "ResetList", &ComboBox::ResetList);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "GetCurrentSelect", &ComboBox::GetCurrentSelect);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "SetCurrentSelect", &ComboBox::SetCurrentSelect);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "SetItemText", &ComboBox::SetItemText);
	//-----------------------------------------------------------
	//13. StaticImage
	StaticImage::s_pMetaTable = new LuaPlus::LuaObject;
	*StaticImage::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_StaticImage");
	//*StaticImage::s_pMetaTable = *Window::s_pMetaTable;
	StaticImage::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	StaticImage::s_pMetaTable->SetObject("__index", *StaticImage::s_pMetaTable);

	StaticImage::s_pMetaTable->RegisterObjectFunctor( "SetImage", &StaticImage::SetImage);
	StaticImage::s_pMetaTable->RegisterObjectFunctor( "SetImageColor", &StaticImage::SetImageColor);

	//-----------------------------------------------------------
	//14. ScrollBar
	ScrollBar::s_pMetaTable = new LuaPlus::LuaObject;
	*ScrollBar::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ScrollBar");
	//*ScrollBar::s_pMetaTable = *Window::s_pMetaTable;
	ScrollBar::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ScrollBar::s_pMetaTable->SetObject("__index", *ScrollBar::s_pMetaTable);

	ScrollBar::s_pMetaTable->RegisterObjectFunctor( "SetPosition", &ScrollBar::SetPosition );
	ScrollBar::s_pMetaTable->RegisterObjectFunctor( "GetPosition", &ScrollBar::GetPosition );

	//-----------------------------------------------------------
	//15. SuperTooltip
	SuperTooltip::s_pMetaTable = new LuaPlus::LuaObject;
	*SuperTooltip::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SuperTooltip");
	//*SuperTooltip::s_pMetaTable = *Window::s_pMetaTable;
	SuperTooltip::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	SuperTooltip::s_pMetaTable->SetObject("__index", *SuperTooltip::s_pMetaTable);

	SuperTooltip::s_pMetaTable->RegisterObjectFunctor( "PositionSelf", &SuperTooltip::PositionSelf );
	SuperTooltip::s_pMetaTable->RegisterObjectFunctor( "modifyWidth", &SuperTooltip::modifyWidth );

	//-----------------------------------------------------------
	//16. MeshWindow
	MeshWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*MeshWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_MeshWindow");
	//*MeshWindow::s_pMetaTable = *Window::s_pMetaTable;
	MeshWindow::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	MeshWindow::s_pMetaTable->SetObject("__index", *MeshWindow::s_pMetaTable);

	MeshWindow::s_pMetaTable->RegisterObjectFunctor("SetFakeObject", &MeshWindow::SetFakeObject );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("RotateBegin", &MeshWindow::RotateBegin );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("RotateEnd", &MeshWindow::RotateEnd );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("GetObjName", &MeshWindow::GetObjName );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("SetUnFakeObject", &MeshWindow::SetUnFakeObject );
	
	//-----------------------------------------------------------
	//17. ChatHistory
	ChatHistory::s_pMetaTable = new LuaPlus::LuaObject;
	*ChatHistory::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ChatHistory");
	//*ChatHistory::s_pMetaTable = *Window::s_pMetaTable;
	ChatHistory::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ChatHistory::s_pMetaTable->SetObject("__index", *ChatHistory::s_pMetaTable);

	ChatHistory::s_pMetaTable->RegisterObjectFunctor("InsertChatString", &ChatHistory::InsertChatString);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("RemoveAllChatString", &ChatHistory::RemoveAllChatString);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("ScrollToEnd", &ChatHistory::ScrollToEnd);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("ExtendClearRegion", &ChatHistory::ExtendClearRegion);
	//ChatHistory::s_pMetaTable->RegisterObjectFunctor("OnMouseButtonUp", &ChatHistory::OnMouseButtonUp);

	//-----------------------------------------------------------
	//18. ChatChannel
	ChatChannel::s_pMetaTable = new LuaPlus::LuaObject;
	*ChatChannel::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ChatChannel");
	//*ChatChannel::s_pMetaTable = *Window::s_pMetaTable;
	ChatChannel::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	ChatChannel::s_pMetaTable->SetObject("__index", *ChatChannel::s_pMetaTable);

	ChatChannel::s_pMetaTable->RegisterObjectFunctor("ClearAllChannel", &ChatChannel::ClearAllChannel);
	ChatChannel::s_pMetaTable->RegisterObjectFunctor("AddChannel", &ChatChannel::AddChannel);
	

	//20. Worldmap
	Worldmap::s_pMetaTable = new LuaPlus::LuaObject;
	*Worldmap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_WorldMapWindow");
	//*Worldmap::s_pMetaTable = *Window::s_pMetaTable;
	Worldmap::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	Worldmap::s_pMetaTable->SetObject("__index", *Worldmap::s_pMetaTable);

	Worldmap::s_pMetaTable->RegisterObjectFunctor("InitWorldMap", &Worldmap::InitWorldMap );
	Worldmap::s_pMetaTable->RegisterObjectFunctor("GetCurrentSelectScene", &Worldmap::GetCurrentSelectScene );
	Worldmap::s_pMetaTable->RegisterObjectFunctor("UpdateWorldMap", &Worldmap::UpdateWorldMap );
	Worldmap::s_pMetaTable->RegisterObjectFunctor("SetCityHover", &Worldmap::SetCityHover);
	

	// 21. InfoList
	InfoList::s_pMetaTable = new LuaPlus::LuaObject;
	*InfoList::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_InfoListWindow");
	//*InfoList::s_pMetaTable = *Window::s_pMetaTable;
	InfoList::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	InfoList::s_pMetaTable->SetObject("__index", *InfoList::s_pMetaTable);

	InfoList::s_pMetaTable->RegisterObjectFunctor("AddInfo", &InfoList::AddInfo );

	// 22 CtrlList
	CtrlList::s_pMetaTable = new LuaPlus::LuaObject;
	*CtrlList::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_CtrlListWindow");
	//*CtrlList::s_pMetaTable = *Window::s_pMetaTable;
	CtrlList::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	CtrlList::s_pMetaTable->SetObject("__index", *CtrlList::s_pMetaTable);

	CtrlList::s_pMetaTable->RegisterObjectFunctor("AddColumn", &CtrlList::AddColumn );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("InsertColumn", &CtrlList::InsertColumn );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("AddNewItem", &CtrlList::AddNewItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("SetItemText", &CtrlList::SetItemText );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("SetItemData", &CtrlList::SetItemData );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("DeleteItem", &CtrlList::DeleteItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("RemoveAllItem", &CtrlList::RemoveAllItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetItemText", &CtrlList::GetItemText );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetItemCount", &CtrlList::GetItemCount );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetColumnCount", &CtrlList::GetColumnCount );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetSelectItem", &CtrlList::GetSelectItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("RemoveColumn", &CtrlList::RemoveColumn );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("SetSelectItem", &CtrlList::SetSelectItem );

	// 23 SoftKeyWindow
	SoftKeyWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*SoftKeyWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SoftKeyWindow");
	//*SoftKeyWindow::s_pMetaTable = *Window::s_pMetaTable;
	SoftKeyWindow::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	SoftKeyWindow::s_pMetaTable->SetObject("__index", *SoftKeyWindow::s_pMetaTable);

	SoftKeyWindow::s_pMetaTable->RegisterObjectFunctor("SetAimEditBox", &SoftKeyWindow::SetAimEditBox );

	// 24 Animate
	AnimateWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*AnimateWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_AnimateWindow");
	//*AnimateWindow::s_pMetaTable = *Window::s_pMetaTable;
	AnimateWindow::s_pMetaTable->SetMetaTable( *Window::s_pMetaTable );
	AnimateWindow::s_pMetaTable->SetObject("__index", *AnimateWindow::s_pMetaTable);

	AnimateWindow::s_pMetaTable->RegisterObjectFunctor("Play", &AnimateWindow::Play);

	// 25 EditBox
	EditBox::s_pMetaTable = new LuaPlus::LuaObject;
	*EditBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_EditBox");
	EditBox::s_pMetaTable->SetMetaTable(*Window::s_pMetaTable);
	EditBox::s_pMetaTable->SetObject("__index", *EditBox::s_pMetaTable);

	EditBox::s_pMetaTable->RegisterObjectFunctor("SetItemElementsString", &EditBox::setItemElementsString);
	EditBox::s_pMetaTable->RegisterObjectFunctor("GetItemElementsString", &EditBox::getItemElementsString);
	EditBox::s_pMetaTable->RegisterObjectFunctor("SetBlinkText", &EditBox::setBlinkText);
	EditBox::s_pMetaTable->RegisterObjectFunctor("SetSelected", &EditBox::setSelected);

	// 26 MultiLineEditbox
	MultiLineEditbox::s_pMetaTable = new LuaPlus::LuaObject;
	*MultiLineEditbox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_MultiLineEditBox");
	MultiLineEditbox::s_pMetaTable->SetMetaTable(*Window::s_pMetaTable);
	MultiLineEditbox::s_pMetaTable->SetObject("__index", *MultiLineEditbox::s_pMetaTable);

	MultiLineEditbox::s_pMetaTable->RegisterObjectFunctor("SetItemElementsString", &MultiLineEditbox::setItemElementsString);
	MultiLineEditbox::s_pMetaTable->RegisterObjectFunctor("GetItemElementsString", &MultiLineEditbox::getItemElementsString);

	// 27 ScrollInfo 滚动公告板 20100624
	ScrollInfo::s_pMetaTable = new LuaPlus::LuaObject;
	*ScrollInfo::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ScrollInfo");
	ScrollInfo::s_pMetaTable->SetMetaTable(*Window::s_pMetaTable);
	ScrollInfo::s_pMetaTable->SetObject("__index", *ScrollInfo::s_pMetaTable);

	ScrollInfo::s_pMetaTable->RegisterObjectFunctor("SetScrollInfo", &ScrollInfo::setScrollInfo );

	// 28 树型控件类
	Tree::s_pMetaTable = new LuaPlus::LuaObject;
	*Tree::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Tree");
	Tree::s_pMetaTable->SetMetaTable(*Window::s_pMetaTable);
	Tree::s_pMetaTable->SetObject("__index", *Tree::s_pMetaTable);

	Tree::s_pMetaTable->RegisterObjectFunctor( "AddItem", &Tree::AddItem );
	Tree::s_pMetaTable->RegisterObjectFunctor( "ClearTree", &Tree::ClearTree );
	Tree::s_pMetaTable->RegisterObjectFunctor( "GetFirstSelectItem", &Tree::GetFirstSelectItem );
	Tree::s_pMetaTable->RegisterObjectFunctor( "GetFirstSelectItemString", &Tree::GetFirstSelectItemString );
	Tree::s_pMetaTable->RegisterObjectFunctor( "SetFirstSelectItemString", &Tree::SetFirstSelectItemString );
	Tree::s_pMetaTable->RegisterObjectFunctor( "SetItemSelectByItemID", &Tree::SetItemSelectByItemID );
	Tree::s_pMetaTable->RegisterObjectFunctor( "SetItemToggle", &Tree::SetItemToggle );

}

VOID Window::DestroyMetaTable(VOID)
{
	#define safe_delete_metaTable( x ) if( x ){ delete( x ); x = NULL; }

	// 26
	safe_delete_metaTable( MultiLineEditbox::s_pMetaTable );
	// 25
	safe_delete_metaTable( EditBox::s_pMetaTable );
	// 24 
	safe_delete_metaTable( AnimateWindow::s_pMetaTable );
	// 23 
	safe_delete_metaTable( SoftKeyWindow::s_pMetaTable );
	// 22
	safe_delete_metaTable( CtrlList::s_pMetaTable );
	// 21
	safe_delete_metaTable( InfoList::s_pMetaTable );
	// 20
	safe_delete_metaTable( Worldmap::s_pMetaTable );
	// 19
	safe_delete_metaTable( Scenemap::s_pMetaTable );
	// 18
	safe_delete_metaTable( ChatChannel::s_pMetaTable );
	// 17
	safe_delete_metaTable( ChatHistory::s_pMetaTable );
	// 16
	safe_delete_metaTable( MeshWindow::s_pMetaTable );
	// 15
	safe_delete_metaTable( SuperTooltip::s_pMetaTable );
	// 14
	safe_delete_metaTable( ScrollBar::s_pMetaTable );
	// 13
	safe_delete_metaTable( StaticImage::s_pMetaTable );
	// 12
	safe_delete_metaTable( ComboBox::s_pMetaTable );
	// 11.
	safe_delete_metaTable( CheckButton::s_pMetaTable );
	//10.
	safe_delete_metaTable( ImageListBoxEx::s_pMetaTable );
	//9.
	safe_delete_metaTable( ImageListBox::s_pMetaTable )
	//8.
	//7.

	//6.
	safe_delete_metaTable( ListBox::s_pMetaTable );
	
	//5.
	safe_delete_metaTable( Minimap::s_pMetaTable );

	//4.
	safe_delete_metaTable( ComplexWindow::s_pMetaTable );

	//3. 
	safe_delete_metaTable( PopMenu::s_pMetaTable );
	
	//2.
	safe_delete_metaTable( ActionButton::s_pMetaTable );

	//1.
	safe_delete_metaTable( ProgressBar::s_pMetaTable );

	//0.
	safe_delete_metaTable( Window::s_pMetaTable );
}

INT Window::setProperty(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	if (!(args[2].IsString())) return 0;
	if (!(args[3].IsString())) return 0;

	CEGUI::Window* pWindow = m_pWindow;
	if ( !args[4].IsNil() )
	{
		if ( args[4].IsString() )
		{
			char* p = (char*)args[4].GetString();
			if( strcmp( p, "Parent" ) == 0 )
			{
				pWindow = m_pWindow->getParent();
			}
			else
			{
				pWindow = CEGUI::WindowManager::getSingleton().getWindow( p );
			}
		}
	}

	LPCTSTR szPropertyName = args[2].GetString();
	LPCTSTR szPropertyValue = args[3].GetString();

	STRING strIconFullName,strUtf8_IconName;
	char name[ 128 ];
	sprintf( name, "%s", szPropertyName);

	if(strcmp(name, "ShortImage") == 0)
	{
		szPropertyName = "Image";
		strIconFullName = CUIIconsManager::GetMe()->GetIconFullName( args[3].GetString() );
		CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);
		szPropertyValue = strUtf8_IconName.c_str();
	}
	
	STRING strValue_utf8;
	if(strcmp(name, "Text") == 0)
	{
		std::string strMbcs = args[3].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
		pWindow->setText(strUtf32);
		return 0;
	}
	if(strcmp(name, "Tooltip") == 0)
	{
		std::string strMbcs = args[3].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
		pWindow->setProperty(name, strUtf32.c_str());
		return 0;
	}	
	//// 使ActionButton支持边角文字，可以换颜色
	//if(strcmp(name, "CornerChar") == 0)
	//{	
	//	CEGUI::String value(szPropertyValue);

	//	size_t f1  = value.find_first_of( " ", 0, 1 );
	//	CEGUI::String s1 = value.substr( 0, f1 );
	//	CEGUI::String s2 = value.substr( f1 + 1 );

	//	int iPos = -1;
	//	char* s = (char*)s1.c_str();
	//	if(!value.empty())
	//	{
	//		if(s1 == "TopLeft")
	//		{
	//			iPos = 0;
	//		}
	//		else if(s1 == "TopRight")
	//		{
	//			iPos = 1;
	//		}
	//		else if(s1 == "BotLeft")
	//		{
	//			iPos = 2;
	//		}
	//		else if(s1 == "BotRight")
	//		{
	//			iPos = 3;
	//		}
	//	}

	//	// 解析文字和颜色
	//	std::string strMbcs = (char*)s2.c_str();
	//	CEGUI::String32 strUtf32;
	//	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	//	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setCornerChar(iPos, strUtf32, true);
	//	
	//	return 0;
	//}

	try
	{
		pWindow->setProperty((CEGUI::utf8*)szPropertyName, (CEGUI::utf8*)szPropertyValue);
	}
	catch(...) { }

	return 0;
}

INT  Window::SetEvent( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);

	if (!(args[2].IsString())) return 0;
	if (!(args[3].IsString())) return 0;

	CEGUI::Window* pWindow = m_pWindow;
	if ( !args[4].IsNil() )
	{
		if ( args[4].IsString() )
		{
			char* p = (char*)args[4].GetString();
			if( strcmp( p, "Parent" ) == 0 )
			{
				pWindow = m_pWindow->getParent();
			}
			else
			{
				pWindow = CEGUI::WindowManager::getSingleton().getWindow( p );
			}
		}
	}


	CEGUI::String eventName( args[2].GetString() );
	CEGUI::String functionName( args[3].GetString() );

	// attempt to subscribe property on window
	if (pWindow )
	{
		pWindow->subscribeEvent(eventName, CEGUI::ScriptFunctor(functionName));
	}

	return 1;
}



// 将窗口居中显示，相对与父窗口
INT		Window::CenterWindow( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	CEGUI::Window* pParent = m_pWindow->getParent();
	if( pParent )
	{
		CEGUI::Point pos;
		CEGUI::Rect rect = pParent->getAbsoluteRect();
		pos.d_x = ( rect.d_left + rect.d_right) / 2 - m_pWindow->getAbsoluteRect().getWidth() / 2;
		pos.d_y = ( rect.d_top + rect.d_bottom ) / 2 - m_pWindow->getAbsoluteRect().getHeight() / 2;
		m_pWindow->setPosition( CEGUI::Absolute, pos );
	}
	return 0;
}
// 将窗口居中显示，相对与父窗口
INT		Window::SetWindowCenter( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	if( !args[ 3 ].IsInteger() ) return 0;

	CEGUI::Point pos;
	pos.d_x = args[ 2 ].GetInteger() - m_pWindow->getAbsoluteRect().getWidth() / 2;
	pos.d_y = args[ 3 ].GetInteger() - m_pWindow->getAbsoluteRect().getHeight() / 2;
	m_pWindow->setPosition( CEGUI::Absolute, pos );
	return 0;
}
//设置窗口的随父属性
INT		Window::SetClippedByParent( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	
	INT n = args[ 2 ].GetInteger();

	if(n == 0)
	{
		m_pWindow->setClippedByParent(false);
		m_pWindow->setAlwaysOnTop(true);
	}
	else
		m_pWindow->setClippedByParent(true);

	return 0;
}

//自动设置窗口的位置
INT		Window::AutoMousePosition( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	FLOAT fX = args[2].GetFloat();
	FLOAT fY = args[3].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	if(fX + fWidth + 5.0f> fScreenWidth) 
		fX -= fWidth+5.0f;
	else 
		fX += 5.0f;

	if(fY + fHeight + 5.0f > fScreenHeight) 
		fY -= fHeight+5.0f;
	else
		fY += 5.0f;

	CEGUI::Point tmpPos;
	tmpPos.d_x = fX;
	tmpPos.d_y = fY;

	((CEGUI::Window*)m_pWindow)->setPosition(CEGUI::Absolute, tmpPos);

	return 0;
}

INT Window::getProperty(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	if (!(args[2].IsString())) return 0;

	CEGUI::Window* pWindow = m_pWindow;
	if ( !args[3].IsNil() )
	{
		if ( args[3].IsString() )
		{
			char* p = (char*)args[3].GetString();
			if( strcmp( p, "Parent" ) == 0 )
			{
				pWindow = m_pWindow->getParent();
			}
			else
			{
				pWindow = CEGUI::WindowManager::getSingleton().getWindow( p );
			}
		}
	}

	try
	{
		CEGUI::String strUtf32 = pWindow->getProperty((CEGUI::utf8*)args[2].GetString());
		
		STRING strUtf8;
		CUIStringSystem::utf8_to_mbcs(strUtf32.c_str(), strUtf8);

		pState->PushString(strUtf8.c_str());
	}
	catch(...)
	{
		pState->PushString("");
	}

	return 1;
}

INT	Window::setText(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	INT t = args[2].GetType();
	if( args[ 2 ].IsInteger() ) // 如果是个数字
	{
		char value[ 128 ];
		sprintf( value, "%d", args[ 2 ].GetInteger() );
		std::string strUtf8;
		CUIStringSystem::mbcs_to_utf8(value, strUtf8);

		m_pWindow->setText((CEGUI::String32)(CEGUI::utf8*)(strUtf8.c_str()));
	}
	else
	if ( args[2].IsString() )
	{

		//代码转化
		std::string strMbcs = args[2].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

		m_pWindow->setText(strUtf32);
	}

	return 0;
}

INT Window::Hide(LuaPlus::LuaState* pState)
{
	m_pWindow->setVisible(false);
	if(m_pWindow->testClassName((const CEGUI::utf8*)"Editbox"))
	{
		m_pWindow->releaseInput();
	}
	return 0;
}

INT	Window::getText(LuaPlus::LuaState* pState)
{
	std::string strUtf8 = m_pWindow->getText().c_str();//utf::mbcs_to_utf8(strMbcs);
	std::string strMbcs;
	CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
	pState->PushString( strMbcs.c_str() );
	return 1;
}
INT	Window::setTextOriginal(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	INT t = args[2].GetType();
	if (!(args[2].IsString())) return 0;

	//代码转化
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	m_pWindow->setTextOriginal((CEGUI::String32)(CEGUI::utf8*)(strUtf8.c_str()));

	return 0;
}
INT	Window::setAlpha(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	float iVal = args[2].GetFloat();
	m_pWindow->setAlpha(iVal);

	return 1;
}


INT	Window::setForce( LuaPlus::LuaState* pState)
{
	m_pWindow->activate();

	return 0;
}

// 设置window显示区域 Nick 2007.11.27
// 改变图片的显示区域和纹理坐标 Nick 2008.1.7
INT	Window::setAreaAndTexCoord( LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	if( !args[2].IsInteger() || !args[4].IsInteger() ) 
		return 0;

	if( !args[3].IsNumber() || !args[5].IsNumber() ) 
		return 0;

	m_pWindow->setAreaAndTexCoord( args[2].GetInteger(), 
		args[3].GetFloat(), args[4].GetInteger(), args[5].GetFloat() );

	return 0;
}


INT	Window::CreateChildWindow( LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	if( !args[2].IsString() || !args[3].IsString() ) 
		return 0;

	STRING strWinType = args[2].GetString();
	STRING strWinName = args[3].GetString();

	CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().createWindow( strWinType, strWinName );
	m_pWindow->addChildWindow( pWindow );

	return 0;
}

INT	Window::RemoveChildWindow( LuaPlus::LuaState* pState)
{
	
	LuaStack args(pState);

	if( !args[2].IsString() ) 
		return 0;


	STRING strWinName = args[2].GetString();
	CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow( strWinName );
	if( pWindow )
	{
		m_pWindow->removeChildWindow( pWindow );
	}

	return 0;
}


INT	Window::DebugWindowInfo( LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	if( !args[2].IsString() ) 
		return 0;

	char* p = (char*)args[2].GetString();
	
	return 0;
}



INT Window::GetPosition( LuaPlus::LuaState* pState )
{
	pState->PushNumber( m_pWindow->getPosition(CEGUI::Absolute).d_x);
	pState->PushNumber( m_pWindow->getPosition(CEGUI::Absolute).d_y);
	return 2;
}


INT		Window::SetPosition(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() )
	{
		return 0;
	}
	if( !args[3 ].IsInteger() )
	{
		return 0;
	}
	if( !args[ 4 ].IsInteger() )
	{
		return 0;
	}
	CEGUI::Point pos;
	pos.d_x = args[ 3 ].GetInteger();
	pos.d_y = args[ 4 ].GetInteger();
 
	CEGUI::MetricsMode  m;
	if (args[ 2 ].GetInteger() == 0 )
	{
		m = CEGUI:: Relative;

	}else if (args[ 2 ].GetInteger() ==1)
	{
		m =  CEGUI::Absolute;
	}else
	{	
		m = CEGUI::Inherited ;
	}

	m_pWindow->setPosition(m ,pos);
		return 0;
}


INT	Window::SetToolTip( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !( args[ 2 ].IsString() ) ) return 0;
	std::string strMbcs = args[2].GetString();
	CEGUI::String32  strLast;
	CUIStringSystem::GetMe()->ParserString_Runtime( strMbcs, strLast );

	m_pWindow->setProperty("Tooltip", strLast.c_str() );
	
	return 0;
}

INT	Window::TransText(LuaPlus::LuaState* pState)
{
	CEGUI::String32 str = m_pWindow->getTextOriginal();
	if(!str.empty())
	{
		STRING strMBCS_ToTrans;
		CUIStringSystem::utf8_to_mbcs(str.c_str(), strMBCS_ToTrans);
		// 经过special转化的string，已经是utf32格式。
		CEGUI::String32 strMBCS_Transed_Done;
		
		CUIStringSystem::GetMe()->ParserString_Runtime(strMBCS_ToTrans, strMBCS_Transed_Done);

		m_pWindow->setText(strMBCS_Transed_Done);
	}
	return 0;
}

INT Window::CaptureInput( LuaPlus::LuaState* pState)
{
	m_pWindow->activate();
	m_pWindow->captureInput();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* SuperProgressBar::s_pMetaTable = NULL;
LuaPlus::LuaObject* ProgressBar::s_pMetaTable = NULL;
INT ProgressBar::SetProgress(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsNumber())) return 0;
//	if (!(args[3].IsNumber())) return 0;

	FLOAT fNow = (FLOAT)args[2].GetNumber();
	FLOAT fMax = (FLOAT)args[3].GetNumber();

	if (fMax == -1)
	((CEGUI::ProgressBar*)m_pWindow)->setProgress((FLOAT)fNow);
	else
	((CEGUI::ProgressBar*)m_pWindow)->setProgress((FLOAT)fNow/fMax);

	return 0;
}

//==============================================================================
LuaPlus::LuaObject* ActionButton::s_pMetaTable = NULL;
INT ActionButton::Lua_SetActionItem(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	INT idItem = (INT)args[2].GetInteger();

	Update(idItem);
	return 0;
}

INT ActionButton::SetActionSystemKeyState(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger()))
		return 0;

	if (!(args[3].IsInteger()))
		return 0;

	INT keyState = (INT)args[2].GetInteger();
	BOOL isFKey = (BOOL)args[3].GetInteger();

	g_pActionSys->SetActionSystemKeyState( keyState, isFKey );

	return 0;
}

INT ActionButton::GetActionItemId(LuaPlus::LuaState* pState)
{

 
	if( !m_pActionItem )
	{
		pState->PushInteger( -1 );
		return 1;
	}

	if( m_pActionItem == NULL )
	{
		pState->PushInteger( -1 );
		return 1;
	}


	INT	id = m_pActionItem->GetID();
	pState->PushInteger( id );

	return 1;
}

INT ActionButton::GetActionItemDefineId(LuaPlus::LuaState* pState)
{
	if( !m_pActionItem )
	{
		pState->PushInteger( -1 );
		return 1;
	}

	INT	id = m_pActionItem->GetDefineID();
	pState->PushInteger( id );

	return 1;
}

INT ActionButton::Lua_DoAction(LuaPlus::LuaState* pState)
{
	if(m_pActionItem)
		m_pActionItem->DoAction();

	return 0;
}

INT ActionButton::Lua_DoSubAction(LuaPlus::LuaState* pState)
{
	if(m_pActionItem)
	{
		m_pActionItem->DoSubAction();
		pState->PushInteger(1);
	}
	pState->PushInteger(0);
	return 1;
}

INT ActionButton::Lua_SetPushed(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	BOOL bCheck = args[2].GetInteger() == 1;

	SetCheck(bCheck);

	return 0;
}

INT	ActionButton::Lua_SetFlash(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	BOOL bFlash = args[2].GetInteger();

	EnterFlash(bFlash);

	return 0;
}

INT	ActionButton::setActionItemIconName(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	STRING iconName(args[2].GetString());
	m_pActionItem->SetIconName(iconName);

	return 0;
}


INT	ActionButton::setLogicItemData(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;

	int id = args[2].GetInteger();

	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setLogicItemData( (void*)(INT_PTR)id );

	return 0;
}

INT	ActionButton::setNormalImage(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	m_pWindow->show();
	m_pWindow->setProperty("Empty", "False");
	STRING strIconName = CUISystem::GetMe()->GetIconManager()->GetIconFullName(args[2].GetString());
	if(!strIconName.empty())
	{
		m_pWindow->setProperty("NormalImage", strIconName.c_str());
	}

	return 0;
}

//锁定
VOID ActionButton::Locked(VOID)
{
	m_pWindow->setProperty("Locked", "False");
	m_pWindow->requestRedraw();
}
//解除锁定
VOID ActionButton::Unlock(VOID)
{
	m_pWindow->setProperty("Locked", "False");
	m_pWindow->requestRedraw();
}

VOID ActionButton::BeDestroyed(VOID)
{
	m_pWindow->setProperty("Empty", "True");
	//m_pWindow->setProperty("Tooltip", (CEGUI::utf8*)"");
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setLogicItemData((void*)(INT_PTR)(-1));
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();

	//隐藏Tooltip
	if(g_pActionSys->GetTooltipsFocus() && g_pActionSys->GetTooltipsFocus() == m_pActionItem)
	{
		AxTrace(0, 0, "***CleanSupertooltips");
		g_pEventSys->PushEvent(GE_SUPERTOOLTIP, "0");
	}

	m_pWindow->requestRedraw();
	m_pActionItem = NULL;
}

VOID ActionButton::Update(INT idItem)
{
	//取得ActionItem
	tActionItem* pActionItem = g_pActionSys->GetAction(idItem);

	//断开和原来的Action的联系
	if(m_pActionItem && pActionItem != m_pActionItem)
	{
		m_pActionItem->RemoveRefrence(this);
	}

	if(NULL == pActionItem)
	{
		//如果逻辑项为空, 清空显示
		BeDestroyed();
		m_pWindow->setProperty("ShowTooltip","True");
		return;
	}
	//不显示默认tooltip
	m_pWindow->setProperty("ShowTooltip","False");
	//设置有效无效标记
	if(pActionItem->IsEnabled())
	{
		Bright();
	}
	else
	{
		Gloom();
	}

	//设置新的Action
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setLogicItemData((void*)(INT_PTR)pActionItem->GetID());

	m_pActionItem = pActionItem;
	LPCTSTR DragName = ((CEGUI::FalagardActionButton*)m_pWindow)->getDragAcceptName();

	if(DragName[0] == 'M' || DragName[0] == 'F')
	{
		pActionItem->AddReference(this,TRUE);
	}
	else
	{
		pActionItem->AddReference(this,FALSE);
	}

	const STRING strIconName = 
		CUISystem::GetMe()->GetIconManager()->GetIconFullName(m_pActionItem->GetIconName());

	m_pWindow->show();
	m_pWindow->setProperty("Empty", "False");
	if(!strIconName.empty())
	{
		m_pWindow->setProperty("NormalImage", strIconName);
	}
}

//屏蔽
VOID	ActionButton::Disable(VOID)
{
	m_pWindow->setProperty("Disabled", "True");
	m_pWindow->requestRedraw();
}

//开启
VOID	ActionButton::Enable(VOID)
{
	m_pWindow->setProperty("Disabled", "False");
	m_pWindow->requestRedraw();
}

//变灰
VOID	ActionButton::Gloom(VOID)
{
	m_pWindow->setProperty("Gloom", "True");
	m_pWindow->requestRedraw();
}

//变亮
VOID	ActionButton::Bright(VOID)
{
	m_pWindow->setProperty("Gloom", "False");
	m_pWindow->requestRedraw();
}

INT	ActionButton::Lua_Gloom(LuaPlus::LuaState* pState)
{
	Gloom();

	return 0;
}

INT	ActionButton::Lua_Bright(LuaPlus::LuaState* pState)
{
	Bright();

	return 0;
}

VOID ActionButton::SetCheck(BOOL bCheck)
{
	m_pWindow->setProperty("Checked", bCheck ? "True" : "False");

	m_pWindow->requestRedraw();
}

//按钮设置成default的状态
VOID ActionButton::SetDefault(BOOL bDefault)
{
	m_pWindow->setProperty("Default", bDefault ? "True" : "False");

	m_pWindow->requestRedraw();
}

VOID ActionButton::SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar)
{
	CHAR szTemp[MAX_PATH];

	switch(pos)
	{
	case ANP_TOPLEFT:
		{
			_snprintf(szTemp, MAX_PATH, "TopLeft %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_TOPRIGHT:
		{
			_snprintf(szTemp, MAX_PATH, "TopRight %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_BOTLEFT:
		{
			_snprintf(szTemp, MAX_PATH, "BotLeft %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_BOTRIGHT:
		{
			_snprintf(szTemp, MAX_PATH, "BotRight %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	}
}

VOID ActionButton::EnterCoolDown(INT nTime, FLOAT fPercent)
{
	if(nTime <= 0)
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();
	}
	else
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setAnimate(
			"CoolDown", nTime, fPercent);
	}
}

VOID ActionButton::EnterFlash(BOOL bFlash)
{
	if(bFlash)
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setAnimate(
			"Flash", 1000, 0.0);
	}
	else
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();
	}
}


//==============================================================================
LuaPlus::LuaObject* PopMenu::s_pMetaTable = NULL;
INT PopMenu::OpenPopMenu(LuaPlus::LuaState* pState)
{
	((CEGUI::PopupMenu*)m_pWindow)->openPopupMenu();
	return 0;
}

INT PopMenu::ClosePopMenu(LuaPlus::LuaState* pState)
{
	((CEGUI::PopupMenu*)m_pWindow)->closePopupMenu();
	return 0;
}

// 设置位置
INT	PopMenu::SetPopMenuPos(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	FLOAT fX = args[2].GetFloat();
	FLOAT fY = args[3].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	if(fX + fWidth > fScreenWidth) fX -= fWidth;
	if(fY + fHeight > fScreenHeight) fY -= fHeight;

	((CEGUI::PopupMenu*)m_pWindow)->setXPosition(CEGUI::Relative, fX/fScreenWidth);
	((CEGUI::PopupMenu*)m_pWindow)->setYPosition(CEGUI::Relative, fY/fScreenHeight);

	m_pWindow->requestRedraw();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* ComplexWindow::s_pMetaTable = NULL;
INT ComplexWindow::AddTextElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	//文字转化
	std::string strMbcs = args[2].GetString();

//	std::string strUtf8 = utf::mbcs_to_utf8(strMbcs);

	CEGUI::String32 strUtf32;
	
	CEGUI::FontBase* theFont = NULL;
	//创建新的文字节
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	if(args[3].IsString())
	{
		std::string strMbcsFont = args[3].GetString();
		CEGUI::String32 strUtf32Font;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsFont, strUtf32Font);
		
		CEGUI::FontManager* pFontManager = CEGUI::FontManager::getSingletonPtr();
		theFont = pFontManager->getFont(strUtf32Font.c_str());

	}

	INT	Typeset = 4;

	std::string strMbcsExtra1;	// = args[3].GetString();

	//马匪首领@4
	const char* strTemp = args[2].GetString();
	const char* strDelim1 = strchr(strTemp,'@');
	
	if(strDelim1)
	{
		strMbcs.assign(strDelim1+2);
		strMbcsExtra1.assign(strDelim1+1,strDelim1+2);
		Typeset = atoi(&strMbcsExtra1.at(0));
	}

	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	
	pComplexWindow->AddChildElement_Text(strUtf32,theFont,(CEGUI::TextFormatting)Typeset);
	

	return 0;
}

INT ComplexWindow::AddChatBoardElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	//文字转化
	std::string strMbcs = args[2].GetString();

	//	std::string strUtf8 = utf::mbcs_to_utf8(strMbcs);

	CEGUI::String32 strUtf32;

	CEGUI::FontBase* theFont = NULL;
	//创建新的文字节
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	if(args[3].IsString())
	{
		std::string strMbcsFont = args[3].GetString();
		CEGUI::String32 strUtf32Font;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsFont, strUtf32Font);

		CEGUI::FontManager* pFontManager = CEGUI::FontManager::getSingletonPtr();
		theFont = pFontManager->getFont(strUtf32Font.c_str());

	}

	INT	Typeset = 4;

	std::string strMbcsExtra1;	// = args[3].GetString();

	//马匪首领@4
	const char* strTemp = args[2].GetString();
	const char* strDelim1 = strchr(strTemp,'@');

	if(strDelim1)
	{
		strMbcs.assign(strDelim1+2);
		strMbcsExtra1.assign(strDelim1+1,strDelim1+2);
		Typeset = atoi(&strMbcsExtra1.at(0));
	}

	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	pComplexWindow->AddChildElement_ChatBoard(strUtf32, NULL);

	return 0;
}

INT ComplexWindow::AddOptionElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	//文字转化
	std::string strMbcs;// = args[2].GetString();
	std::string strMbcsExtra1;// = args[3].GetString();
	std::string strMbcsExtra2;// = args[4].GetString();
	std::string strMbcsExtra3;// = args[5].GetString();
	//马匪首领#21107,-1$-1
	const char* strTemp = args[2].GetString();
	const char* strDelim1 = strchr(strTemp,'#');
	const char* strDelim2 = strchr(strTemp,',');
	const char* strDelim3 = strchr(strTemp,'$');

	if( (strDelim1 && (!strDelim2 || !strDelim3) ) ||
		(strDelim2 && (!strDelim1 || !strDelim3) ) ||
		(strDelim3 && (!strDelim1 || !strDelim2) ) )
	{
		KLAssert(strDelim1 && strDelim2 && strDelim3);
	}

	if(strDelim1 && strDelim2 && strDelim3)
	{
		strMbcs.assign(strTemp,strDelim1-strTemp);
		strMbcsExtra1.assign(strDelim1+1,strDelim2-1-strDelim1);
		strMbcsExtra2.assign(strDelim2+1,strDelim3-1-strDelim2);
		strMbcsExtra3.assign(strDelim3+1);
	}
	else
	{
		strMbcs.assign(strTemp);
		strMbcsExtra1.assign("-1");
		strMbcsExtra2.assign("-1");
		strMbcsExtra3.assign("-1");
	}

	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	CEGUI::String32 strExtra1;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra1, strExtra1);
	
	CEGUI::String32 strExtra2;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra2, strExtra2);

	CEGUI::String32 strExtra3;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra3, strExtra3);

	//创建新的选项节
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->AddChildElement_Option(strUtf32,strExtra1,strExtra2,strExtra3);

	return 0;
}
INT ComplexWindow::AddItemElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	if (!(args[3].IsInteger())) return 0;
	if (!(args[4].IsInteger())) return 0;

	INT  idItem	= args[2].GetInteger();
	INT	 nNum	= args[3].GetInteger();
	BOOL bRadio		= (args[4].GetInteger() == 1);
	BOOL bReArrange = FALSE;
	if(args[5].IsInteger())
		bReArrange = (args[5].GetInteger() == 1);

	tObject_Item* pItem = g_pObjectSystem->FindItem(idItem);

	STRING strIconFullName = CUIIconsManager::GetMe()->GetIconFullName(pItem->GetIconName());
	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);

	STRING strUtf8_Desc;
	CUIStringSystem::mbcs_to_utf8(pItem->GetName(), strUtf8_Desc);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()), (CEGUI::String32)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio==TRUE, bReArrange==TRUE);
//	pComplexWindow->AddChildElement_Item( idItem, (CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio==TRUE, bReArrange==TRUE);


	return 0;
}

INT ComplexWindow::AddActionElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	if (!(args[3].IsInteger())) return 0;
	if (!(args[4].IsInteger())) return 0;

	INT  idItem	= args[2].GetInteger();
	INT	 nNum	= args[3].GetInteger();
	BOOL bRadio		= (args[4].GetInteger() == 1);
	BOOL bReArrange = FALSE;
	if(args[5].IsInteger())
		bReArrange = (args[5].GetInteger() == 1);

	tActionItem* pActionItem = g_pActionSys->GetAction(idItem);

	if(NULL == pActionItem)
		return 0;

	const STRING strIconFullName = 
		CUISystem::GetMe()->GetIconManager()->GetIconFullName(pActionItem->GetIconName());

	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	CEGUI::IFalagardActionButton* pActionButton = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pComplexWindow->AddChildElement_Action( strUtf8_IconName, idItem, bRadio==TRUE, bReArrange==TRUE);

	if(pActionItem->GetDefineID() != 20005020)//随机奖励物品不显示supertooltip
	{
	//MouseEnter
		pActionButton->subscribeMouseEnterEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseEnter, CUISystem::GetMe()));
	//MouseLeave
		pActionButton->subscribeMouseLeaveEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseLeave, CUISystem::GetMe()));
	//ParentHidden
		((CEGUI::Window*)(CEGUI::FalagardActionButton*)pActionButton)->
			subscribeEvent((CEGUI::utf8*)"ParentHidden", CEGUI::Event::Subscriber(&CUISystem::handleActionButtonParentHidden, CUISystem::GetMe()));
	}
	return 0;
}

//struct SBuffImpact;
INT ComplexWindow::AddImpactElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;
//	if (!(args[3].IsInteger())) return 0;
//	if (!(args[4].IsInteger())) return 0;

//	args[2].GetString()  idImpact	= args[2].GetString();
//	INT		nNum	= args[3].GetInteger();
//	BOOL bRadio		= (args[4].GetInteger() == 1);

//	tObject_Item* pItem = g_pObjectSystem->FindItem(idItem);
	//SBuffImpact *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );

	STRING strIconFullName = CUIIconsManager::GetMe()->GetIconFullName( args[2].GetString() );
	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName, strUtf8_IconName);

	STRING strUtf8_Desc;
	CUIStringSystem::mbcs_to_utf8("", strUtf8_Desc);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
//	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()), (CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio);
	//	改了使用方法
	//	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()),(CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()),0,0);
	return 0;
}

INT ComplexWindow::AddMoneyElement(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;

	pComplexWindow->AddChildElement_Money(atoi(args[2].GetString()));

	return 0;
}

INT ComplexWindow::CleanAllElement(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;

	pComplexWindow->ClearAllElement();

	return 0;
}

//更改字体的颜色
INT ComplexWindow::SetTextColour(LuaPlus::LuaState* pState)
{
	return 0;
}
INT ComplexWindow::PageEnd(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->PageEnd();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* Minimap::s_pMetaTable = NULL;

//#define MAP_FLAG_NUM	14

//LPCTSTR FLAG_CLASS_NAME[] = {"Animate","ExpNPC","Teamate","OtherPlayer","ExpObj",};
INT Minimap::UpdateFlag( LuaPlus::LuaState* pState )
{

	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
	if( m_pWindow->isVisible() )
	{
		// 更新所有的标志位置
		pWindow->Clear();

		for( INT i = 0; i < MINIMAP_TYPE_NUM; i ++ )	
		{
			std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( i );

			if ( listObj == NULL)
			{
				continue;
			}
			
			// 得到了当前类型所有的物体的链表
			std::list< MAP_POS_DEFINE >::iterator it;
			for( it = listObj->begin(); it != listObj->end(); it ++ )
			{
				//std::string strUtf8;
				//std::string strMbcs = (*it).name;

				// 如果是队友，需要单独处理
				if( i == MINIMAP_TYPE_TEAM ) 
				{
					// 在同一场景才会添加
					if( (*it).dwSceneID != g_pWorldSystem->GetActiveSceneID() ) 
						continue;
				}

				pWindow->setMinimapData( i, (*it).pos.x, (*it).pos.z,  (CEGUI::String32)(CEGUI::utf8*)(*it).utf8name );
			}
		}

	}

	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	int		 nDir = g_pWorldSystem->GetMyselfDir( );
	pWindow->setMyselfPos( CEGUI::Point( MyselfPos.x, MyselfPos.z ) );
	pWindow->setMyselfDir( nDir );


	return 0;
}

//==============================================================================
// ListBox
LuaPlus::LuaObject* ListBox::s_pMetaTable = NULL;
INT ListBox::AddItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if (!(args[3].IsInteger()))
		return 0;

	//Text
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	//TextIndex
	INT nIndex = args[3].GetInteger();

	//TextColor
	CEGUI::colour colorText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	if(args[4].IsString())
	{
		colorText = CEGUI::PropertyHelper::stringToColour((CEGUI::utf8*)args[4].GetString());
	}
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( strUtf32,  nIndex);
	pItem->setTextColours(colorText);
	CEGUI::colour colorSelText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	if(args[5].IsString())
	{
		colorSelText = CEGUI::PropertyHelper::stringToColour((CEGUI::utf8*)args[5].GetString());
	}
	pItem->setSelectionColours(colorSelText);

	
	if( args[ 6 ].IsInteger() )
		pItem->setHorzFomate( CEGUI::TextFormatting( args[ 6 ].GetInteger() ) );

	if ( args[7].IsString() && args[8].IsString() )
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)args[7].GetString() , (CEGUI::utf8*)args[8].GetString() );
	}
	else
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)"wxsj_questlog", (CEGUI::utf8*)"questlog_choice_back");
	}

	((CEGUI::Listbox*)m_pWindow)->addItem( pItem );

	return 0;
}

// 设置ListboxItem的索引(支持Listbox键盘快捷键选中ListboxItem)
INT ListBox::SetTagText( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( !( args[ 3 ].IsString() ) )
		return 0;

	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	pItem->setTagText( args[ 3 ].GetString() );

	return 0;
}

INT ListBox::SetRefIndex( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;

	((CEGUI::Listbox*)m_pWindow)->setRefIndex( args[ 2 ].GetInteger() );

	return 0;
}

INT ListBox::SetItemTooltip( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( !( args[ 3 ].IsString() ) )
		return 0;

	((CEGUI::Listbox*)m_pWindow)->setItemTooltipsEnabled( true );
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	
	pItem->setTooltipText( args[ 3 ].GetString() );

	return 0;
}

INT ListBox::SetItemTooltipState( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !(args[2].IsInteger()) )
		return 0;

	bool set = (args[2].GetInteger() == 0) ? false : true;

	if( m_pWindow )
		((CEGUI::Listbox*)m_pWindow)->setItemTooltipsEnabled( set );
	
	return 0;
}

INT ListBox::SetItemText( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( !( args[ 3 ].IsString() ) )
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	std::string strMbcs = args[3].GetString();
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	pItem->setText( strUtf32 );
	((CEGUI::Listbox*)m_pWindow)->requestRedraw();
	return 0;
}

INT ListBox::DelItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	((CEGUI::Listbox*)m_pWindow)->removeItem( pItem );
	return 0;
}

INT ListBox::GetItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( args[ 2 ].GetInteger() < 0 )
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() - 1 );
	std::string strMbcs;
	CUIStringSystem::utf8_to_mbcs(pItem->getText().c_str(), strMbcs);
	pState->PushString( strMbcs.c_str() );
	pState->PushInteger( pItem->getID() );
	return 2;
}

INT ListBox::GetMouseMoveItem( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getMouseMoveSelectedItem();
	if( !pItem )
		pState->PushInteger( -1 );
	else
		pState->PushInteger( pItem->getID() );

	return 1;
}


INT ListBox::ClearListBox( LuaPlus::LuaState* pState )
{
	((CEGUI::Listbox*)m_pWindow)->resetList();
	return 0;
}

INT	ListBox::GetFirstSelectItem( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxTextItem* pItem = (CEGUI::ListboxTextItem*)((CEGUI::Listbox*)m_pWindow)->getFirstSelectedItem();
	if(!pItem) 
		pState->PushInteger(-1);
	else
		pState->PushInteger(pItem->getID());

	return 1;
}

INT	ListBox::GetFirstSelectItemString( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxTextItem* pItem = (CEGUI::ListboxTextItem*)((CEGUI::Listbox*)m_pWindow)->getFirstSelectedItem();
	if( pItem != NULL )
	{
		std::string strUtf8 = pItem->getText().c_str();
		std::string strMbcs;
		CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
		pState->PushString( strMbcs.c_str() );
	}
	else
		pState->PushString("");

	return 1;
}

INT	ListBox::SetFirstSelectItemString( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;

	CEGUI::ListboxTextItem* pItem = (CEGUI::ListboxTextItem*)((CEGUI::Listbox*)m_pWindow)->getFirstSelectedItem();
	if( pItem != NULL )
	{
		std::string strMbcs = args[2].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
		pItem->setText(strUtf32);
		((CEGUI::Listbox*)m_pWindow)->requestRedraw();
	}

	return 0;
}

INT	ListBox::ClearAllSelections( LuaPlus::LuaState* pState )
{
	((CEGUI::Listbox*)m_pWindow)->clearAllSelections();

	return 1;
}

INT	ListBox::SetItemSelect( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	
	INT nItemIndex = args[2].GetInteger();

	((CEGUI::Listbox*)m_pWindow)->setItemSelectState(nItemIndex, true);
	return 0;
}

INT	ListBox::SetItemSelectByItemID( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;

	INT nItemID = args[2].GetInteger();

	((CEGUI::Listbox*)m_pWindow)->setItemSelectStateByItemID(nItemID, true);
	return 0;
}

//==============================================================================
// ImageListBox
LuaPlus::LuaObject* ImageListBox::s_pMetaTable = NULL;
INT ImageListBox::AddItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;

	CEGUI::String strText;
	char value[ 128 ];
	sprintf( value, "id=%d text=%s", args[ 3 ].GetInteger(), args[2].GetString() );
	m_pWindow->setProperty( "AddItem", value );
	return 0;
}
//==============================================================================
// ImageListBoxEx
LuaPlus::LuaObject* ImageListBoxEx::s_pMetaTable = NULL;
INT ImageListBoxEx::AddItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if( !( args[ 3 ].IsInteger() ) )
		return 0;
	if( !( args[ 4 ].IsInteger() ) )
		return 0;

	CEGUI::String strText;
	char value[ 128 ];
	
	sprintf( value, "id=%d state=%d text=%s", args[ 3 ].GetInteger(), args[ 4 ].GetInteger(), args[2].GetString() );
	std::string strMbcs = value;
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);
	m_pWindow->setProperty( "AddItem", (CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()) );
	return 0;
}
//==============================================================================
//CheckButton
LuaPlus::LuaObject* CheckButton::s_pMetaTable = NULL;
INT CheckButton::SetCheck( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	
	if( args[ 2 ].GetInteger() == 1 )

		m_pWindow->setProperty( "Selected", "True" );
	else
		m_pWindow->setProperty( "Selected", "False" );

	return 0;
}
INT CheckButton::GetCheck( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if(CEGUI::PropertyHelper::stringToBool(  m_pWindow->getProperty( "Selected" ) ))
		pState->PushInteger(TRUE);
	else
		pState->PushInteger(FALSE);

//	pState->PushBoolean( CEGUI::PropertyHelper::stringToBool(  m_pWindow->getProperty( "Selected" ) ) );
	return 1;
}

LuaPlus::LuaObject* ComboBox::s_pMetaTable = NULL;
INT ComboBox::ComboBoxAddItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( (CEGUI::String32)(CEGUI::utf8*)(strUtf8.c_str()), args[ 3 ].GetInteger() );

	pItem->setSelectionColours(CEGUI::colour(1.0f, 0.0f, 0.0f));
	if ( args[3].IsString() && args[4].IsString() )
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)args[3].GetString() , (CEGUI::utf8*)args[4].GetString() );
	}
	else
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)"wxsj_questlog", (CEGUI::utf8*)"questlog_choice_back");
	}
	((CEGUI::Combobox*)m_pWindow)->addItem( pItem );

	return 0;
}
INT		ComboBox::ResetList( LuaPlus::LuaState* pState )
{
	((CEGUI::Combobox*)m_pWindow)->resetList();
	return 0;
}
INT		ComboBox::GetCurrentSelect( LuaPlus::LuaState* pState )
{
	if( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem() )
	{
		pState->PushString( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem()->getText().c_str() );
		pState->PushInteger( int( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem()->getID() ) );
	}
	else
	{
		pState->PushString( "-1" );
		pState->PushInteger( -1 );
	}
		
	return 2;
}
INT		ComboBox::SetCurrentSelect( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);	
	CEGUI::ListboxItem* pItem = ((CEGUI::Combobox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	((CEGUI::Combobox*)m_pWindow)->setItemSelectState( args[ 2 ].GetInteger() , true );
	((CEGUI::Combobox*)m_pWindow)->setText( pItem->getText() );
	return 0;

}
INT		ComboBox::SetItemText( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);	
	if( !(args[2].IsInteger()) )return 0;
	if( !(args[3].IsString()) )return 0;

	CEGUI::ListboxItem* pItem = ((CEGUI::Combobox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	CEGUI::String32 strMbcs;
	CUIStringSystem::GetMe()->ParserString_Runtime( args[3].GetString(), strMbcs );

	pItem->setText( strMbcs );

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
LuaPlus::LuaObject* StaticImage::s_pMetaTable = NULL;
INT StaticImage::SetImageColor( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	CEGUI::colour color = CEGUI::PropertyHelper::stringToColour( args[ 2 ].GetString() );
	CEGUI::ColourRect colorRect( color );
	m_pWindow->setProperty( "ImageColours", CEGUI::PropertyHelper::colourRectToString( colorRect ) );

	return 0;
}
INT StaticImage::SetImage( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if( args[3].IsString() ) // 说明这个图片是全路径
	{
		char strImageName[128];
		_snprintf("set:%s image:%s", 128, args[2].GetString(), args[3].GetString());
		m_pWindow->setProperty("Image", strImageName );
	}
	else// 说明是icon
	{
		STRING strIconName;
		try
		{
			strIconName = CUISystem::GetMe()->GetIconManager()->GetIconFullName(args[2].GetString());
			m_pWindow->setProperty("Image", (CEGUI::utf8*)strIconName.c_str());
		}
		catch(...)
		{
			CHAR szDebug[1024] = {0};
			_snprintf(szDebug, 1024, 
				"传说中的LuaError\n"
				"arg2=%s\n"
				"Full=%s\n"
				"pWindow=%08X\n"
				"this=%08X",
				args[2].GetString(), strIconName.c_str(), (DWORD)(DWORD_PTR)m_pWindow, (DWORD)(DWORD_PTR)this);

			MessageBox(g_hMainWnd, szDebug, "LUA", MB_OK|MB_ICONINFORMATION);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
// ScrollBar
LuaPlus::LuaObject* ScrollBar::s_pMetaTable = NULL;
INT ScrollBar::GetPosition( LuaPlus::LuaState* pState )
{
	pState->PushNumber( ((CEGUI::Scrollbar*)m_pWindow)->getScrollPosition() );
	return 1;
}

INT ScrollBar::SetPosition( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsNumber())) 
		return 0;
	((CEGUI::Scrollbar*)m_pWindow)->setScrollPosition((float)args[2].GetNumber());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// SuperTooltip
LuaPlus::LuaObject* SuperTooltip::s_pMetaTable = NULL;

INT SuperTooltip::PositionSelf( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	//强制SuperTooltip重新计算组件大小
	((CEGUI::IFalagardSuperTooltip*)(CEGUI::FalagardSuperTooltip*)m_pWindow)->resizeSelf();

	float btn_left		= args[2].GetFloat();
	float btn_top		= args[3].GetFloat();
	float btn_right		= args[4].GetFloat();
	float btn_bottom	= args[5].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	float fX, fY;

	if(btn_right + fWidth + 5.0f> fScreenWidth) 
		fX = btn_left-fWidth;
	else 
		fX = btn_right+0.5f;

	if(btn_bottom + fHeight + 5.0f > fScreenHeight) 
		fY = btn_top-fHeight;
	else
		fY = btn_bottom+0.5f;

	if(fX<0.f) fX=0.f;
	if(fY<0.f) fY=0.f;

	((CEGUI::PopupMenu*)m_pWindow)->setXPosition(CEGUI::Absolute, fX);
	((CEGUI::PopupMenu*)m_pWindow)->setYPosition(CEGUI::Absolute, fY);

	pState->PushNumber(fX);
	pState->PushNumber(fY);

	return 2;

}

INT		SuperTooltip::modifyWidth(LuaPlus::LuaState* pState)
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////
// MeshWindow
LuaPlus::LuaObject* MeshWindow::s_pMetaTable = NULL;
INT MeshWindow::SetFakeObject( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[2].IsString())
		return 0;

	int nWinWidth  = (int)(m_pWindow->getAbsoluteWidth());
	int nWinHeight = (int)(m_pWindow->getAbsoluteHeight());

	int nTemp = 1; 
	
	while(std::pow(2.f, nTemp) < nWinWidth) nTemp++;
	nWinWidth = std::pow(2.f, nTemp);

	nTemp=1;
	while(std::pow(2.f, nTemp) < nWinHeight) nTemp++;
	nWinHeight = std::pow(2.f, nTemp);

	//通知FakeObject系统
    g_pFakeSystem->OnUIHook( m_pWindow->getName().c_str(), args[2].GetString(), nWinWidth, nWinHeight,
        m_pWindow->getProperty((CEGUI::utf8*)"BackgroundName").c_str() );
	//通知UI
	m_pWindow->setProperty((CEGUI::utf8*)"FakeObject", (CEGUI::utf8*)args[2].GetString());

	return 0;
}

INT MeshWindow::RotateBegin(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[2].IsNumber())
		return 0;

	FLOAT fAngle = args[2].GetFloat();

	//通知FakeObject系统
	g_pFakeSystem->RotateObj_Begin(m_pWindow->getProperty((CEGUI::utf8*)"FakeObject").c_str(), fAngle);

	return 0;
}

INT MeshWindow::RotateEnd(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);

	//通知FakeObject系统
	g_pFakeSystem->RotateObj_End(m_pWindow->getProperty((CEGUI::utf8*)"FakeObject").c_str());

	return 0;
}

INT MeshWindow::GetObjName(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	
	// 返回对象名字
	//pState->PushString( g_pFakeSystem->GetObjName( m_pWindow->getName().c_str() ).c_str() );
	pState->PushString( "CreateRole_Man");

	return 0;
}

INT MeshWindow::SetUnFakeObject(LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[2].IsString())
		return 0;

	//通知FakeObject系统
	g_pFakeSystem->OnUIUnHook( args[2].GetString() );

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// ChatHistory
LuaPlus::LuaObject* ChatHistory::s_pMetaTable = NULL;
INT ChatHistory::InsertChatString(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[2].IsString())
		return 0;

	//代码转化
	std::string strMbcs = args[2].GetString();
	std::string::size_type sB = strMbcs.find( "#aB" );
	std::string::size_type sE = strMbcs.find( "#aE" );
	int TalkerLength = sE - sB -3 ;
	TalkerLength = (TalkerLength-2)/2 + 2;
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	const char* a = strUtf32.c_str();
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->insertChatString(-1,strUtf32, TalkerLength);

	return 0;
}

INT ChatHistory::RemoveAllChatString(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->removeAllChatStrings();
	return 0;
}

INT	ChatHistory::ScrollToEnd(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->scrollToEnd();
	return 0;
}

INT ChatHistory::ExtendClearRegion(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->extendClearRegion();
	return 0;
}

//INT ChatHistory::OnMouseButtonUp(LuaPlus::LuaState* pState)
//{
//	((CEGUI::FalagardChatHistory*)m_pWindow)->onMouseButtonUp();
//	return 0;
//}

////////////////////////////////////////////////////////////////////////////////////
// ChatChannel
LuaPlus::LuaObject* ChatChannel::s_pMetaTable = NULL;
INT ChatChannel::ClearAllChannel(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatChannel*)(CEGUI::FalagardChatChannel*)m_pWindow)->clearAllChannel();

	return 0;
}

INT ChatChannel::AddChannel(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[2].IsString() || !args[3].IsString() || !args[4].IsString())
		return 0;

	//代码转化
	std::string strMbcs_ChannelType =  args[2].GetString();
	CEGUI::String32 strUtf32_ChannelType;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_ChannelType, strUtf32_ChannelType);

	std::string strMbcs_IconName =  args[3].GetString();
	CEGUI::String32 strUtf32_IconName;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_IconName, strUtf32_IconName);

	std::string strMbcs_Name =  args[4].GetString();
	CEGUI::String32 strUtf32_Name;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_Name, strUtf32_Name);

	((CEGUI::IFalagardChatChannel*)(CEGUI::FalagardChatChannel*)m_pWindow)->addChannel(
		strUtf32_ChannelType.c_str(), strUtf32_IconName.c_str(), strUtf32_Name.c_str());

	m_pWindow->moveToFront();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* Scenemap::s_pMetaTable = NULL;
INT Scenemap::SetSceneZoomMode( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsNumber() ) return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->SetZoomMode( float( args[ 2 ].GetNumber()) );
	return 0;
}
INT Scenemap::UpdateViewRect( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	CEGUI::Point pos;
	pos.d_x = MyselfPos.x;
	pos.d_y = MyselfPos.z;
	pWindow->setMyselfPos( pos );

	pWindow->setSceneMapCenter( );
	return 0;
}
const char* Scenemap::GetSceneMapFilePostfixByName( const char* szFileName)
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_First_Column_Equ(_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, i);
		if( !stricmp(pLine->szSceneMap,szFileName))
		{
			if(strstr(pLine->szMiniMap,"jpg"))
				return "jpg";
			if(strstr(pLine->szMiniMap,"png"))
				return "png";
		}
	}

	return "jpg";
}

INT Scenemap::SetSceneFileName( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsNumber() ) return 0;
	if( !args[ 3 ].IsNumber() ) return 0;
	if( !args[ 4 ].IsString() ) return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->SetSceneSize( CEGUI::Size( float( args[ 2 ].GetNumber() ), float( args[ 3 ].GetNumber() ) ) );
	const char* szFileName = args[ 4 ].GetString();
	const char* szPostFix = GetSceneMapFilePostfixByName(szFileName);
	pWindow->SetSceneFileName( args[ 4 ].GetString(),szPostFix);

	return 0;
}
INT Scenemap::CloseSceneMap( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->Clear();
	return 0;
}

INT Scenemap::GetMouseScenePos( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	const CEGUI::Point pos = pWindow->GetMouseScenePos();
	pState->PushInteger( int( pos.d_x ) );
	pState->PushInteger( int( pos.d_y ) );
	return 2;
}


INT	Scenemap::UpdateSceneMap( LuaPlus::LuaState* pState )
{
	if( !m_pWindow->isVisible() )
		return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
	if( m_pWindow->isVisible() )
	{
		// 跟新所有的标志位置
		pWindow->Clear();

		for( INT i = 0; i < MINIMAP_TYPE_NUM; i ++ )
		{
			// 队友, 闪光点, 激活方向点
			if( i == MINIMAP_TYPE_TEAM || i == MINIMAP_TYPE_FLASH || i == MINIMAP_TYPE_ACTIVE )	// 5, 7
			{
				std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( i );

				if ( listObj == NULL )
				{
					continue;
				}
				
				// 得到了当前类型所有的物体的链表
				std::list< MAP_POS_DEFINE >::iterator it;
				for( it = listObj->begin(); it != listObj->end(); it ++ )
				{
					// 如果是队友，需要单独处理
					if( i == MINIMAP_TYPE_TEAM )
					{
						// 在同一场景才会添加
						if( (*it).dwSceneID != g_pWorldSystem->GetActiveSceneID() ) 
							continue;
					}

					pWindow->setMinimapData( i, (*it).pos.x, (*it).pos.z,  (CEGUI::String32)(CEGUI::utf8*)(*it).utf8name);
				}
			}
		}
	}

	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	int nDir = g_pWorldSystem->GetMyselfDir( );
	
	pWindow->setMyselfPos( CEGUI::Point( MyselfPos.x, MyselfPos.z ) );
	pWindow->setMyselfDir( nDir );

	return 0;
}

INT Scenemap::moveMapByDirection( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsNumber() ) return 0;
	if( !args[ 3 ].IsNumber() ) return 0;

	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;

	pWindow->moveMapByDirection( args[2].GetInteger(), args[3].GetInteger() );
	return 0;
}

INT Scenemap::AddTeamPlayerPos(LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	if( !args[ 3 ].IsInteger() ) return 0;
	if( !args[ 4 ].IsString() )	return 0;
	if( !m_pWindow->isVisible() )
		return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
	if( m_pWindow->isVisible() )
	{
		CEGUI::Point point = CEGUI::Point( CEGUI::Vector2(args[2].GetInteger(), args[3].GetInteger()) );
		CEGUI::String name = args[4].GetString();
		CEGUI::String32 str32Name;
		CUIStringSystem::GetMe()->ParserString_Runtime( name, str32Name );
		pWindow->addTeammatePos( point, str32Name );
	}

	return 0;
}

//INT Scenemap::SceneClear(LuaPlus::LuaState *pState)
//{
//	if( !m_pWindow->isVisible() )
//		return 0;
//	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
//	if( m_pWindow->isVisible() )
//	{
//		pWindow->Clear();
//	}
//
//	return 0;
//}

// 初始化世界地图
LuaPlus::LuaObject* Worldmap::s_pMetaTable = NULL;
INT	Worldmap::InitWorldMap( LuaPlus::LuaState* pState )
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	//std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( 2 );
	//std::list< MAP_POS_DEFINE >::iterator it;
	
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;

	pWindow->createBackground();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine )
		{
			if( pLine->nWroldMapPosX == 0 ||pLine->nWroldMapPosY == 0 )
				continue;
			//pWindow->setCityPos( pLine->nServerID, CEGUI::Point( float( pLine->nWroldMapPosX ), float( pLine->nWroldMapPosY ) ) );
			//pWindow->setCityNamePos( pLine->nServerID, CEGUI::Point( float( pLine->nNameWroldMapPosX ),float( pLine->nNameWroldMapPosY ) ) );
			//pWindow->setCityType( pLine->nServerID, pLine->szSceneType );
			//
			//pWindow->setCityNameNormalImage( pLine->nServerID, pLine->szCityNameNormalImageSet, pLine->szCityNameNormalImage  );
			//pWindow->setCityNameHoverImage( pLine->nServerID, pLine->szCityNameHoverImageSet, pLine->szCityNameHoverImage );

			////	从表格中读取Tips的信息，然后将其连接起来，作为Tooltip显示出来(_WX@7)
			//CEGUI::String cityName = pLine->szName;
			//INT cityLevel = pLine->nCityLevel;
			//char sztip[128];
			//sprintf( sztip, "  %s  等级：%d", cityName.c_str(), cityLevel );
			//CEGUI::String name;
			//CUIStringSystem::mbcs_to_utf8( sztip, name );
			//pWindow->setCityNameTooltipText( pLine->nServerID, (CEGUI::utf8*)name.c_str() );


			// 判断是不是当前场景
			//
			if( pLine->nServerID == g_pWorldSystem->GetActiveScene()->GetSceneDefine()->nServerID )
				pWindow->setCityCurrentScene( pLine->nServerID, 1 );
			else
				pWindow->setCityCurrentScene( pLine->nServerID, 0 );
		}

	}
	UpdatePlayer();
	pWindow->resetWorldMap();

	return 0;
}

INT	Worldmap::UpdateWorldMap( LuaPlus::LuaState* pState )
{
	UpdatePlayer();
	return 0;
}
INT Worldmap::GetCurrentSelectScene( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;
	pState->PushInteger( pWindow->getCurrentSelectScene() );
	return 1;
}
INT Worldmap::SetCityHover( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:SetCityHover Wrong Param1");
	}

	if (!(args[3].IsBoolean()))
	{
		KLThrow("LUA:SetCityHover Wrong Param2");
	}
	
	int sceneid = args[2].GetInteger();
	bool bHover = args[3].GetBoolean();

	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;

	pWindow->SetCityHover( sceneid, bHover );

	return 0;
}

CEGUI::String Worldmap::GetSceneNameByID( int id )
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_First_Column_Equ(_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, i);
		if( pLine->nServerID == id )
			return pLine->szName;
	}
	return "";
}
void Worldmap::UpdatePlayer()
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( MINIMAP_TYPE_TEAM );
	std::list< MAP_POS_DEFINE >::iterator it;
	// 遍历所有的好友
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;
	pWindow->clearPlayerInfo();
	for( it = listObj->begin(); it != listObj->end(); it ++ )
	{
		std::string strUtf8;
		std::string strMbcs = ( *it ).name;
		CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8 );
		pWindow->addPlayerToCity(  (*it ).dwSceneID, (CEGUI::String)(CEGUI::utf8*)strUtf8.c_str() );

	}
}
LuaPlus::LuaObject* InfoList::s_pMetaTable = NULL;
INT InfoList::AddInfo( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	//TextIndex
	std::string strMbcs = args[2].GetString();

	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	((CEGUI::IFalagardInfoList*)(CEGUI::FalagardInfoList*)m_pWindow)->AddInfo(strUtf32);
	return 0;
}

LuaPlus::LuaObject* CtrlList::s_pMetaTable = NULL;

INT CtrlList::AddColumn( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
	((CEGUI::MultiColumnList*)m_pWindow)->addColumn( strUtf32, args[ 3 ].GetInteger(), args[ 4 ].GetFloat() );

	return 0;
}

INT CtrlList::InsertColumn( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
	((CEGUI::MultiColumnList*)m_pWindow)->insertColumn(	strUtf32, args[ 3 ].GetInteger(), args[ 4 ].GetFloat(), args[ 5 ].GetInteger() );
	
	return 0;
}

INT CtrlList::RemoveColumn( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);

	if( !args[2].IsInteger() )
		return 0;

	((CEGUI::MultiColumnList*)m_pWindow)->removeColumn(	args[2].GetInteger() );

	return 0;
}

INT CtrlList::AddNewItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);

	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( strUtf32, args[ 3 ].GetInteger() );
	CEGUI::colour colorText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	pItem->setTextColours(colorText);
	pItem->setSelectionColours(CEGUI::colour(1.0f, 0.0f, 0.0f));

	if( args[5].IsString() )
	{
		pItem->setTextColours( CEGUI::PropertyHelper::stringToColour( args[5].GetString() ) );
	}
	if( args[6].IsString() )
	{
		pItem->setSelectionColours( CEGUI::PropertyHelper::stringToColour( args[6].GetString() ) );
	}
	if( args[7].IsString() )
	{
		std::string tooltips = args[7].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(tooltips, strUtf32);
		pItem->setTooltipText( strUtf32 );
	}

	if ( args[8].IsString() && args[9].IsString() )
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)args[8].GetString() , (CEGUI::utf8*)args[9].GetString() );
	}
	else
	{
		pItem->setSelectionBrushImage((CEGUI::utf8*)"wxsj_questlog", (CEGUI::utf8*)"questlog_choice_back");
	}

	//Find first listbox item in row.
	CEGUI::MCLGridRef  grid_ref( args[ 4 ].GetInteger(), 0 );
	CEGUI::ListboxItem* pExistItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pExistItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if(pExistItem)
	{//exist, add subItem
		((CEGUI::MultiColumnList*)m_pWindow)->setItem(pItem, args[3].GetInteger(), args[4].GetInteger());
	}
	else
	{//not exist, add new Item
		//set first listboxitem's ID as row idx. for CtrlList::GetSelectItem
		pItem->setID(args[4].GetInteger());
		((CEGUI::MultiColumnList*)m_pWindow)->addRow( pItem, args[ 3 ].GetInteger(), args[ 4 ].GetInteger() );
	}
	return 0;
}
INT CtrlList::SetItemText( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
		std::string caption = args[4].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
		pItem->setText( strUtf32 );
	}
	return 0;
}
INT CtrlList::SetItemData( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
	}
	return 0;
}
INT CtrlList::DeleteItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	((CEGUI::MultiColumnList*)m_pWindow)->removeRow( args[ 2 ].GetInteger() );
	return 0;
}
INT CtrlList::RemoveAllItem( LuaPlus::LuaState* pState )
{
	((CEGUI::MultiColumnList*)m_pWindow)->resetList();
	return 0;
}
INT CtrlList::GetItemText( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
		std::string strUtf8 = pItem->getText().c_str();//utf::mbcs_to_utf8(strMbcs);
		std::string strMbcs;
		CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
		pState->PushString( strMbcs.c_str() );
	}
	
	return 1;
}
INT CtrlList::GetItemCount( LuaPlus::LuaState* pState )
{
	pState->PushInteger( ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount() );
	return 1;
}
INT CtrlList::GetColumnCount( LuaPlus::LuaState* pState )
{
	pState->PushInteger( ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount() );
	return 1;
}
INT CtrlList::GetSelectItem( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxItem* pItem = (CEGUI::ListboxItem*)((CEGUI::MultiColumnList*)m_pWindow)->getFirstSelectedItem();
	if(!pItem) 
		pState->PushInteger(-1);
	else
		pState->PushInteger(pItem->getID());

	return 1;
}

INT CtrlList::SetSelectItem( LuaPlus::LuaState* pState )
{
	LuaStack args(pState);
	CEGUI::MultiColumnList* pMultiList = (CEGUI::MultiColumnList*)m_pWindow;
	INT rowidx = args[2].GetInteger();

	if(rowidx < 0 || rowidx >= (INT)pMultiList->getRowCount())	return 0;

	CEGUI::MCLGridRef  grid_ref(rowidx, 0);
	CEGUI::ListboxItem* pItem = pMultiList->getItemAtGridReference( grid_ref );
	if(pItem)	pMultiList->setItemSelectState(grid_ref, true);

	return 0;
}


LuaPlus::LuaObject* SoftKeyWindow::s_pMetaTable = NULL;

INT SoftKeyWindow::SetAimEditBox( LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[ 2 ].IsString() ) return 0;
	CEGUI::IFalagardSoftKeyBoard * pWindow= (CEGUI::IFalagardSoftKeyBoard *)(CEGUI::FalagardSoftKeyBoard*)m_pWindow;
	pWindow->SetActiveEditBox( (CEGUI::String)( CEGUI::utf8*)(args[2].GetString()) );
	return 0;
}

LuaPlus::LuaObject* AnimateWindow::s_pMetaTable = NULL;

INT AnimateWindow::Play( LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardAnimate * pWindow= (CEGUI::IFalagardAnimate*)(CEGUI::FalagardAnimate*)m_pWindow;
	LuaStack args(pState);
	pWindow->Play( args[ 2 ].GetBoolean() );
	return 0;
}

LuaPlus::LuaObject* EditBox::s_pMetaTable = NULL;

INT EditBox::setItemElementsString(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if(!args[2].IsString()) return 0;

	CEGUI::IFalagardIMEEditBox *pWindow = (CEGUI::IFalagardIMEEditBox*)(CEGUI::FalagardIMEEditBox*)m_pWindow;

	std::string strMBCS = args[2].GetString();
	CEGUI::String32 strUtf32;

	CUIStringSystem::GetMe()->ParserString_ElementOnly_Runtime(strMBCS, strUtf32);
	pWindow->setItemElementString(strUtf32);

	return 0;
}
INT	EditBox::setSelected( LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	if( !args[ 3 ].IsInteger() ) return 0;
	int nStart = args[ 2 ].GetInteger();
	int nEnd = args[ 3 ].GetInteger();
	CEGUI::Editbox *pWindow = (CEGUI::Editbox *)m_pWindow;
	if( nEnd == -1 )
		nEnd = int( pWindow->getText().length() );
	pWindow->setSelection( nStart, nEnd );
	return 0;
}
//目前只支持数字
INT EditBox::setBlinkText(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if( args[ 2 ].IsInteger() ) // 如果是个数字
	{
		CEGUI::IFalagardIMEEditBox *pWindow = (CEGUI::IFalagardIMEEditBox*)(CEGUI::FalagardIMEEditBox*)m_pWindow;

		char value[ 128 ];
		sprintf( value, "%d", args[ 2 ].GetInteger() );
		std::string strUtf8;
		mbcs_to_utf8(value, strUtf8);

		pWindow->setBlinkText((CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()));
	}

	return 0;
}

INT EditBox::getItemElementsString(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardIMEEditBox *pWindow = (CEGUI::IFalagardIMEEditBox*)(CEGUI::FalagardIMEEditBox*)m_pWindow;
	CEGUI::String32 strUtf32 = pWindow->getItemElementString();

	std::string strMBCS;
	CUIStringSystem::GetMe()->ParserString_ReverseRuntime(strUtf32, strMBCS);
	pState->PushString(strMBCS.c_str());

	return 1;
}


LuaPlus::LuaObject* MultiLineEditbox::s_pMetaTable = NULL;
INT MultiLineEditbox::setItemElementsString(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if(!args[2].IsString()) return 0;

	CEGUI::FalagardMultiIMEEditBox *pWindow = (CEGUI::FalagardMultiIMEEditBox*)m_pWindow;

	std::string strMBCS = args[2].GetString();
	CEGUI::String32 strUtf32;

	CUIStringSystem::GetMe()->ParserString_ElementOnly_Runtime(strMBCS, strUtf32);
	pWindow->setItemElementString(strUtf32);

	return 0;
}

INT MultiLineEditbox::getItemElementsString(LuaPlus::LuaState* pState)
{
	CEGUI::FalagardMultiIMEEditBox *pWindow = (CEGUI::FalagardMultiIMEEditBox*)m_pWindow;
	CEGUI::String32 strUtf32 = pWindow->getItemElementString();

	std::string strMBCS;
	CUIStringSystem::GetMe()->ParserString_ReverseRuntime(strUtf32, strMBCS);
	pState->PushString(strMBCS.c_str());

	return 1;
}

// 设置并显示滚动公告板		20100624 BLL
LuaPlus::LuaObject* ScrollInfo::s_pMetaTable = NULL;

INT ScrollInfo::setScrollInfo(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardScrollInfo *pWindow = (CEGUI::IFalagardScrollInfo*)(CEGUI::FalagardScrollInfo*)m_pWindow;
	LuaStack args(pState);
	std::string strMbcs = args[2].GetString();
	std::string useDefaltColor;
	if(args[3].IsString()) useDefaltColor = args[3].GetString();
	else useDefaltColor == "0";

	CEGUI::String32 strUtf32;
	if(useDefaltColor=="0")  //使用默认颜色
	{
		strUtf32 += 0xFBFFFFFF; // 字体颜色赋值白色
		CUIStringSystem::GetMe()->ParserString_Runtime( strMbcs, strUtf32 );		
	}
	else					//使用strMbcs中定义的颜色
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	pWindow->setWaitingInfo( strUtf32 );
	return 0;	
}


// 树型控件类
LuaPlus::LuaObject* Tree::s_pMetaTable = NULL;

//清除树型控件上的所有子控件
INT Tree::ClearTree( LuaPlus::LuaState* state )
{
	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;

	pWindow->resetList();

	return 0;
}

// 添加子项目
INT Tree::AddItem( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if (!(args[2].IsString()))		//任务名字 
		return 0;
	if (!(args[3].IsInteger()))	//ItemId
		return 0;

	//Text
	std::string strMbcs = args[2].GetString();
	//std::string strUtf8;
	//CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	//TextIndex
	INT nIndex = args[3].GetInteger();

	CEGUI::colour colorText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	if(args[4].IsString())
	{
		colorText = CEGUI::PropertyHelper::stringToColour((CEGUI::utf8*)args[4].GetString());
	}

	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;

	if( !pWindow ) return 0;



	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	CEGUI::TreeItem* pItem = new CEGUI::TreeItem( strUtf32, nIndex );
	pItem->setSelectionBrushImage( "wxsj_questlog", "questlog_choice_back" );
	pItem->setTextColours(colorText);

	if ( args[5].IsString() )		//是否传入了场景名字
	{
		std::string strSceneMbcs = args[5].GetString();
		//std::string strSceneUtf8;
		//CUIStringSystem::mbcs_to_utf8(strSceneMbcs, strSceneUtf8);
		CEGUI::String32 strUtfScene32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strSceneMbcs, strUtfScene32);

		CEGUI::TreeItem* SceneItem = pWindow->findFirstItemWithText( strUtfScene32 );
		if( SceneItem == NULL ) 
		{
			SceneItem = new CEGUI::TreeItem( strUtfScene32 );
			pWindow->addItem( SceneItem );
		}
		SceneItem->setSelectionBrushImage( "wxsj_questlog", "questlog_choice_back" );
		SceneItem->addItem(pItem);
	}
	else
	{
		pWindow->addItem(pItem);
	}

	return 0;
}


//获得选择的item的id
INT Tree::GetFirstSelectItem( LuaPlus::LuaState* state )
{
	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;
	CEGUI::TreeItem* pItem = pWindow->getFirstSelectedItem();
	if( !pItem )
	{
		state->PushInteger(-1);
		return 1;
	}

	if ( pItem->getItemCount() > 0 )
	{
		state->PushInteger(-1);
		return 1;
	}

	state->PushInteger( pItem->getID() );
	return 1;
}


//获得选择的items的名字
INT Tree::GetFirstSelectItemString( LuaPlus::LuaState* state )
{
	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;
	CEGUI::TreeItem* pItem = pWindow->getFirstSelectedItem();
	if( !pItem )
	{
		state->PushString("");
		return 1;
	}
	
	std::string strUtf8 = pItem->getText().c_str();
	std::string strMbcs;
	CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
	state->PushString( strMbcs.c_str() );

	return 1;
}

INT Tree::SetFirstSelectItemString( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if (!(args[2].IsString())) 
		return 0;

	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;
	CEGUI::TreeItem* pItem = pWindow->getFirstSelectedItem();

	if( !pItem )
	{
		std::string strMbcs = args[2].GetString();
		CEGUI::String32 strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
		pItem->setText(strUtf32);
		pWindow->requestRedraw();
	}

	//CEGUI::ListboxTextItem* pItem = (CEGUI::ListboxTextItem*)((CEGUI::Listbox*)m_pWindow)->getFirstSelectedItem();
	//if( pItem != NULL )
	//{
	//	std::string strMbcs = args[2].GetString();
	//	CEGUI::String32 strUtf32;
	//	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	//	pItem->setText(strUtf32);
	//	((CEGUI::Listbox*)m_pWindow)->requestRedraw();
	//}
	
	return 0;
}

INT Tree::SetItemSelectByItemID( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))	//ItemId
		return 0;
	INT id = args[2].GetInteger();

	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;
	pWindow->setItemSelectState( id, true );

	return 0;
}


INT Tree::SetItemToggle( LuaPlus::LuaState* state )
{
	CEGUI::FalagardTree *pWindow = (CEGUI::FalagardTree*)m_pWindow;
	CEGUI::TreeItem* pItem = pWindow->getFirstSelectedItem();
	if( !pItem ) return 0;

	if( pItem->getItemCount() == 0 ) return 0;

	pItem->toggleIsOpen();
	pWindow->requestRedraw();

	return 0;

}




} // end namespace
