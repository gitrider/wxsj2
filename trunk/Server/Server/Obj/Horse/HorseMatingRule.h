/********************************************************************
	created:	2008/04/02
	created:	2:4:2008   19:24
	filename: 	MapServer\Server\Obj\Horse\HorseMatingRule.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseMatingRule
	file ext:	h
	author:		Richard
	
	purpose:	�������䣬��ֳ���ͺ�����̹���
*********************************************************************/

#ifndef HorseMatingRule_h__
#define HorseMatingRule_h__

#include "Type.h"
#include "Team.h"
#include "DB_Struct.h"
//class TeamInf;
class Obj_Character;
class Obj_Human;
class Item;

const UINT HorseMatingFinishTimeInHour = 5;
//���䵽�������ʱ�䣬Сʱ
const UINT HorseMatingFinishTime = 60;//HorseMatingFinishTimeInHour * 3600;
const UINT HorseMatingDeleteTime = HorseMatingFinishTime*10;//48 * 3600 + HorseMatingFinishTime;

class HorseMatingRule
{
public:
	HorseMatingRule();
	~HorseMatingRule();

	//************************************
	// Method:    AskHorseMating
	// FullName:  HorseMatingRule::AskHorseMating
	// Access:    public 
	// Returns:   BOOL ����ɹ����� TRUE ���� FALSE
	// Qualifier:
	// Parameter: const Obj_Character * pMe
	// Parameter: const Obj_Character * pChar
	// Parameter: const Obj_Character * pNPC
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: PET_GUID_t guidOtherHorse
	//************************************
	BOOL AskHorseMating
		( 
		Obj_Character* pMe, 
		Obj_Character* pChar, 
		const Obj_Character* pNPC, 
		PET_GUID_t guidMyHorse,
		PET_GUID_t guidOtherHorse
		);
	//************************************
	// Method:    RetrieveHorse
	// FullName:  HorseMatingRule::RetrieveHorse
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pMe
	// Parameter: Obj_Human * pCh
	// Parameter: Obj_Character * pNPC
	//����ȡ
	//************************************
	VOID RetrieveHorse( Obj_Human* pMe, Obj_Human* pCh, Obj_Character* pNPC );

	//************************************
	// Method:    IsTeamInArea
	// FullName:  HorseMatingRule::IsTeamInArea
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Obj_Human * pOther
	// Parameter: const Obj_Character * pNPC
	// Parameter: const TeamInfo * pTeamInf
	//�ж�2����NPC�ľ������С��5
	//************************************
	INT IsTeamInArea
		( 
		Obj_Human* pHuman, 
		Obj_Human* pOther, 
		const Obj_Character* pNPC, 
		const TeamInfo* pTeamInf 
		);
private:
	//************************************
	// Method:    HorseMatingImp
	// FullName:  HorseMatingRule::HorseMatingImp
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pMe
	// Parameter: Obj_Human * pChar
	// Parameter: const Obj_Character * pNPC
	// Parameter: const TeamInfo * pTeamInf
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: PET_GUID_t guidOtherHorse
	//************************************
	INT HorseMatingImp
		( 
		Obj_Human* pMe, 
		Obj_Human* pChar,
		const Obj_Character* pNPC,
		const TeamInfo* pTeamInf,
		PET_GUID_t guidMyHorse,
		PET_GUID_t guidOtherHorse
		);

	//************************************
	// Method:    IsInTeam
	// FullName:  HorseMatingRule::IsInTeam
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	// Parameter: Obj_Human * pMe
	// Parameter: Obj_Human * pChar
	//�ж��Ƿ����      
	//************************************
	INT IsInTeam( const TeamInfo* pTeamInf, Obj_Human* pMe, Obj_Human* pChar );
	
	
	//************************************
	// Method:    IsSuitabilityMemCount
	// FullName:  HorseMatingRule::IsSuitabilityMemCount
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	//�Ƿ��������Ϊ2
	//************************************
	INT IsSuitabilityMemCount( const TeamInfo* pTeamInf );
	//************************************
	// Method:    IsTeamLeader
	// FullName:  HorseMatingRule::IsTeamLeader
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	//�Ƿ��Ƕӳ�
	//************************************
	INT IsTeamLeader( const TeamInfo* pTeamInf );

	//************************************
	// Method:    IsHeaderHaveEnoughMoney
	// FullName:  HorseMatingRule::IsHeaderHaveEnoughMoney
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//�ӳ��ֽ����N=2���
	//************************************
	INT IsHeaderHaveEnoughMoney( Obj_Human* pHuman );
	INT SpendMoney( Obj_Human* pHuman );
	//************************************
	// Method:    IsBothHorseCanMating
	// FullName:  HorseMatingRule::IsBothHorseCanMating
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Obj_Human * pOther
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: PET_GUID_t guidOtherHorse
	//�ж���ƥ���뽻������Ƿ�Ϊ�ɽ���
	//************************************
	INT IsBothHorseCanMating
		( 
		Obj_Human* pHuman, 
		Obj_Human* pOther,	
		PET_GUID_t guidMyHorse,
		PET_GUID_t guidOtherHorse
		);

	//************************************
	// Method:    IsEnoughLevel
	// FullName:  HorseMatingRule::IsEnoughLevel
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Item * pMyHorse
	// Parameter: Item * pOrHorse
	//************************************
	INT IsEnoughLevel( Item* pMyHorse, Item* pOrHorse );
	//************************************
	// Method:    IsCallup
	// FullName:  HorseMatingRule::IsCallup
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: Obj_Human * pOther
	// Parameter: PET_GUID_t guidOtherHorse
	//************************************
	INT IsCallup( Obj_Human* pHuman, PET_GUID_t guidMyHorse, Obj_Human* pOther, PET_GUID_t guidOtherHorse );
	//************************************
	// Method:    IsTwoHorseOppositeSex
	// FullName:  HorseMatingRule::IsTwoHorseOppositeSex
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Obj_Human * pOther
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: PET_GUID_t guidOtherHorse
	//�ж���ƥ���뽻������Ƿ�Ϊ����
	//************************************
	INT IsTwoHorseOppositeSex
		( 
		Obj_Human* pHuman, 
		Obj_Human* pOther,	
		PET_GUID_t guidMyHorse,
		PET_GUID_t guidOtherHorse
		);

	//************************************
	// Method:    IsEnoughMatingLevelup
	// FullName:  HorseMatingRule::IsEnoughMatingLevelup
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//************************************
	INT IsEnoughMatingLevelup( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );

	//************************************
	// Method:    IsGeneration
	// FullName:  HorseMatingRule::IsGeneration
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//************************************
	INT IsGeneration( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//************************************
	// Method:    IsVariation
	// FullName:  HorseMatingRule::IsVariation
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//************************************
	INT IsVariation( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	INT IsCanMating( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//************************************
	// Method:    IsAnyHorseMating
	// FullName:  HorseMatingRule::IsAnyHorseMating
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Obj_Human * pOther
	// �����Ƿ����������ڽ���
	//************************************
	INT IsAnyHorseMating( Obj_Human* pHuman, Obj_Human* pOther );
	//************************************
	// Method:    IsBothHorseGeneration
	// FullName:  HorseMatingRule::IsBothHorseGeneration
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//˫������Ƿ���һ�����
	//************************************
	INT IsBothHorseGeneration( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );

	//************************************
	// Method:    IsBothHorseHappinesEnough
	// FullName:  HorseMatingRule::IsBothHorseHappinesEnough
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//˫����˿���ֵ�Ƿ���100
	//************************************
	INT IsBothHorseHappinesEnough( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );

	//************************************
	// Method:    IsLevelSuitability
	// FullName:  HorseMatingRule::IsLevelSuitability
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//˫����˵ĵȼ����Ƿ���5��֮��
	//************************************
	INT IsLevelSuitability( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );

	//************************************
	// Method:    IsSpoused
	// FullName:  HorseMatingRule::IsSpoused
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: _PET_DB * pMyPetDB
	// Parameter: _PET_DB * pOrPetDB
	//˫����˻�Ϊ��ż����˫��������ż
	//************************************
	INT IsSpoused( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );

	//************************************
	// Method:    GetTeammate
	// FullName:  HorseMatingRule::GetTeammate
	// Access:    private 
	// Returns:   Obj_Character*
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	// Parameter: Obj_Character * pHuman
	// �õ����ѵ�Objʵ��
	//************************************
	Obj_Character* GetTeammate( const TeamInfo* pTeamInf, Obj_Character* pHuman );

	//************************************
	// Method:    SendMatingResult
	// FullName:  HorseMatingRule::SendMatingResult
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	// Parameter: Obj_Human * pOther
	// Parameter: INT iResult
	//�Խ����������Ӧ�𣬷��ʹ�����
	//************************************
	VOID SendMatingResult( Obj_Human* pHuman, Obj_Human* pOther, INT iResult );

	VOID SendHorseMatingSuccessMSG( Obj_Human* pHuman );
	VOID SendHorseRetrieveSuccessMSG( Obj_Human* pHuman );

	//************************************
	// Method:    StartMating
	// FullName:  HorseMatingRule::StartMating
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: Obj_Human * pMe
	// Parameter: Obj_Human * pCh
	// Parameter: PET_GUID_t guidMyHorse
	// Parameter: PET_GUID_t guidOtherHorse
	//�������������������ƥ������ʼ��ʱ��5Сʱ�����ҷ���Ϣȡ��
	//************************************
	VOID StartMating
		( 
		Obj_Human* pMe, 
		Obj_Human* pCh,
		PET_GUID_t guidMyHorse,
		PET_GUID_t guidOtherHorse
		);

	//************************************
	// Method:    IsBothHuman
	// FullName:  HorseMatingRule::IsBothHuman
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: Obj_Human * pMe
	// Parameter: Obj_Character * pChar
	//************************************
	BOOL IsBothHuman( Obj_Human* pMe, Obj_Character* pChar );

	//************************************
	// Method:    GetTeamInfo
	// FullName:  HorseMatingRule::GetTeamInfo
	// Access:    private 
	// Returns:   TeamInfo*
	// Qualifier:
	// Parameter: Obj_Human * pMe
	//************************************
	TeamInfo* GetTeamInfo( Obj_Human* pMe );

	//************************************
	// Method:    IsHuman
	// FullName:  HorseMatingRule::IsHuman
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: Obj_Character * pChar
	//************************************
	BOOL IsHuman( Obj_Character* pChar );
	
	//�����������
	INT MakingChild( Obj_Human* pMe, Obj_Human* pCh );
	//��ʼ���������
	INT InitChild( _PET_DB* pPetDB, Obj_Human* pHuman );
	//�Ƿ�����������
	INT IsCanRetrieveHorse( Obj_Human* pMe, Obj_Human* pCh, Obj_Character* pNPC );
	//�Ƿ�������һ���ո�
	INT IsHaveEmptySolt( _PET_DB* pMyEmptyDB, _PET_DB* pOrEmptyDB );
	//��ֳ�Ƿ����
	INT IsMatingFinished( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//��������ж�
	INT IsSpouse( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//���������ϴ��ڽ���״̬����
	_PET_DB* GetMatingHorseDB( Obj_Human* pHuman );
	//ȡ��������Ͽո�
	_PET_DB* GetEmptyHorseDB( Obj_Human* pHuman );

};

#endif // HorseMating_h__