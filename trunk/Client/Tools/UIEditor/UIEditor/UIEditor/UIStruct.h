
#pragma once

//不要修改宏定义
//#define UIEDITOR_VERSION 10
//#define UIEDITOR_ID	0x88889999

//接口可以使用的命令ID范围
#define ID_BEGIN	40000
#define ID_END		50000

#define MAX_NAME 32
#define MAX_TIP 64

enum CT_TYPE
{
	CT_EDITBOX,
	CT_COMBOX,
	CT_SPINBOX,
	CT_COLORDLG,
	CT_BROWSEDLG,
};

enum VALUE_TYPE
{
	VT_INTERGER,
	VT_UINTERGER,
	VT_STRING,
	VT_FLOAT,
	VT_COLORS,			//值是颜色
	VT_IMAGE,			//值是图像，包括图像集和
	VT_ENUM,			//如果是枚举类型则arrInitValue为枚举值，不容许其他值
	VT_STRINGOP,	//需要计算才能得到初始值
};

struct PE_List
{
	char name[MAX_NAME];		//名称的值
	CT_TYPE inputType;				//值的操作类型
	char toolTip[MAX_TIP];			//提示文字
	//初始属性值, 值之间用\0隔开，个数由numOfInitValue确定，
	//根据不同的类型，初始值不同,如果alueType== VT_ENUM则，该值为字符串类型，
	//如果是VT_FLOAT,VT_INTERGER,VT_UINTERGER,则第一个保存的是10进制，后两个保存的是
	//SPLIN的上限和下限，必须是整数
	//，如果是VT_IMAGE则保存的是默认的图像集合字符串，
	//如果是VT_COLORS则表示的是默认颜色的字符串
	char* arrInitValue;					
	INT numOfInitValue;				//初始属性的个数
	VALUE_TYPE valueType;		//值的类型
	char value[MAX_NAME];		//值的内容
	/*PE_List()
	{
		memset(name, 0, sizeof(name));
		memset(toolTip, 0, sizeof(toolTip));
		inputType = CT_COMBOX;
		valueType = VT_STRING;
		numOfInitValue = 0;
		//mems
		et(arrInitValue, 0, sizeof(arrInitValue));
		arrInitValue = NULL;
	}*/
};


//the follow is the Interface of CEGUI

enum
{
	INTERFACE_SYSTEM,
	INTERFACE_RENDER,
};

enum ENUM_TREE
{
	ET_WINDOW_TREE,
	ET_IMAGE_TREE,
};

#define MENU_SPERATOR	0
#define MENU_PARENT_ITEM 1
struct MENU_ITEM 
{
	TCHAR ItemName[100];
	UINT nCmdID;			//nCmdID == MENU_SPERATOR then A Seperator will be append
};
enum DESING_SIZE
{
	DESIGN_SIZE_800_600,
	DESIGN_SIZE_1024_768,
	DESIGN_SIZE_UNLIMIT,
};
struct IRender
{
	virtual bool Initialize(HINSTANCE hInst, HWND hMainWnd) =0;
	virtual bool UnInitialize() =0;
	virtual void PreRender() = 0;
	virtual void Render(UINT dwEslapse) =0;
	virtual void OnSizeChange(DESING_SIZE designSize, UINT width, UINT height) = 0;
};
#define MAX_TOOL_NAME 32
#define MAX_TOOL_TIP 64
struct TOOLBARITEM 
{
	UINT commandID;   // must from 40000 to 50000
	TCHAR name[MAX_TOOL_NAME];
	TCHAR toolTip[MAX_TOOL_TIP];
	HCURSOR hCursor;		//如果不提供则设置为0
	UINT imageID;
};

struct TOOLBAR 
{
	UINT numBars;			//button's number
	TOOLBARITEM* barItem;	//store the bar item
	HIMAGELIST  hBitmapNormal;			//image of the bar
	HIMAGELIST  hBitmapHover;			//image of the bar
	HIMAGELIST  hBitmapDisable;			//image of the bar
	UINT height;						//each bar 's height must be same of the image
	UINT width;						//each bar's widht
	UINT column;						//tool bars's coloumn > 1
};

enum ENUM_MOUSE_BUTTON
{
	EM_LEFTBUTTON,
	EM_RIGHTBUTTON,
	EM_MIDDBUTTON,		//scroll
	EM_MOUSEMOVE,
};
enum ENUM_MOUSE_OP
{
	OP_BUTTON_DOWN,
	OP_BUTTON_UP,
	OP_BUTTON_DRAGING,
	OP_BUTTON_DRAGEND,
	OP_BUTTON_DBDOWN,
	OP_BUTTON_SCROLLUP,
	OP_BUTTON_SCROLLDOWN,
	OP_MOUSE_MOVE,
};
enum ENUM_KEY_STATE
{
	EK_KEYDOWN = 1,
	EK_KEYUP = 2,
	EK_KEYDOUBLE = 4,
	EK_CONTROL = 8,
	EK_SHIFT = 16,
	EK_ALT = 32,

};
//EPT_IMAGE 可以和另外两个可以共存
enum ENUM_PRJ_TYPE
{
	EPT_LAYOUT,
	EPT_IMAGE,
	EPT_DESIGN,
};
enum MENU_TYPE
{
	//中间的渲染窗口
	MENU_TYPE_MIDDLE_VIEW_CONTEXT,
	//左边的图像窗口
	MENU_TYPE_LEFT_IMAGE_CONTEXT,
	//左边的UI窗口窗口
	MENU_TYPE_LEFT_WINDOW_CONTEXT,
	//右边的窗口
	MENU_TYPE_RIGHT_VIEW_CONTEXT,
	//主菜单
	MENU_TYPE_MAIN,
};
enum ENUM_LIST
{
	ECL_EVENT_LIST,
	ECL_PROP_LIST,
};
//initialize and run with the tools
struct ISystem 
{
	//系统初始化时候调用，调用是主窗口尚未创建
	virtual bool Initialize(HINSTANCE hInst) = 0;
	//系统退出时调用
	virtual bool UnInitialize() = 0;
	//获取ToolBar信息，插件需要填充TOOLBAR这个结构，这个结构保存在编辑器中
	virtual void GetToolBarInfo(TOOLBAR& BarInfo) = 0;
	//注入鼠标消息，只有渲染窗口才会调用这个函数，两侧的窗口不会调用
	virtual void InjectMouseEvent(ENUM_MOUSE_BUTTON btn, ENUM_MOUSE_OP op, POINT pos, ENUM_KEY_STATE ckey) = 0;
	//同上，键盘注入消息
	virtual void InjectKeyboardEvent(ENUM_KEY_STATE state, UINT keyCode, UINT repeat) = 0;
	//同上，字符注入消息
	virtual void InjectChar(UINT nChar, UINT repeat) = 0;
	//删除一个窗口
	virtual bool DelWindow(const char* wndName) = 0;
	//创建一个窗口
	virtual bool CreateUIWnd(UINT wndTypeID, const char* pParentName) = 0;
	//响应用户的命令操作，包括用户点击了工具条命令和菜单命令
	virtual void OnCommand(UINT nCmd, const char* pName = NULL) = 0;
	//显示菜单命令
	virtual bool GetMenuInfo(MENU_TYPE type, UINT& nCount, MENU_ITEM ** pMenuItem, const char* pContext = NULL) = 0;
	//当菜单被选择的时候调用
	virtual void OnMenuSelect(MENU_TYPE type, UINT nCmd, ENUM_KEY_STATE key) = 0;
		
};
//save and open and any operate
struct IAction
{
	virtual bool CreateProject(ENUM_PRJ_TYPE type) = 0;
	virtual bool OpenProject(ENUM_PRJ_TYPE type, TCHAR* pScriptName, TCHAR* pLayoutName)  = 0;
	virtual bool CloseProject(TCHAR* pScriptName, TCHAR* pLayoutName)  = 0;
	//更新单个属性和事件
	virtual bool UpdatePropety(PE_List* pWho, char* pValue) = 0;
	virtual bool UpdateEvent(PE_List * pName, char* pValue) = 0;
	//检查一个值是否满足要求
	virtual bool CheckValue(PE_List* pName, char* pValue) = 0;
	//操作字符串,一般用作生成脚本函数
	virtual bool StringOp(PE_List* pName,  int & strCount, char* pOut, int OutLen) = 0;
	//当用户点击Spin时候会被调用
	virtual bool OnSpinPosChange(ENUM_LIST who, PE_List* pList, INT currPos, INT delta) = 0;
	//设置当前的选择窗口
	virtual void SetSelectWindow(const char* wndName) = 0;
};

//image interface process the image
struct IImage 
{
};

struct EditorInfo 
{
};

//UIEditor provide interface
struct IEditor 
{
	//向窗口树里插入一个新节点,who代表向那个窗口插入， childName要插入的名称
	//parentPath代表父节点的路径，一定要从根节点开始算起，用/号分隔开
	virtual bool InsertChildToTree(ENUM_TREE who, const char* childName, const char* parentPath)  = 0;
	//删除所有的节点
	virtual bool DeleteAllTreeItem(ENUM_TREE who) = 0;
	//删除指定的节点
	virtual bool DeleteTreeItem(ENUM_TREE who, const char* childName) = 0;
	//通知编辑器一个窗口被选择了
	virtual bool OnUIWndSelected(const char* Who) = 0;
	//向属性界面插入所有的属性对
	virtual bool InsertPropety(UINT nCount, PE_List** plist)  = 0;
	//向事件界面中插入事件对
	virtual bool InsertEvent(UINT nCount, PE_List** plist)  = 0;
	//清空一个列表
	virtual bool CleanupList(ENUM_LIST who) = 0;
	//通知更新某个列表项
	virtual bool UpdateListItem(ENUM_LIST who, PE_List* pList, INT nItem) = 0;
	//获取编辑器的信息
	virtual void GetEditorInfo(EditorInfo* pInfo)  = 0;
	//执行Lua脚本
	virtual void LuaDoString(const char* pFunc) = 0;
	//当一个窗口被隐藏时被插件调用
	virtual void OnHideWindow(const char* wndName) = 0;
	//等待插件操作，显示忙型鼠标，一定要成对调用
	virtual void WaitForPluginWork(BOOL wait) = 0;
	//save
	virtual void PrepareSave() = 0;
};
