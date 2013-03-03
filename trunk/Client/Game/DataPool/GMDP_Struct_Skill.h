
/** ���ݳ����ݽṹ
*/

#pragma once


#include "Type.h"



//-------------------------------------------
//	0)�ķ����ݽṹ
//------------------------------------------// $$
//struct _DBC_SKILL_XINFA;
//struct SCLIENT_XINFA
//{
//	const _DBC_SKILL_XINFA*		m_pDefine;		// �ķ�����
//	INT							m_nPosIndex;	// �ڼ����ķ� (UIͬ��)
//	BOOL						m_bLeaned;		// �Ƿ��Ѿ�ѧ��
//	INT							m_nLevel;		// �ķ��ȼ�
//};
//typedef std::map< INT, SCLIENT_XINFA >			SXINFA_MAP;




//-------------------------------------------
//	1) �������ݽṹ
//-------------------------------------------
// $$
struct _DBC_SKILL_DATA;
//struct _DBC_SKILL_DEPLETE;
struct SCLIENT_SKILL
{
//	const _DBC_SKILLDATA_V1_DEPLETE*	m_pSkillData_V1_Define;		// skillData_v1����
	const _DBC_SKILL_DATA*				m_pDefine;					// ���ܱ���
//	const _DBC_SKILL_DEPLETE*			m_pDeplete;					// ���ܵ�����
	INT									m_nPosIndex;				// �ڼ������� (UIͬ��)
	BOOL								m_bLeaned;					// �Ƿ��Ѿ�ѧ��
	INT									m_nLevel;					// ���ܼ���

	ORESULT IsCanUse( INT idUser, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir ) const;
	ORESULT IsCanUse_Leaned( VOID ) const;
	ORESULT IsCanUse_CheckCoolDown( VOID ) const;
	ORESULT IsCanUse_CheckDeplete( INT idUser ) const;
	ORESULT IsCanUse_CheckTarget( INT idUser, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir ) const;

	LPCTSTR GetSkillDesc() const;				// �õ����ܵ�����

	FLOAT	GetDemageRange(void) const;

	LPCTSTR GetSkillDesc_Interface() const;		// �õ����ܵ����� for ����
	const _DBC_SKILL_DATA* GetSkillData() const;

	mutable std::vector<FLOAT>	m_fDamageRange;
};
typedef std::map< INT, SCLIENT_SKILL >			SSKILL_MAP;




//-------------------------------------------
//	2) �ͻ�����������ݽṹ
//-------------------------------------------
struct _DBC_LIFEABILITY_DEFINE;
struct SCLIENT_LIFEABILITY
{
	const _DBC_LIFEABILITY_DEFINE*	m_pDefine;		// ����ܱ��еĶ���
	INT								m_nPosIndex;	// �ڼ������� (UIͬ��)
	INT								m_nLevel;		// ���ܵȼ�
	INT								m_nExp;			// ����������
};
typedef std::map< INT, SCLIENT_LIFEABILITY >	SLIFEABILITY_MAP;




//-------------------------------------------
//	3) �ͻ����䷽���ݽṹ
//-------------------------------------------
struct _DBC_LIFEABILITY_ITEMCOMPOSE;
struct SCLIENT_PRESCR
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE*		m_pDefine;		// ���䷽���еĶ���
};
typedef std::map< INT, SCLIENT_PRESCR >			SPRESCR_MAP;

