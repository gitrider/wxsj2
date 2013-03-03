/********************************************************************************
 *	�ļ�����	Ability.h
 *	ȫ·����	d:\Prj\Server\Server\Ability\Ability.h
 *	�����ˣ�	����
 *	����ʱ�䣺	
 *
 *	����˵����	����ܵ�ͨ���࣬�洢����
 *	�޸ļ�¼��	
 *				��һЩ�������ص����ݽṹŲ���������������Ϊȫ�ֱ������䷽��
*********************************************************************************/

#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "DB_Struct.h"
#include "Obj_Human.h"

class Ability
{
public:
	Ability();
	~Ability() {};

	VOID						CleanUp();

	enum
	{
		TB_ABILITY_INVALID		= -1,
		TB_ABILITY_ID			= 0,
		TB_ABILITY_NAME,
		TB_ABILITY_LEVEL_DEMAND,
		TB_ABILITY_LEVEL_LIMIT,
		TB_ABILITY_LEVELUP_CONFIG,
		TB_ABILITY_OPERATION_TIME,
		TB_ABILITY_OPERATION_TOOL,
		TB_ABILITY_PLATFORM_DISTANCE,
		TB_ABILITY_PLATFORM,
		TB_ABILITY_OPERATION_ACTION,
	};

	inline	AbilityID_t			AbilityID(); 
	inline	VOID				AbilityID(AbilityID_t id);
	inline	const CHAR*			AbilityName() const;
	inline	VOID				AbilityName(const CHAR* name);
	inline	INT					LevelDemand();
	inline	VOID				LevelDemand(INT lvl);
	inline	INT					AbilityLevelLimit();
	inline	VOID				AbilityLevelLimit(INT lvl);
	inline	INT					LevelUpConfigIndex();
	inline	VOID				LevelUpConfigIndex(INT idx);
	inline	INT					OperationTime();
	inline	VOID				OperationTime(INT time);
	inline	UINT				OperationToolID();
	inline	VOID				OperationToolID(INT id);
	inline	INT					PlatformID();
	inline	VOID				PlatformID(INT id);
	inline	FLOAT				PlatformDistance();
	inline	VOID				PlatformDistance(FLOAT dis);
	inline	UINT				OperationActionID();
	inline	VOID				OperationActionID(INT id);

	/*
	 * �Ƿ���Ҫ����ִ��(�ӿڱȽ���ʱ)
	 */
	virtual
		BOOL					IsAutoRedo(VOID)const{ return FALSE; }

	/*
	 * �ж�����Ƿ����ʹ�ø������
	 * ����ֵ���������
	 */
	virtual
        ORESULT					CanUseAbility(Obj_Human* pHuman);

	/*
	 * ���ʹ������ܹ����б���ϵ��ô˺���
	 */
	virtual
		VOID					OnProcInterrupt(Obj_Human* pHuman);

	/*
	 * ������������꼼�ܺ󣨵��ﴦ�����ʱ�䣩���ô˺���
	 */
	virtual
		ORESULT					OnProcOver(Obj_Human* pHuman){ return OR_OK; }

protected:
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman){ return OR_OK; }
	
	virtual
		ORESULT					OnProcFailure(Obj_Human* pHuman){ return OR_ERROR; }

protected:
	AbilityID_t					m_AbilityID;			// ���� ID
	const CHAR*					m_AbilityName;			// ��������
	INT							m_LevelDemand;			// ����ѧϰ����Ҫ�������ҵȼ�
	INT							m_AbilityLevelLimit;	// ��������ѧ������������߼���С�� 1 ��ʾû������
	INT							m_LevelUpConfigIndex;	// �������õ���Ϣ��AbilityLevelUpConfigManager�е�����
	INT							m_OperationTime;		// ����ʱ�䣨ms��
	UINT						m_OperationToolID;		// �������蹤�ߵ� ID���� INVALID_ID ��ʾ����Ҫ����
	INT							m_PlatformID;			// ��������ĸ���ƽ̨�����������Ҫ¯����ҩ��Ҫ��¯�ȵȣ�
	FLOAT						m_PlatformDistance;		// ��� m_PlatformID ���� INVALID_ID���������ж�ƽ̨���ڵķ�Χ�뾶
	UINT						m_OperationActionID;	// �������� ID

	BOOL						m_BindFlag;             //�߼��ϳ��е���Ʒ��״̬
	INT							m_BagIndex;             //�߼��ϳ���������Ʒ�İ�λ��

	
};

#define MAX_PRESCRIPTION_STUFF	5
//ҩ������,�ϳɼ�����
struct _DBC_PRESCRIPTION_DATA
{
	PrescriptionID_t			m_PrescriptionID;
	UINT						m_CompoundID;
	BYTE						m_CompoundNum;
	INT							m_CompoundQuality;
	AbilityID_t					m_AbilityID;
	INT							m_AbilityLevel;
	UINT						m_Stuff_ID[MAX_PRESCRIPTION_STUFF];
	BYTE						m_Stuff_Num[MAX_PRESCRIPTION_STUFF];
	INT							m_Attr_Vigor;
	INT							m_Attr_Energy;
	INT							m_Attr_Reserved;
	INT							m_Attr_Reserved2;
	INT							m_ColddownTime;
	UINT						m_ToolID;
	CooldownID_t				m_ColddownID;
	INT							m_ExpRequired;
	INT							m_ExpIncrement;
	INT							m_nOperaTime;
	INT							m_ScriptID;
	UINT						m_ReplicaID;
	INT							m_nSpecStuffSubClass;	// ������ϵ���Ʒ���ࣨFrom CommonItem.txt��������д�����

	enum
	{
		TB_PrescriptionID		= 0,	// �䷽ ID
		TB_CompoundID			= 2,	// �ϳ��� ID
		TB_CompoundNum,					// �ϳ�������(ͨ��ֻ�пɵ�����������ֵ)
		TB_AbilityID,					// ʹ�ø��䷽��Ҫ�ļ��� ID
		TB_AbilityLevel,				// ʹ�ø��䷽��Ҫ�ļ��ܵȼ�
		TB_Stuff_BEGIN,					// ����Ʒ����ԭ�� ID
		TB_Stuff_END			= TB_Stuff_BEGIN + MAX_PRESCRIPTION_STUFF*2 - 1, // ����Ʒ����ԭ������
		TB_Attr_VIGOR,					// ʹ�ø��䷽��Ҫ����Ļ���ֵ
		TB_Attr_ENERGY,					// ʹ�ø��䷽��Ҫ����ľ���ֵ
		TB_Attr_RESERVED,				// ʹ�ø��䷽��Ҫ��������ɹ��׶�ֵ
		TB_Attr_RESERVED2,				// ʹ�ø��䷽��Ҫ���������ֵ
		TB_ColddownTime,				// ʹ�ø��䷽��Ҫ����ȴʱ��
		TB_ToolID,						// ʹ�ø��䷽��Ҫ�Ĺ��� ID����һ���䷽��Ҫ���ֹ���ʱ���Ժͼ�����Ҫ�Ĺ��ߵ���ʹ��
		TB_ColddownID,					// ��ȴ�� ID
		TB_ExpRequired,					// �������䷽������������
		TB_ExpIncrement,				// �������䷽���ܻ�õ�������
		TB_OperaTime			= TB_ExpIncrement + 2,					// �����䷽����ʱ��
		TB_SCRIPTID,					// �ű� ID����ͨ��ͨ�ã��䷽��Ҫָ��һ���ű�
		TB_REPLICAID,					// �ϳ�ʧ��ʱ��������
		TB_CompoundDesc,				// ��ϸ����
		TB_CompoundQuality,				// �ϳ�������
		TB_SubClass,					// ������ϵ���Ʒ���ࣨFrom CommonItem.txt��������д�����
	};
};

typedef _DBC_PRESCRIPTION_DATA _PRESCRIPTION_DATA;

struct _ABILITY_PRESCRIPTION_TBL
{
	_PRESCRIPTION_DATA			m_Table[MAX_ABILITY_PRESCRIPTION_NUM];
	INT							m_Count;

	_ABILITY_PRESCRIPTION_TBL()
	{
		memset(m_Table,0,sizeof(_PRESCRIPTION_DATA)*MAX_ABILITY_PRESCRIPTION_NUM);
		m_Count	=	0;
	}

	const _PRESCRIPTION_DATA* Get_Prescription(PrescriptionID_t id) const
	{
		INT idx;
		idx = (INT)id;
		Assert(idx>=0 && id<m_Count);
		return &m_Table[idx];
	}

};

//����ܾ���� 
struct _AbilityExpTable
{
	const CHAR*					m_AbilityObjectName;	// ���ܶ�������
	INT							m_AbilityBaseExp;		// ����������
	INT							m_AbilityLevel;			// ���ܵȼ�
	INT							m_AbilityCostPercent;	// ��ı���
	AbilityID_t					m_AbilityId;			// ���� ID
	INT							m_AbilityItemTypeSn;    // ���ܶ����ItemType

	enum ETableElement
	{
		ETE_ObjectName,
		ETE_BaseExp,
		ETE_CostPercent,
		ETE_Level,
		ETE_AbilityId,
		ETE_ItemTypeSn,
		ETE_ElementNum = 6
	};

};

#include "Ability.inl"

#endif // __ABILITY_H__
