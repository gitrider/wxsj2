#pragma once

#include "../DataPool/GMDP_Struct_Pet.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CObject_Character;

namespace SCRIPT_SANDBOX
{
	class Mount
	{
	//LuaPlus������ؽӿ�
	public:
		// ��ѯ �������
		INT		GetMountName(LuaPlus::LuaState* state);

		// ��ѯ ����ǳ�
		INT		GetMountNickName(LuaPlus::LuaState* state);

		// ��ѯ ��輶��
		INT		GetMountLevel(LuaPlus::LuaState* state);

		// ��ѯ ����Ա�
		INT		GetMountGender(LuaPlus::LuaState* state);

		// ��ѯ ������
		INT		GetMountEraCount(LuaPlus::LuaState* state);

		// ��ѯ ����Ƿ�ɽ���
		INT		IsCanBeMating(LuaPlus::LuaState* state);

		// ��ѯ ���ID
		INT		GetMountGUID(LuaPlus::LuaState* state);

		// ��ѯ �����żID
		INT		GetSpouseID(LuaPlus::LuaState* state);

		// ��ѯ ����б�
		INT		GetMountList(LuaPlus::LuaState* state);

		// ��ѯ ������ֵ
		INT		GetMountHappiness(LuaPlus::LuaState* state);

		// ��ѯ ���ɳ���
		INT		GetMountGrowRate(LuaPlus::LuaState* state);

		// ��ѯ ������
		INT		GetMountBasic(LuaPlus::LuaState* state);

		// ��ѯ �������
		INT		GetMountSavvy(LuaPlus::LuaState* state);

		// ��ѯ �������
		INT		GetMountStr(LuaPlus::LuaState* state);

		// ��ѯ �������
		INT		GetMountDex(LuaPlus::LuaState* state);

		// ��ѯ �������
		INT		GetMountCon(LuaPlus::LuaState* state);

		// ��ѯ �������
		INT		GetMountInt(LuaPlus::LuaState* state);

		// ��ѯ ���Ǳ��
		INT		GetMountPotential(LuaPlus::LuaState* state);

		// ��ѯ �����������
		INT		GetMountStrApt(LuaPlus::LuaState* state);

		// ��ѯ �����������
		INT		GetMountDexApt(LuaPlus::LuaState* state);

		// ��ѯ �����������
		INT		GetMountConApt(LuaPlus::LuaState* state);

		// ��ѯ �����������
		INT		GetMountIntApt(LuaPlus::LuaState* state);		

		// ��ѯ ���UIģ������
		INT		GetUIModelName(LuaPlus::LuaState* state);
		
		// ��ѯ ����UIģ������
	    INT     GetMountUIModelName(LuaPlus::LuaState* state);

		// ��ѯ ��輼��
		INT		GetMountSkill(LuaPlus::LuaState* state);

		// ��ѯ ����Item��Action
    	INT     GetMountAction(LuaPlus::LuaState* state);

		// �ӵ�ȷ��
		INT		SetAttrAddRequest(LuaPlus::LuaState* state);

		// �Ƿ������
		INT		IsMounting(LuaPlus::LuaState* state);

		// ����
		INT		Mounting(LuaPlus::LuaState* state);
        
		// ����
		INT		Dismount(LuaPlus::LuaState* state);

		// ����
		INT		FreeMount(LuaPlus::LuaState* state);

		// �ٻ�����  [7/14/2010 �¾���]
		INT     CallMount(LuaPlus::LuaState* state);

	    // �ٻ�����  [7/14/2010 �¾���]
	    INT     ReCallMount(LuaPlus::LuaState* state);

		// ��������  [7/14/2010 �¾���]
	    INT     DiscardMount(LuaPlus::LuaState* state);

		// �Ƿ�ɷ�ֳ
		INT		CanMating(LuaPlus::LuaState* state);

		// ��ѯ ����״̬
		INT		IsLocked(LuaPlus::LuaState* state);
		



		// Ŀ���������
		INT		GetTargetMountName(LuaPlus::LuaState* state);

		// Ŀ�������ǳ�
		INT		GetTargetMountNickName(LuaPlus::LuaState* state);

		// Ŀ����輶��
		INT		GetTargetMountLevel(LuaPlus::LuaState* state);

		// Ŀ������Ա�
		INT		GetTargetMountGender(LuaPlus::LuaState* state);

		// Ŀ�����ID
		INT		GetTargetMountID(LuaPlus::LuaState* state);

		// Ŀ����żID
		INT		GetTargetSpouseID(LuaPlus::LuaState* state);

		// Ŀ������
		INT		GetTargetMountStr(LuaPlus::LuaState* state);

		// Ŀ������
		INT		GetTargetMountCon(LuaPlus::LuaState* state);

		// Ŀ������
		INT		GetTargetMountDex(LuaPlus::LuaState* state);

		// Ŀ������
		INT		GetTargetMountInt(LuaPlus::LuaState* state);

		// Ŀ����������
		INT		GetTargetMountStrApt(LuaPlus::LuaState* state);

		// Ŀ����������
		INT		GetTargetMountDexApt(LuaPlus::LuaState* state);

		// Ŀ����������
		INT		GetTargetMountConApt(LuaPlus::LuaState* state);

		// Ŀ����������
		INT		GetTargetMountIntApt(LuaPlus::LuaState* state);

		// Ŀ�����
		INT		GetTargetMountBasic(LuaPlus::LuaState* state);

		// Ŀ������
		INT		GetTargetMountSavvy(LuaPlus::LuaState* state);

		// Ŀ��ɳ���
		INT		GetTargetMountGrowRate(LuaPlus::LuaState* state);

		// Ŀ��UIģ����
		INT		GetTargetUIModelName(LuaPlus::LuaState* state);

		// ����Ŀ���������
		INT		TargetMountAttrRequest(LuaPlus::LuaState* state);

		// Ŀ�꼼��
		INT		GetTargetMountSkill(LuaPlus::LuaState* state);




		// �õ� ��ǰ��ս������
		INT		GetBattleMountIndex(LuaPlus::LuaState* state);
        
		// �õ� ��ǰUIѡ������
		INT		GetSelectMount(LuaPlus::LuaState* state);

		// ���� ��ǰUIѡ������
		INT		SetSelectMount(LuaPlus::LuaState* state);

		// �����Ʒ����״̬
		INT		ItemUnLock(LuaPlus::LuaState* state);

		// ʹ����Ʒ
		INT		UseMountItem(LuaPlus::LuaState* state);

		// ����ı����������
		INT		ChangeMountName(LuaPlus::LuaState* state);

		// �������ɳ���
		INT		RequestIdentifyMount(LuaPlus::LuaState* state);

		// ���ͽ��������������Ϣ
		INT		RequestMyMountMatingLocked(LuaPlus::LuaState* state);

		// ���ʹ򿪷�ֳ������Ϣ
		INT		RequestTogleMatingPage(LuaPlus::LuaState* state);

		// ����ȷ�ϻ�ȡ��������Ϣ
		INT		RequestMating(LuaPlus::LuaState* state);

		// ������ȡ��ֳ������Ϣ
		INT		RequestGainMatingMount(LuaPlus::LuaState* state);



	protected:
		// ��ǰUI�б���ѡ������
		PET_GUID_t		m_nSelectMount;
		

	public:
		Mount(void){}
		~Mount(void){}

		static Mount s_Mount;
		static LuaPlus::LuaObject* s_pMetaTable;

		
	
	};
}

