/********************************************************************
	created:	2008/04/02
	created:	2:4:2008   19:24
	filename: 	MapServer\Server\Obj\Horse\HorseMatingRule.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseMatingRule
	file ext:	h
	author:		Richard
	
	purpose:	处理马交配，繁殖，和后代产程规则
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
//马交配到完成所需时间，小时
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
	// Returns:   BOOL 如果成功交配 TRUE 否则 FALSE
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
	//马领取
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
	//判断2人与NPC的距离必须小于5
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
	//判断是否组队      
	//************************************
	INT IsInTeam( const TeamInfo* pTeamInf, Obj_Human* pMe, Obj_Human* pChar );
	
	
	//************************************
	// Method:    IsSuitabilityMemCount
	// FullName:  HorseMatingRule::IsSuitabilityMemCount
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	//是否组队人数为2
	//************************************
	INT IsSuitabilityMemCount( const TeamInfo* pTeamInf );
	//************************************
	// Method:    IsTeamLeader
	// FullName:  HorseMatingRule::IsTeamLeader
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: const TeamInfo * pTeamInf
	//是否是队长
	//************************************
	INT IsTeamLeader( const TeamInfo* pTeamInf );

	//************************************
	// Method:    IsHeaderHaveEnoughMoney
	// FullName:  HorseMatingRule::IsHeaderHaveEnoughMoney
	// Access:    private 
	// Returns:   INT
	// Qualifier:
	// Parameter: Obj_Human * pHuman
	//队长现金大于N=2金币
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
	//判断两匹参与交配的马是否为可交配
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
	//判断两匹参与交配的马是否为异性
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
	// 主人是否有其它马在交配
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
	//双方骑乘是否都是一代骑乘
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
	//双方骑乘快乐值是否都是100
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
	//双方骑乘的等级差是否在5级之内
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
	//双方骑乘或互为配偶，或双方都无配偶
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
	// 得到队友的Obj实例
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
	//对交配申请进行应答，发送处理结果
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
	//如果条件满足则锁定两匹马，并开始计时，5小时后给玩家发消息取马
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
	
	//后代产生规则
	INT MakingChild( Obj_Human* pMe, Obj_Human* pCh );
	//初始化后代数据
	INT InitChild( _PET_DB* pPetDB, Obj_Human* pHuman );
	//是否符合领回条件
	INT IsCanRetrieveHorse( Obj_Human* pMe, Obj_Human* pCh, Obj_Character* pNPC );
	//是否有至少一个空格
	INT IsHaveEmptySolt( _PET_DB* pMyEmptyDB, _PET_DB* pOrEmptyDB );
	//繁殖是否结束
	INT IsMatingFinished( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//领回条件判断
	INT IsSpouse( _PET_DB* pMyPetDB, _PET_DB* pOrPetDB );
	//获得玩家身上处于交配状态的马
	_PET_DB* GetMatingHorseDB( Obj_Human* pHuman );
	//取得玩家身上空格
	_PET_DB* GetEmptyHorseDB( Obj_Human* pHuman );

};

#endif // HorseMating_h__