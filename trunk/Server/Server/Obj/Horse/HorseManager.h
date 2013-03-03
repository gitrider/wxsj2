/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   14:43
	filename: 	MapServer\Server\Obj\Horse\HorseManager.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseManager
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef HorseManager_h__
#define HorseManager_h__

#include "Type.h"
#include "ObjSingleManager.h"

class Obj_Horse;
class HorseManager : public ObjSingleManager
{
public:
	friend class Obj_Horse;

	HorseManager( VOID );
	virtual ~HorseManager( VOID );

	VOID Cleanup();

	virtual BOOL HeartBeat( UINT uTime = 0 );


	Obj_Horse *GetHorse( ObjID_t idObj );

	// ���ļ�Load������Ϣ
	BOOL	LoadHorse( const CHAR *pszFileName );
	// ɾ������Pet
	BOOL	RemoveAllHorse( VOID );
	// ɾ��ĳ��Pet
	BOOL	RemoveHorse( ObjID_t idObj );
	// ����һ��Pet
	BOOL	CreateHorse( INT nDataID, WORLD_POS* pPos, ObjID_t& rPetID );
	//// ��ʼ��һ��Petģ��
	//BOOL	InitPet(INT idModel, ItemContainer& rPetContainer, INT index);
	// ����һ����׽������GUID
	BOOL	CreateGUIDOfHorse(_PET_DB *pPetDB, ObjID_t idHuman, ObjID_t idPet);
	// ����һ����������� --bNeedLevelFluctuate �Ƿ���Ҫ�ȼ����� --bReturnToChild �Ƿ�Ҫ���л�ͯ
	BOOL	MakeCapturePetAttrib(_PET_DB *pPetDB, BOOL bNeedLevelFluctuate=TRUE, BOOL bReturnToChild=FALSE);
	// ͨ��DataIDȥ��ֵһ��PET DB
	BOOL	InitHorseDB( _PET_DB *pPetDB, INT nDataID );
	VOID	SetScene( Scene* pScene ){ m_pScene = pScene; }
protected:
	// ���ɳ�����Ը�����
	VOID		CreateAITypeOfPet(_PET_DB& rPetDB);
	// ��ó�������
	VOID		CreateTypeOfPet(_PET_DB& rPetDB, BOOL bNeedLevelFluctuate, BOOL bReturnToChild);
	// ��ó������Ե㣬 idDataΪ����ı�Ų���ObjID
    VOID		CreateRemainPointOfPet(_PET_DB& rPetDB);
	// ��ó��＼�ܣ�idDataΪ����ı�Ų���ObjID
    VOID		CreateSkillOfPet(_PET_DB& rPetDB);
	// ��ó���ɳ��ʣ�idDataΪ����ı�Ų���ObjID
	VOID		CreateGrowRateOfPet(_PET_DB& rPetDB);
	// ��ó������ʣ�idDataΪ����ı�Ų���ObjID
	VOID		CreatePerceptionOfPet(_PET_DB& rPetDB);
	// ����function
	VOID		AddPetSkill(_PET_SKILL *paSkillList, INT& rSkillIndex);

	INT			GetRand( INT iMin, INT iMax );

	VOID		DeleteDeadHorse( INT nRemoveCount, INT * aNeedRemovePet );
	VOID		ClearRemoveArray( INT* aNeedRemovePet );

private:
	Scene*	m_pScene;		

};

#endif // HorseManager_h__
