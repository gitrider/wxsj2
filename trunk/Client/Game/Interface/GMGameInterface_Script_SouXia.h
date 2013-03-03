/*
	����¼��ؽӿ�
*/

#pragma once


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

struct SOUXIA_DATA;
class CObject_Item;
class CObject_PlayerOther;

namespace SCRIPT_SANDBOX
{
	class SouXiaLu
	{
	public:
		SouXiaLu(){};
		virtual ~SouXiaLu(){};
		
		static SouXiaLu s_SouXiaLu;
		static LuaPlus::LuaObject* s_pMetaTable;

	public:

		//��������Ƿ�װ��������¼
		INT Lua_IsEquipSoiuXia( LuaPlus::LuaState* state );

		//	��ñ�����ָ�����͵�����¼��������ߵ���Ŀ
		INT Lua_GetSouXiaUsbNum( LuaPlus::LuaState* state );

		// ��õ�ǰҳ�棬USB�����������ʾ���ڣ�ָ��λ��Ӧ����ʾ����Ʒ (����Ҫ��������ʾ���ڼ�ҳ<INT>, ��ʾ���ڵڼ���λ��<INT> )
		INT Lua_GetSouXiaUsbItem( LuaPlus::LuaState* state );

		// ���װ��������¼��ǰ�򿪵����͵��ܵ�ҳ��
		INT Lua_GetSouXiaPageNum_Type( LuaPlus::LuaState* state );

		// ��õ�ǰ�򿪵�����¼���ߵ�Ʒ�ʣ�1-4 �ֱ�������͵�������
		INT Lua_GetSouXiaLevel( LuaPlus::LuaState* state );

		//	��õ�ǰװ��������¼���ߵĶ����������������������
		INT Lua_GetSouXiaIndex( LuaPlus::LuaState* state );

		// ��õ�ǰװ��������¼�����ڵ�ǰ�򿪵Ľ����������Ѿ�ѧϰ�ļ���ҳ��
		INT Lua_GetSouXiaPageNum( LuaPlus::LuaState* state );

		// ѧϰUSB�����������
		INT Lua_StudyByUSBItem( LuaPlus::LuaState* state );

		// �������¼������ѧ�б��У���Xҳ��Y���ļ��ܵ�ID
		INT Lua_GetSouXiaSkillID( LuaPlus::LuaState* state );

		// ���ݼ���ID�����ActionID
		INT Lua_GetSouXiaActionIDBySkillID( LuaPlus::LuaState* state );

		// ���ݼ���ID��øü��ܵ�����
		INT Lua_GetSouXiaSkillName( LuaPlus::LuaState* state );

		// �������¼������ѧ�б��У���X���䷽���䷽ID
		INT Lua_GetSouXiaArmID( LuaPlus::LuaState* state );

		// �����䷽ID����úϳɺ��������UI��ʾͼ��
		INT Lua_GetSouXiaArmIcon( LuaPlus::LuaState* state );

		// �����䷽ID����úϳɺ��������ActionID�� ���֣� ˵��
		INT Lua_GetSouXiaArmData( LuaPlus::LuaState* state );

		// ��ȡ����¼��ѧ�б��У���X���䷽����Ĳ���
		INT Lua_GetSouXiaArmMatData( LuaPlus::LuaState* state );

		// ������ұ����ض�λ�õ�USB����������ڴ洢�ľ������Ϣֵ�����ܣ��䷽��Ʒ�����ޣ������ID��
		INT Lua_GetSouXia_Data( LuaPlus::LuaState* state );

		// ����Ԥ��ĳ�����ܵ�Ч��
		INT Lua_SetPrewSkill( LuaPlus::LuaState* state );

		// ��������ID����ʾ�������ε�Ԥ��
		INT Lua_SetPrewArm( LuaPlus::LuaState* state );

		// ����Ԥ��ĳ������
		INT Lua_SetPrewPet( LuaPlus::LuaState* state );

		//����Ԥ��ĳ������
		INT Lua_SetPrewZuoJi( LuaPlus::LuaState* state );

		// ����������ε�Ԥ��
		INT Lua_ClearPrewArm( LuaPlus::LuaState* state );

		// ��ʼ��ģ�͵�Avatra
		INT Lua_InitModelAvart( LuaPlus::LuaState* state );

		// ��������䷽����� ����������ť
		INT Lua_SouXiaLu_ArmMake( LuaPlus::LuaState* state );

		// �жϱ�����ĳ��λ���ϵ�����¼�����Ƿ��ǿհ�����¼
		INT Lua_IsBlankSouXiaItem( LuaPlus::LuaState* state );

		//����ActionID������б���ļ���ID
		INT Lua_GetSkillIdByActionID( LuaPlus::LuaState* state );

		//�����ٻ�����ID��ý�Ҫ�ٻ��������ModelID
		INT Lua_GetModelIDbySkillID( LuaPlus::LuaState* state );


		//����¼��ж����
		INT Lua_SouXiaLu_Dismantle_Accept( LuaPlus::LuaState* state );

		//����¼���ٹ���
		INT Lua_SouXiaLu_Destory_Accept( LuaPlus::LuaState* state );

		//����¼��������
		INT Lua_SouXiaLu_Complex_Accept( LuaPlus::LuaState* state );

		//����¼��Ϲ���
		INT Lua_SouXiaLu_Sew_Accept( LuaPlus::LuaState* state );

		INT Lua_SouXia_SetLock( LuaPlus::LuaState* state );
		
		//��������ܿ�����е���������
		INT Lua_Clear_SouXiaSkill( LuaPlus::LuaState* state );

		//���ÿ�������������ܿ�ʹ�õĴ���
		INT Lua_SetMainMenuBarSouXiaSkillNum( LuaPlus::LuaState* state );

		//��õ�ǰ�Ի���NPC��ID
		INT Lua_SouXia_NpcId( LuaPlus::LuaState* state );

	public:
		
		//��������Ƿ�װ��������¼
		BOOL IsEquipSouXia();

		//�ж�һ�������Ƿ���ָ�����͵ļ��������( 0.ȫ��  1.����  2.����  3.����  4.���� )
		BOOL IsSouXiaUsbItem( INT type, INT pos );
		BOOL IsSouXiaUsbItem( INT type, tObject_Item* pItem );

		//�ж�һ�������Ƿ�������¼�����Ʒ( type == 0.����¼����  1.USB�����  2.������  3.��������  4.��ϵ���)
		BOOL IsSouXiaItem( INT pos, INT type);

		//�жϱ����ڵ�һ�������Ƿ��ǿհ׵�����¼����( return true:�ǿհ�  false:���ǿհ� )
		BOOL IsBlankSouXiaItem( INT pos );

		//�ж�һ�������Ƿ�������¼��ҳ
		BOOL IsSouXiadeformityItem( INT pos );
		BOOL IsSouXiadeformityItem( tObject_Item* pItem );

		//�ж�һ������¼��ҳ�Ƿ��ǿհײ�ҳ
		BOOL IsBlankSouXiadeformity( INT pos );
		BOOL IsBlankSouXiadeformity( tObject_Item* pItem );

		//�õ�����¼��ҳ�����ͣ���ú� IsSouXiadeformityItem �������ʹ�ã�
		INT GetSouXiadeformityType( INT pos );
		INT GetSouXiadeformityType( tObject_Item* pItem );

		//�õ�����¼��ҳ����ϸ����
		SOUXIA_DATA* GetSouXiadeformityData( INT pos );
		SOUXIA_DATA* GetSouXiadeformityData( tObject_Item* pItem );

		//�ж�һ�������Ƿ�����������
		BOOL IsSouXiaSkill( SkillID_t nSkillID );

		//���װ��������¼λ���ڴ�ŵ�����¼
		tObject_Item* GetHEquipSouxia();

		//��õ�ǰװ��������¼���ߵ���ϸ����
		SOUXIA_DATA* GetHEquipSouxiaInfo();

		//�������¼��������ߵ����ͣ�return 1,2,3,4 ---- 1.����  2.����  3.����  4.���� ��
		INT GetSouXiaUsbType( tObject_Item* pItem );

		//���ݲ��ϵ����ͺ͵ȼ�����������ϵ�ID����
		INT GetMatIndex( int type, int level);	

		// ����TableID���������ұ������ж��ٴ���Ʒ
		INT GetItemNum_ByIdTable( INT IdTable );

		// ������ұ����ض�λ�õ�USB����������ڴ洢�ľ������Ϣֵ�����ܣ��䷽�����ޣ������ID��
		INT GetSouXiaUsb_Data( INT bagIndex );

		//	��ñ�����������ָ��λ����Ʒ��actionID
		INT GetSouXiaEnumActionId( INT nPosIndex, LPCTSTR szFilter );

		//������Ʒ����
		VOID SetLockSouXiaItem( INT pos, BOOL lock );

		//���¿������ָ����������ID�Ĵ���
		VOID UpdateMainMenuBarSouxiaSkillIDNum( INT SkillId, INT num );

		//����ActionID��ü���ID
		INT GetSkillIdByActionID( INT nActionId );

		//��õ�ǰװ��������¼��ָ�����ܵ��ܵ�ʹ�ô���
		INT GetSouXiaSkillNum( INT SkillId );

		//�����ٻ�����ID��ý�Ҫ�ٻ��������ModelID
		INT GetModelIDbySkillID( INT SkillID );

		//���ÿ�������������ܿ�ʹ�õĴ���
		VOID SetMainMenuBarSouXiaSkillNum( INT ActionId, INT Index );

		//��ʾ������ҳ����
		VOID ShowSouXiaDeformity( tObject_Item* pItem );

		std::vector<INT>	m_vPageUsbID;	//�洢�� ���������з��ϵ�ǰ������USB����������ڱ����е�λ��
		std::map<INT, CObject_Item*> m_ProItem;			//�����������õ�����Ʒ��ÿ����Ʒ���ֻ����һ����Ʒ

	};
}