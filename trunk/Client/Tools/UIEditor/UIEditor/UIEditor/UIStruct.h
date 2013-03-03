
#pragma once

//��Ҫ�޸ĺ궨��
//#define UIEDITOR_VERSION 10
//#define UIEDITOR_ID	0x88889999

//�ӿڿ���ʹ�õ�����ID��Χ
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
	VT_COLORS,			//ֵ����ɫ
	VT_IMAGE,			//ֵ��ͼ�񣬰���ͼ�񼯺�
	VT_ENUM,			//�����ö��������arrInitValueΪö��ֵ������������ֵ
	VT_STRINGOP,	//��Ҫ������ܵõ���ʼֵ
};

struct PE_List
{
	char name[MAX_NAME];		//���Ƶ�ֵ
	CT_TYPE inputType;				//ֵ�Ĳ�������
	char toolTip[MAX_TIP];			//��ʾ����
	//��ʼ����ֵ, ֵ֮����\0������������numOfInitValueȷ����
	//���ݲ�ͬ�����ͣ���ʼֵ��ͬ,���alueType== VT_ENUM�򣬸�ֵΪ�ַ������ͣ�
	//�����VT_FLOAT,VT_INTERGER,VT_UINTERGER,���һ���������10���ƣ��������������
	//SPLIN�����޺����ޣ�����������
	//�������VT_IMAGE�򱣴����Ĭ�ϵ�ͼ�񼯺��ַ�����
	//�����VT_COLORS���ʾ����Ĭ����ɫ���ַ���
	char* arrInitValue;					
	INT numOfInitValue;				//��ʼ���Եĸ���
	VALUE_TYPE valueType;		//ֵ������
	char value[MAX_NAME];		//ֵ������
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
	HCURSOR hCursor;		//������ṩ������Ϊ0
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
//EPT_IMAGE ���Ժ������������Թ���
enum ENUM_PRJ_TYPE
{
	EPT_LAYOUT,
	EPT_IMAGE,
	EPT_DESIGN,
};
enum MENU_TYPE
{
	//�м����Ⱦ����
	MENU_TYPE_MIDDLE_VIEW_CONTEXT,
	//��ߵ�ͼ�񴰿�
	MENU_TYPE_LEFT_IMAGE_CONTEXT,
	//��ߵ�UI���ڴ���
	MENU_TYPE_LEFT_WINDOW_CONTEXT,
	//�ұߵĴ���
	MENU_TYPE_RIGHT_VIEW_CONTEXT,
	//���˵�
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
	//ϵͳ��ʼ��ʱ����ã���������������δ����
	virtual bool Initialize(HINSTANCE hInst) = 0;
	//ϵͳ�˳�ʱ����
	virtual bool UnInitialize() = 0;
	//��ȡToolBar��Ϣ�������Ҫ���TOOLBAR����ṹ������ṹ�����ڱ༭����
	virtual void GetToolBarInfo(TOOLBAR& BarInfo) = 0;
	//ע�������Ϣ��ֻ����Ⱦ���ڲŻ�����������������Ĵ��ڲ������
	virtual void InjectMouseEvent(ENUM_MOUSE_BUTTON btn, ENUM_MOUSE_OP op, POINT pos, ENUM_KEY_STATE ckey) = 0;
	//ͬ�ϣ�����ע����Ϣ
	virtual void InjectKeyboardEvent(ENUM_KEY_STATE state, UINT keyCode, UINT repeat) = 0;
	//ͬ�ϣ��ַ�ע����Ϣ
	virtual void InjectChar(UINT nChar, UINT repeat) = 0;
	//ɾ��һ������
	virtual bool DelWindow(const char* wndName) = 0;
	//����һ������
	virtual bool CreateUIWnd(UINT wndTypeID, const char* pParentName) = 0;
	//��Ӧ�û�����������������û�����˹���������Ͳ˵�����
	virtual void OnCommand(UINT nCmd, const char* pName = NULL) = 0;
	//��ʾ�˵�����
	virtual bool GetMenuInfo(MENU_TYPE type, UINT& nCount, MENU_ITEM ** pMenuItem, const char* pContext = NULL) = 0;
	//���˵���ѡ���ʱ�����
	virtual void OnMenuSelect(MENU_TYPE type, UINT nCmd, ENUM_KEY_STATE key) = 0;
		
};
//save and open and any operate
struct IAction
{
	virtual bool CreateProject(ENUM_PRJ_TYPE type) = 0;
	virtual bool OpenProject(ENUM_PRJ_TYPE type, TCHAR* pScriptName, TCHAR* pLayoutName)  = 0;
	virtual bool CloseProject(TCHAR* pScriptName, TCHAR* pLayoutName)  = 0;
	//���µ������Ժ��¼�
	virtual bool UpdatePropety(PE_List* pWho, char* pValue) = 0;
	virtual bool UpdateEvent(PE_List * pName, char* pValue) = 0;
	//���һ��ֵ�Ƿ�����Ҫ��
	virtual bool CheckValue(PE_List* pName, char* pValue) = 0;
	//�����ַ���,һ���������ɽű�����
	virtual bool StringOp(PE_List* pName,  int & strCount, char* pOut, int OutLen) = 0;
	//���û����Spinʱ��ᱻ����
	virtual bool OnSpinPosChange(ENUM_LIST who, PE_List* pList, INT currPos, INT delta) = 0;
	//���õ�ǰ��ѡ�񴰿�
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
	//�򴰿��������һ���½ڵ�,who�������Ǹ����ڲ��룬 childNameҪ���������
	//parentPath�����ڵ��·����һ��Ҫ�Ӹ��ڵ㿪ʼ������/�ŷָ���
	virtual bool InsertChildToTree(ENUM_TREE who, const char* childName, const char* parentPath)  = 0;
	//ɾ�����еĽڵ�
	virtual bool DeleteAllTreeItem(ENUM_TREE who) = 0;
	//ɾ��ָ���Ľڵ�
	virtual bool DeleteTreeItem(ENUM_TREE who, const char* childName) = 0;
	//֪ͨ�༭��һ�����ڱ�ѡ����
	virtual bool OnUIWndSelected(const char* Who) = 0;
	//�����Խ���������е����Զ�
	virtual bool InsertPropety(UINT nCount, PE_List** plist)  = 0;
	//���¼������в����¼���
	virtual bool InsertEvent(UINT nCount, PE_List** plist)  = 0;
	//���һ���б�
	virtual bool CleanupList(ENUM_LIST who) = 0;
	//֪ͨ����ĳ���б���
	virtual bool UpdateListItem(ENUM_LIST who, PE_List* pList, INT nItem) = 0;
	//��ȡ�༭������Ϣ
	virtual void GetEditorInfo(EditorInfo* pInfo)  = 0;
	//ִ��Lua�ű�
	virtual void LuaDoString(const char* pFunc) = 0;
	//��һ�����ڱ�����ʱ���������
	virtual void OnHideWindow(const char* wndName) = 0;
	//�ȴ������������ʾæ����꣬һ��Ҫ�ɶԵ���
	virtual void WaitForPluginWork(BOOL wait) = 0;
	//save
	virtual void PrepareSave() = 0;
};
