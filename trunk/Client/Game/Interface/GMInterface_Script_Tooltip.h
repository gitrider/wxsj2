
/** Tooltips�ӿ�
 */


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CActionItem;

//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{

class SuperTooltips
{
public:

	//��ѯTooltip��Title
	//
	// ������� 
	//   
	// ��������
	//   0[String] - Title
	//
	INT		GetTitle(LuaPlus::LuaState* state);

	//��ѯTooltip��Icon
	//
	// ������� 
	//   
	// ��������
	//   0[String] - Image(set:imageset image:imagename)
	//
	INT		GetIcon(LuaPlus::LuaState* state);

	//��ѯTooltip����ϸ�߼�����
	//
	// ������� 
	//   0[String] - AttributeName
	//   
	// ��������
	//   0[String] - AttributeValue
	//
	INT		GetAttributeValue(LuaPlus::LuaState* state);

	//��ѯTooltip������˵��
	//
	//
	//   
	// ��������
	//   0[String] - �����ַ���
	//
	INT		GetItemDesc(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�ȼ�
	//
	//
	//   
	// ��������
	//   0[int] - -1: �����Ʒû���������
	//
	INT		GetItemLevel(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�;�
	//
	//
	//   
	// ������������
	//   0[int] - ��ǰ�;� -1: �����Ʒû���������
	//   1[int] - ����;� -1: �����Ʒû���������
	//
	INT		GetItemDur(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ���������
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemRepairCount(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�İ���Ϣ
	//
	//
	//   
	// ��������1��
	//   -1: �����Ʒû���������
	//   
	//
	INT		GetItemBindInfo(LuaPlus::LuaState* state);


	//��ѯTooltip����Ʒ�İ���Ϣ
	//
	//
	//   
	// ��������1��
	//   -1: �����Ʒû���������
	//   4��ʾ2�ΰ�
	//

	//��ѯTooltip��װ����2�ΰ���Ϣ
	INT		GetItemSecLevelBindInfo(LuaPlus::LuaState* state);


	//��ѯTooltip����Ʒ�İ󶨼ӳ���Ϣ
	//
	INT	 GetItemBindAddInfo(LuaPlus::LuaState* state);

	//��ѯװ������ɫ

	INT  GetDropVisColor(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ������npc�ļ۸�
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemPrice(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ��ְҵ
	//
	//
	//   
	// ��������1��
	//   -1: �����Ʒû���������
	//   

	INT      GetItemProfession(LuaPlus::LuaState* state);



	//�õ�װ������
 
	INT      GetEquipCard(LuaPlus::LuaState* state);
	//��ѯTooltip�õ���Ʒ�Ƿ�Ψһ
	//
	//
	//   
	// ��������1��
	//   0: ��ʾ�����Ʒ��Ψһ
	//   
	INT		GetItemRuleOnly(LuaPlus::LuaState* state);

	//��ѯTooltip�õ�װ�����п׶�������
	//
	//
	//   
	// ��������1��
	//     ��ʾ���װ���Ŀ���


	//�õ�װ��������


	INT		GetEquipHoleNum(LuaPlus::LuaState* state);

	//�õ�װ����װ��Ϣ

	INT		GetEquipSetInfo(LuaPlus::LuaState* state);

	//�õ�װ�����ͼ������Ϣ

	INT		GetEquipSpritePaiInfo(LuaPlus::LuaState* state);


	//�õ��������װ��С�������Ϣ
	INT		GetOtherEquipSpriteChainInfo(LuaPlus::LuaState* state);


	//�õ�װ��С�������Ϣ
	INT		 GetEquipSpriteChainInfo(LuaPlus::LuaState* state);


	//�õ��������װ���󼤻����Ϣ

	INT		GetOtherEquipSpriteAllActiveInfo(LuaPlus::LuaState* state);

	//�õ�װ���󼤻����Ϣ
	INT		GetEquipSpriteAllActiveInfo(LuaPlus::LuaState* state);

	//�õ�װ����װ��Ϣ

	INT		GetOtherEquipSetInfo(LuaPlus::LuaState* state);


	//�õ�װ����װ�����������Ϣ

	INT		GetEquipSetActiveAttr(LuaPlus::LuaState* state);
	//�õ�װ����װ�����������Ϣ

	INT		GetOtherEquipSetActiveAttr(LuaPlus::LuaState* state);


   //�õ�װ���Ǽ�

	INT		GetStartLevel(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ��������
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemManufacturer(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�İ�ɫ����
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	// INT		GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state);



	//��ѯTooltip����Ʒ�Ļ�������
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//

    INT	GetEquipBaseAttrInfo(LuaPlus::LuaState* state);


	//��ѯTooltip����Ʒ�ĸ�������
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT		GetEquipAddAttrInfo(LuaPlus::LuaState* state);



	// ��ѯTooltip��װ���ļ�������    
	INT	SuperTooltips::GetEquipSkillInfo(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ������
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT	GetItemType(LuaPlus::LuaState* state);


	//��ѯTooltip��װ����Ʒ��(��, ��, ��)
	//
	//
	//   
	// ��������1��
	//   0[int] - 1 : ��
	//            2 : ��
    //            3 : ��
	//INT	GetItemEquipQuantity(LuaPlus::LuaState* state); //remove


	//��ѯTooltip��װ������������
	//
	//
	//   
	// ��������1��
	//   0[int] - ��Ʒ����
	INT	GetItemClassType(LuaPlus::LuaState* state);


	

	//
	// �õ�����������
	// ��������1��
	// 0[int] - ����������
	//INT	GetEquipMaxRepair(LuaPlus::LuaState* state);


	
	//��ѯTooltip��װ���ı�ʯ��Ϣ
	//
	//
	//   
	// ��������15��
	//   0[int] - ��ʯ����
	//   1[str] - ��ʯ1��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 2[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 3[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 4[str] - ��ʯ�������Ե�����
	INT	GetGemInEquipInfo(LuaPlus::LuaState* state);

	INT GetNeedGemInfo(LuaPlus::LuaState* state);
	


	//----------------------------------------------------------------------------------------------------------------------
	// �ķ�����
	//
	
	// �õ��ķ��ȼ�
	// ��������1��
	// 0[int] - ��ǰѧ����ķ��ȼ�.
	INT	GetXinfaLevel(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	// ���ܺ���
	//

	// �õ����ܵȼ�
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	GetSkillLevel(LuaPlus::LuaState* state);

	// �õ��Ƿ�ѧ�Ἴ��
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	GetIsLearnedSkill(LuaPlus::LuaState* state);

	// �õ���������
	// ��������1��
	// 0[str] - �õ���������
	INT	GetSkillDesc(LuaPlus::LuaState* state);

	// �õ����ܶ�Ӧ���ķ��ȼ�
	//
	// ��������1��
	// 0[int] - �ķ��ȼ�
	INT	GetSkillXinfaLevel(LuaPlus::LuaState* state);

	INT	IsPassiveSkill(LuaPlus::LuaState* state);




	//----------------------------------------------------------------------------------------------------------------------
	//
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	// ��������1��
	// 0[int] - ��ʯ�ȼ�
	INT	GetGemLevel(LuaPlus::LuaState* state);

	// �õ���ʯ����
	// ��������1��
	// 0[str] - ��ʯ����
	INT	GetGemArribInfo(LuaPlus::LuaState* state);



	//----------------------------------------------------------------------------------------------------------------------
	//
	// ������Ϣ
	//

	//�õ��������������
	INT	GetFecthHeadInfo_TypeDesc( LuaPlus::LuaState* state );

	//�õ����������ֵ
	INT	GetFecthHeadInfo_Value( LuaPlus::LuaState* state );

	//�õ��������������
	INT	GetFecthHeadInfo_Num( LuaPlus::LuaState* state );


	//----------------------------------------------------------------------------------------------------------------------
	//
	// �õ���Ʒ��quality
	//

	// ��������1��
	// 0[int] - ҩƷ��ʯƷ��
	INT	GetItemQuality(LuaPlus::LuaState* state);

	//�õ�װ���ķ��� ��ͨװ�����Ǹ߼�װ��
	 INT GetItemEquipQua(LuaPlus::LuaState* state);


	// �õ���Ʒ�ڱ��е�����
	// ��������1��
	// 0[int] - ���е����͡�
	INT	GetItemTableType(LuaPlus::LuaState* state);


	
	// �õ��䷽��Ҫ�ļ������֡�
	// ��������1��
	// 0[int] - ���е����͡�
	INT	GetPrescrSkillName(LuaPlus::LuaState* state);

	// �õ��䷽��Ҫ�ļ��ܵȼ���
	// ��������1��
	// 0[int] - ���еĵȼ���
	INT	GetPrescrNeedSkillLevel(LuaPlus::LuaState* state);


	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	// ��������1��
	// 0[int] - �䷽��Ҫ�ĵ�ǰ�ȼ���
	INT	GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state);


	// �����������item����ϸ��Ϣ
	// 
	// 
	INT	SendAskItemInfoMsg(LuaPlus::LuaState* state);

 
	// �õ�װ���Ƿ����
 
	 INT	IsEquipIdentify(LuaPlus::LuaState* state);

	//
	// npc�̵��Ƿ�򿪡�
	// 
	INT	IsNpcShopOpen(LuaPlus::LuaState* state);

	//�Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);

	//�õ�������Ϣ
	INT GetTypeDesc(LuaPlus::LuaState* state);

	//�õ���Ʒ�Ƿ��ڱ�����
	INT IsItemInPacket(LuaPlus::LuaState* state);
	INT GetItemIsOnStall(LuaPlus::LuaState* state);
	INT GetItemPriceOnStall(LuaPlus::LuaState* state);

	INT GetSkillMaxLevel(LuaPlus::LuaState* state);

	INT GetSkillType(LuaPlus::LuaState* state);

	INT GetSkillCoolDown(LuaPlus::LuaState* state);

	INT GetSkillAttackRange(LuaPlus::LuaState* state);

	INT GetSkillGatherTime(LuaPlus::LuaState* state);

	INT GetSkillLeadTime(LuaPlus::LuaState* state);

	INT GetNextLvSkillDesc(LuaPlus::LuaState* state);


	INT GetNextLvSkillGatherTime(LuaPlus::LuaState* state);

	INT GetNextLvSkillLeadTime(LuaPlus::LuaState* state);

	INT GetSkillLvUpLevelQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpPointQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpFahterSkillQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpMoneyQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpXpQua(LuaPlus::LuaState* state);

	INT GetSkillLvUpItemQua(LuaPlus::LuaState* state);

	//�õ��Ƿ���������Ʒ
	INT IsSouXiaItem( LuaPlus::LuaState* state );
	
	//�õ����������ٶ�          [7/21/2010 �¾���]
	INT	GetMountAddtionSpeed(LuaPlus::LuaState* state);

	// �õ��������ʱ��         [7/21/2010 �¾���]
	INT	GetMountLife(LuaPlus::LuaState* state);

	// �õ������ȡʱ��         [7/21/2010 �¾���]
	INT	GetMountObtainDate(LuaPlus::LuaState* state);

	// �õ�����ʣ��ʹ��ʱ��     [7/21/2010 �¾���]
	INT	GetMountRemainTime(LuaPlus::LuaState* state);

	// �õ����ﵱǰ�Ƿ��Ѱ�   [7/21/2010 �¾���]
	INT	GetMountIsBinded(LuaPlus::LuaState* state);

	// �õ����������         [7/21/2010 �¾���]
	BOOL GetMountBindType(LuaPlus::LuaState* state);

public:
	//����Ϸ�߼����ü����е�ActionItem
	VOID	SetActionItem(CActionItem* pActionitem);
	CActionItem* GetActionItem(void) { return m_pActionItem; }

protected:

	INT			 m_nActionId;
	CActionItem* m_pActionItem;

public:
	SuperTooltips();
	virtual ~SuperTooltips();

	static SuperTooltips s_SupperTooltip;
	static SuperTooltips s_SupperTooltip2;
	static LuaPlus::LuaObject* s_pMetaTable;
};




}