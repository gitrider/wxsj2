
/** ϵͳ���ýӿ�
 */


#include "../Global.h"
#include "GMInterface_Lua.h"


namespace SCRIPT_SANDBOX
{

//ϵͳ����
class SystemSetup : public LuaExport< SystemSetup >
{
public:
	//---------------------------------------------------------
	//�����ػ���
	static VOID	WINAPI	_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//������ص��趨
	INT Lua_Texture(LuaPlus::LuaState* state);
	//��ʾ������ص��趨
	INT Lua_Display(LuaPlus::LuaState* state);
	//������ص��趨
	INT Lua_Sound(LuaPlus::LuaState* state);
	//��ϵͳ������
	INT Lua_OpenSetup(LuaPlus::LuaState* state);
	//����Ƶ���ý���
	INT Lua_ViewSetup(LuaPlus::LuaState* state);
	//���������ý���
	INT Lua_SoundSetup(LuaPlus::LuaState* state);
	//��UI���ý���
	INT Lua_UISetup(LuaPlus::LuaState* state);
	//�򿪰������ý���
	INT Lua_InputSetup(LuaPlus::LuaState* state);
	//����Ϸ�����ý���
	INT Lua_GameSetup(LuaPlus::LuaState* state);
	//��Ϸ����
	INT Lua_GameHelp(LuaPlus::LuaState* state);
	//������Ϸ
	INT Lua_BackGame(LuaPlus::LuaState* state);

	//-----------------------------------------------------------------------------
	//��Ƶ���
	INT Lua_View_GetData(LuaPlus::LuaState* state);
	INT Lua_View_SetData(LuaPlus::LuaState* state);
	INT Lua_View_SaveToFile(LuaPlus::LuaState* state);
	//-----------------------------------------------------------------------------
	//��Ϸ�����
	INT Lua_GameGetData(LuaPlus::LuaState* state);
	INT Lua_SaveGameSetup(LuaPlus::LuaState* state);

	INT Set_GameData(INT nData);

	// ��������
	INT Lua_ChangeViewSize( LuaPlus::LuaState* state );

	//-----------------------------------------------------------------------------
	//�����Ϣ

	//��ҳ��
	INT Lua_OpenPrivatePage(LuaPlus::LuaState* state);
	//���������Ϣ
	INT Lua_AskPrivateInfo(LuaPlus::LuaState* state);
	//�ύ�����Ϣ
	INT Lua_ApplyPrivateInfo(LuaPlus::LuaState* state);
	//��������Ϣ
	INT Lua_GetPrivateInfo(LuaPlus::LuaState* state);
	//��������Ϣ
	INT Lua_SetPrivateInfo(LuaPlus::LuaState* state);
	//�������Ϣ�����л������װ������
	INT Lua_OpenEquipFrame(LuaPlus::LuaState* state);
	//�������Ϣ�����л�����ҳ������
	INT Lua_OpenPetFrame(LuaPlus::LuaState* state);
	//���������Ͻ�����ĵ�Obj
	INT Lua_GetCaredObjId(LuaPlus::LuaState* state);

public:
	SystemSetup() : LuaExport< SystemSetup >("SystemSetup") {}

	//---------------------------------------------------------
	//������Ļ�ֱ���
	static VOID	_setResolution(INT nWidth, INT nHeight);
	//���ô����Ƿ�ȫ��
	static VOID	_setResolution(BOOL bFullScreen);
protected:
	//������Ϸ�����õ���ֵ����Server�������ģ�
	INT						m_GameData;
};

}
