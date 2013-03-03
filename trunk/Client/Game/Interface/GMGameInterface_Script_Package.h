
/** ��Ұ����ӿ�
 *
 *		080327	ItemLink
 *
 *
 *
 */

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class tObject_Item;

//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{

class PlayerPackage
{
public:

	//��ѯ���������е���Ʒ
	//
	// ������� 
	//		0[String]	- ��������
	//						"base"		������Ʒ
	//						"material"	������Ʒ
	//						"quest"		������Ʒ
	//
	//		1[Int]		- ��������[1, 20]
	//
	// ��������
	//   0 - ActionItem
	//
	INT		EnumItem(LuaPlus::LuaState* state);
	// �򿪰�̯�����ۣ�����
	INT		OpenStallSaleFrame(LuaPlus::LuaState* state);

	//--------------------------------------------------------
	// �����Ʒ����ʹ�õĽӿں���
	INT		GetSplitSum(LuaPlus::LuaState* state);
	// ������ȷ����ֺ�Ĵ���
	INT		SplitItem(LuaPlus::LuaState* state);
	// ȡ����Ʒ���
	INT		CancelSplitItem(LuaPlus::LuaState* state);

	// ������ȷ����׺�Ĵ���
	INT   AddHoleItem(LuaPlus::LuaState* state); 
	// ������ȡ����Ʒ���
	INT   CancelAddHoleItem(LuaPlus::LuaState* state);

	// ������ȷ���󶨺�Ĵ���
	INT 	Bind_Equip(LuaPlus::LuaState* state);
	// ������ȡ����Ʒ��
	INT		CancelBindItem(LuaPlus::LuaState* state);

	// ������ȷ�����Ǻ�Ĵ���
	INT		AddStar_Equip(LuaPlus::LuaState* state);

	// ������ȡ�����Ǻ�Ĵ���
	INT		Cancel_AddStart_Equip(LuaPlus::LuaState* state);

	//ȡ������װ������
	INT		CancelChangeCarditem(LuaPlus::LuaState* state)  ;
	//ȡ������װ������
	INT      CancelSwapCarditem(LuaPlus::LuaState* state)  ;

	// ȡ����Ʒ����
	INT		Cancel_SpecialRepair_Equip(LuaPlus::LuaState* state);
	// ������ȷ����Ʒ����
	INT		SpecialRepair_Equip(LuaPlus::LuaState* state); 

	// ������ȷ������
	INT		SureRepair_Equip(LuaPlus::LuaState* state) ; 
	// ������ȷ���ϳ�װ����ť
	INT		EquipSynthesize_Sure(LuaPlus::LuaState* state);

	// ������ȷ������װ����ť
	INT  ReWorkEquip(LuaPlus::LuaState* state) ;

	//������ȡ������װ����ť
	INT  ReWorkEquipCannel(LuaPlus::LuaState* state); 


	//  ������ȡ�����װ����ť
	INT  EquipDismantleCannel(LuaPlus::LuaState* state)  ;


	//  ������ȷ�����װ����ť
	INT  EquipDismantleSure(LuaPlus::LuaState* state)  ;


	//ȷ������װ������
	INT   SureSwapCarditem(LuaPlus::LuaState* state)  ;

	//ȷ������װ������
	INT  SureChangeCarditem(LuaPlus::LuaState* state) ;

	//������Ʒ
	INT		PackUpPacket(LuaPlus::LuaState* state);

	//��õȴ������Ʒ�ļ۸�
	INT		GetSplitName(LuaPlus::LuaState* state);
	//�򿪳������
	INT		OpenPetList(LuaPlus::LuaState* state);
	//����������
	INT		OpenLockFrame(LuaPlus::LuaState* state);
	//������Ʒ�����
	INT		Lock(LuaPlus::LuaState* state);
	//�����Ʒ�Ƿ�����
	INT		IsLock(LuaPlus::LuaState* state);
	INT     UnLock(LuaPlus::LuaState* state);
	
	//��Ʒ����
	INT		ItemLink(LuaPlus::LuaState* state);

	BOOL	IsSwapItem(tObject_Item* pItem1, tObject_Item* pItem2);

public:
	static PlayerPackage s_PlayerPackage;
	static LuaPlus::LuaObject* s_pMetaTable;
};

};