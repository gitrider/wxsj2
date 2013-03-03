/*************************************************************************
 �ļ���	: 	AIScript.h
 �汾�� :	0.0.2
 ������	:	FrankWu
 ��  ��	:	������е���չAI�ű���д�Ľ�����ִ����
 �޸ļ�¼:	�޸���ǰ��ӿں�����չ�Բ�ǿ��ȱ��
 *************************************************************************/

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "Type.h"
#include "Ini.h"
#include "AIScriptDef.h"

class MonsterAI;
class AI_Character;
class AIScriptParser;
class AIScriptExecutor;

class AIScript
{
public:

	AIScript();
	~AIScript();

	/** Desc	: ����Pet�Ų��ļ�
	 *  @param	: filename �ű��ļ���
	 *  @return	: �������
	 */
	BOOL			ParseScriptForPet(const CHAR* filename);
	/** Desc	: �������ļ���ȡ�Ľű�����
	 *  @param	: filename �ű��ļ���
	 *  @return	: �������
	 */
	BOOL			ParseScript(const CHAR* filename);
	/** Desc	: ����ű�����ִ�нű�
	 *  @param	: nState ִ���ĸ�Section�Ľű�,�磺ATTACK,APPROACH...
	 *  @return	: ������
	 */
	BOOL			ProcessScript(INT nState, AI_Character* pAI);
	/** Desc	: ���������AI_Character����
	 *  @param	: pAI ��Ҫ�ҽӵ�AI_Character����ָ��
	 *  @return	: ��
	 */
	//VOID			SetAI(AI_Character* pAI) { /*m_pAI = pAI;*/ }
	/** Desc	: ����λm_vStateLists,�����нű����ݸ�λ
	 *  @param	: ��
	 *  @return	: ��
	 */
	VOID			ResetStateList(const AI_Character* pAI);

	/// ���ܴ��������
	//VOID			SetSkillErrorCode(ORESULT oResult) { m_SkillErrorCode = oResult; }
	//ORESULT			GetSkillErrorCode(VOID) const { return m_SkillErrorCode; }

	/////////////////////////////////////////
	//VOID			SetSkillID(INT idSkill) { m_SkillID = idSkill; }
	//INT				GetSkillID(VOID) const { return m_SkillID; }

protected:
	/// �ű�ִ�к���
	BOOL			ExcuteCdtScript(ConditionTree* p, const AI_Character* pAI);
	BOOL			ExcuteToDoScript(TreeNodeList* p, const AI_Character* pAI);
	BOOL			ExcuteExpress(const TreeNode* p, const AI_Character* pAI);
	BOOL			ExcuteToDoExpress(const TreeNode* p, const AI_Character* pAI);
	/// �ű���������
	BOOL			ParseOneScript(const CHAR* szScript);
	INT				ParseCHAR(const CHAR* szch);
	INT				ParseCommand(const CHAR* szch);
	TreeNode		ParseExpress(Atom atom);
	BOOL			VerifyExpress(const CHAR* szch);
	/// �ɽű���Ԥ����ĽŲ����ݽṹ���м亯��
	BOOL			ProcessOptorQ(VOID);
	BOOL			ProcessCdtQ(VOID);
	BOOL			ProcessToDoQ(VOID);
	BOOL			CreateCdtTree(VOID);
	BOOL			CreateCdtNode(VOID);
	BOOL			InsertCdtNodeToCdtList(VOID);
	BOOL			InsertToNodeList(Atom atom,INT flag);
	
	VOID			ClearQ(VOID);
	VOID			ClearList(VOID);
	VOID			ClearTree(ConditionTree* p);
	VOID			ClearNode(ConditionNode* p);
	VOID			ClearCdtList(VOID);
	/// ��������List����غ���
	ConditionList*	GetCdtList(VOID) const { return m_pcdtList; }
	VOID			InitAllStateList(VOID);
	VOID			InitList(ENUM_AISTATE stateIndex) { m_vStateLists[stateIndex] = m_pcdtList; m_pcdtList = NULL; }
	/// �����ĸ�����Ա����
	VOID			VerdictState(ConditionTree* t, ConditionNode* p, INT& ret);
	VOID			SetParam(const CHAR* szCmd, Atom& atom);
	BOOL			IsParam(const CHAR* szCmd);
	VOID			ProcessCtdOrToDoQ(const CHAR* szCmd, BOOL bToDoFlag);
	BOOL			FindParam(const CHAR* szCmd, INT index, CHAR* pString);
	VOID			ReSetTimes(VOID);
	VOID			ReSetFlags(VOID);


private:

	Ini					m_file;
	ConditionList*		m_pcdtList;		//Ϊ�м������Ϊ���ֲ����ṩ����
	ConditionList*		m_pcdtList2;	//������Common����List�Ĵ洢ָ��
	ConditionList*		m_vStateLists[ONDEADSECTION+1];

	OperatorQueue		m_operatorQ;
	CdtExpressionQueue	m_cdtExpQ;
	ToDoExpressionQueue m_todoExpQ;

	TreeNodeList*		m_poptorNodeList;
	TreeNodeList*		m_pcdtNodeList;
	TreeNodeList*		m_ptodoNodeList;

	ConditionTree*		m_pcdtTree;
	ConditionNode*		m_pcdtNode;
	BracketArray		m_array;
	Stack				m_stack;
	INT					m_nID;
	INT					m_nPrev;
	INT					m_fX;
	INT					m_fZ;
	//INT					m_SkillID;
	//INT					m_nSummonCount;
	//INT					m_nSpeakID;
	//FLOAT					m_fNeedGoDist;
	// ���ܴ�����
	//ORESULT				m_SkillErrorCode;

};

#endif