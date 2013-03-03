/********************************************************************
	created:	2008/01/09
	created:	9:1:2008   13:59
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HumanSkillStudy.h
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HumanSkillStudy
	file ext:	h
	author:		Richard
	
	purpose:	���ѧϰ���ܵ�ϵͳ���ģ��˽����ѧϰϵͳ��ȫ����Ϣ��������ϵͳ�ڵ���������

	HumanSkillStudy������Ϊ����Obj_Human::ChangeMenpai�������ع���ԭ���Ǵ˺���������̫�������

	���幦�ܷǳ����ҡ�ͬʱ�Լ�����ӵ���ر仯���з�װ
*********************************************************************/
#include "Type.h"
#include "LogDefine.h"

#ifndef HumanSkillStudy_h__
#define HumanSkillStudy_h__

#include "DataRecords.h"

class Obj_Human;
class Packet;

class HumanSkillStudy
{
public:
	HumanSkillStudy();
	~HumanSkillStudy();
public:
	//************************************
	// Method:    StudySkill
	// FullName:  HumanSkillStudy::StudySkill
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SKILL_LOG_PARAM * pSkillLog
	// Parameter: XINFA_LOG_PARAM * pXinFaLog
	// Parameter: INT iOccupation
	// ��Obj_Humanί�д�����ѧϰ�������Ľ��ܺ���
	//************************************
	VOID StudySkill( SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog, INT Menpai, Obj_Human* pHuman );

private:
	//************************************
	// Method:    SendAttrChangeMessage
	// FullName:  HumanSkillStudy::SendAttrChangeMessage
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: const _XINFA_LIST & rXinFaList
	// Parameter: const _SKILL_LIST & rSkillList
	// Parameter: INT Menpai
	//************************************
	VOID SendAttrChangeMessage( Obj_Human* pHuman, const _XINFA_LIST &rXinFaList, const _SKILL_LIST &rSkillList, INT Menpai );

	//************************************
	// Method:    SendMenPaiMsgToGLServer
	// FullName:  HumanSkillStudy::SendMenPaiMsgToGLServer
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: INT Menpai
	//************************************
	VOID SendMenPaiMsgToGLServer( Obj_Human* pHuman, INT Menpai );

	//************************************
	// Method:    SendSkillMsg
	// FullName:  HumanSkillStudy::SendSkillMsg
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: const _SKILL_LIST & rSkillList
	//************************************
	VOID SendSkillMsg( Obj_Human* pHuman, const _SKILL_LIST &rSkillList );

	//************************************
	// Method:    SendXinFaMsg
	// FullName:  HumanSkillStudy::SendXinFaMsg
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: const _XINFA_LIST & rXinFaList
	//************************************
	VOID SendXinFaMsg( Obj_Human* pHuman, const _XINFA_LIST &rXinFaList );

	//************************************
	// Method:    SendRefeshAttribMsg
	// FullName:  HumanSkillStudy::SendRefeshAttribMsg
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//************************************
	VOID SendRefeshAttribMsg( Obj_Human* pHuman );

	//************************************
	// Method:    SendPacket
	// FullName:  HumanSkillStudy::SendPacket
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Packet * pPacket
	//************************************
	VOID SendPacket( Obj_Human* pHuman, Packet* pPacket );

	//************************************
	// Method:    UpdateHP
	// FullName:  HumanSkillStudy::UpdateHP
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//************************************
	VOID UpdateHP( Obj_Human* pHuman );

	//************************************
	// Method:    UpdateMP
	// FullName:  HumanSkillStudy::UpdateMP
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//************************************
	VOID UpdateMP( Obj_Human* pHuman );
	VOID UpdatePneuma( Obj_Human* pHuman );

	//************************************
	// Method:    FlushItemEffect
	// FullName:  HumanSkillStudy::FlushItemEffect
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//************************************
	VOID FlushItemEffect( Obj_Human* pHuman );

	//************************************
	// Method:    AddSkill
	// FullName:  HumanSkillStudy::AddSkill
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: HumanDB & oDB
	// Parameter: SKILL_LOG_PARAM * pSkillLog
	// Parameter: const _SKILL_LIST & rSkillList
	// Parameter: INT Menpai
	//************************************
	VOID AddSkill( HumanDB &oDB, SKILL_LOG_PARAM* pSkillLog, const _SKILL_LIST &rSkillList, INT Menpai );

	//************************************
	// Method:    InitSkillIDOfDB
	// FullName:  HumanSkillStudy::InitSkillIDOfDB
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: BYTE rCount
	// Parameter: HumanDB & oDB
	//************************************
	VOID InitSkillIDOfDB( BYTE rCount, HumanDB &oDB );
	//************************************
	// Method:    AddSkillToDB
	// FullName:  HumanSkillStudy::AddSkillToDB
	// Access:    public 
	// Returns:   INT
	// Qualifier:
	// Parameter: INT nID
	// Parameter: INT nScaned
	// Parameter: INT Menpai
	// Parameter: HumanDB & oDB
	// Parameter: BYTE rCount
	// Parameter: SKILL_LOG_PARAM * pSkillLog
	//************************************
	INT AddSkillToDB( INT nID, INT nScaned, INT Menpai, HumanDB &oDB, BYTE rCount, SKILL_LOG_PARAM* pSkillLog );

	//************************************
	// Method:    AddSkillWithUserClassCheck
	// FullName:  HumanSkillStudy::AddSkillWithUserClassCheck
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Combat_Module::Skill_Module::SkillTemplateData_T const * pSkillTemplateData
	// Parameter: INT Menpai
	// Parameter: HumanDB & oDB
	// Parameter: INT nID
	// Parameter: BYTE rCount
	// Parameter: SKILL_LOG_PARAM * pSkillLog
	//************************************
	VOID AddSkillWithUserClassCheck( Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData, INT Menpai, HumanDB &oDB, INT nID, BYTE rCount, SKILL_LOG_PARAM* pSkillLog );

	//************************************
	// Method:    AddSkillWithMenpaiCheck
	// FullName:  HumanSkillStudy::AddSkillWithMenpaiCheck
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT Menpai
	// Parameter: Combat_Module::Skill_Module::SkillTemplateData_T const * pSkillTemplateData
	// Parameter: HumanDB & oDB
	// Parameter: INT nID
	// Parameter: BYTE rCount
	// Parameter: SKILL_LOG_PARAM * pSkillLog
	//************************************
	VOID AddSkillWithMenpaiCheck( INT Menpai, Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData, HumanDB &oDB, INT nID, BYTE rCount, SKILL_LOG_PARAM* pSkillLog );

	//************************************
	// Method:    StudyXinFa
	// FullName:  HumanSkillStudy::StudyXinFa
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: HumanDB & oDB
	// Parameter: INT Menpai
	// Parameter: XINFA_LOG_PARAM * pXinFaLog
	// Parameter: const _XINFA_LIST & rXinFaList
	//************************************
	VOID StudyXinFa( HumanDB &oDB, INT Menpai, XINFA_LOG_PARAM* pXinFaLog, const _XINFA_LIST &rXinFaList );

	//************************************
	// Method:    InitXinFaOfDB
	// FullName:  HumanSkillStudy::InitXinFaOfDB
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: BYTE rCount
	// Parameter: HumanDB & oDB
	//************************************
	VOID InitXinFaOfDB( BYTE rCount, HumanDB &oDB );

	//************************************
	// Method:    AddXinFaToDB
	// FullName:  HumanSkillStudy::AddXinFaToDB
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT Menpai
	// Parameter: Combat_Module::Skill_Module::XinFaData_T const * pXinFaData
	// Parameter: HumanDB & oDB
	// Parameter: INT nID
	// Parameter: BYTE rCount
	// Parameter: XINFA_LOG_PARAM * pXinFaLog
	//************************************
	VOID AddXinFaToDB( INT Menpai, Combat_Module::Skill_Module::XinFaData_T const* pXinFaData, HumanDB &oDB, INT nID, BYTE rCount, XINFA_LOG_PARAM* pXinFaLog );
};


#endif // HumanSkillStudy_h__