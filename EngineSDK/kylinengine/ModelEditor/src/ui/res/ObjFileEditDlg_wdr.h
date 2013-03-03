#ifndef __WDR_ObjFileEditDlg_H__
#define __WDR_ObjFileEditDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ObjFileEditDlg_wdr.h"
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

const int ID_LISTBOX_SCENE_SOUND				= 10000;
const int ID_TEXT								= 10001;
const int ID_TEXT_SOUNDIDTEXT					= 10002;
//Locator
const int ID_COMBO_LOCATOR_NAME					= 10003;
const int ID_COMBO_ATTACTHMODEL					= 10004;
const int ID_COMBO_SLOTNAME						= 10005;
const int ID_COMOB_BONE							= 10006;
const int ID_TEXTCTRL_LOCATOR_POSTION			= 10007;
const int ID_TEXTCTRL_LOCATOR_ORIENTATION		= 10008;
const int ID_BUTTON_LOCATOR_ADD					= 10009;
const int ID_BUTTON_LOCATOR_DEL					= 10010;
//effect
const int ID_COMBO_EFFECT						= 10011;
//object
const int ID_TEXTCTRL_OBJECTNAME				= 10012;
const int ID_BUTTON_OBJECT						= 10013;
//attribute
const int ID_COMBO_PROPERTYNAME					= 10014;
const int ID_TEXTCTRL_PROPERVALUE				= 10015;
const int ID_COMBO_PROPERHINTS					= 10016;
const int ID_BUTTON_ATTRIBUTE					= 10017;
//Entitylist
const int ID_COMBO_ENTITYNAME					= 10018;
const int ID_TEXTCTRL_ENTITYFILE				= 10019;
const int ID_COMBO_ENTITYMATERIAL				= 10020;
const int ID_BUTTON_ENTITYLIST					= 10021;
//Materiallist
const int ID_COMBO_MATERIALNAME					= 10022;
const int ID_TEXTCTRL_MATERIALENTITY			= 10023;
const int ID_COMBO_MATERIALMATERIAL				= 10024;
const int ID_BUTTON_MATERIALLIST				= 10025;
//Skeleton
const int ID_COMBO_SKELETON_NAME				= 10026;
const int ID_COMBO_SKELETON_ANIMATION			= 10027;
const int ID_COMBO_SKELETON_TABLE				= 10028;
const int ID_BUTTON_SKELETON					= 10029;
const int ID_BUTTON_SKELETON_ADD				= 10030;
const int ID_BUTTON_SKELETON_DEL				= 10031;

//preview
const int ID_TEXTCTRL_PREVIEW					= 10032;
//statictext
const int ID_STATIC_ATTRIBUTE					= 10033;
const int ID_TEXT_ATTRIBUTE_NAME				= 10034;
const int ID_TEXT_ATTRIBUTE_HINTS				= 10035;
const int ID_TEXT_ATTRIBUTE_SLOT				= 10036;
const int ID_TEXT_ATTRIBUTE_ATTACH				= 10037;

const int ID_STATIC_ENTITY						= 10038;
const int ID_TEXT_ENTITY_NAME					= 10039;
const int ID_TEXT_ENTITY_FILE					= 10040;
const int ID_TEXT_ENTITY_MATERIAL				= 10041;

const int ID_STATIC_MATERIAL					= 10042;
const int ID_TEXT_MATERIAL_NAME					= 10043;
const int ID_TEXT_MATERIAL_ENTITY				= 10044;
const int ID_TEXT_MATERIAL_MATERIAL				= 10045;

const int ID_STATIC_LOCATOR						= 10046;
const int ID_TEXT_LOCATOR_NAME					= 10047;
const int ID_TEXT_LOCATOR_BONE					= 10048;
const int ID_TEXT_LOCATOR_EFFECT				= 10049;
const int ID_TEXT_LOCATOR_POS					= 10050;
const int ID_TEXT_LOCATOR_ORI					= 10051;

const int ID_STATIC_SKELETON					= 10052;
const int ID_TEXT_SKELETON_NAME					= 10053;
const int ID_TEXT_SKELETON_TABLE				= 10054;

//new file
const int ID_COMBO_NEWFILE						= 10055;

const int ID_STATIC_ATTACHMODEL					= 10056;

const int ID_STATIC_POSANDORI					= 10057;

wxSizer *ObjFileEditDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#endif
