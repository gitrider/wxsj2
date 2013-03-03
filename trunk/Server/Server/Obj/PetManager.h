// PetManager.h
// 
//////////////////////////////////////////////////////

#ifndef __PETMANAGER_H__
#define __PETMANAGER_H__

#include "Type.h"
#include "ObjSingleManager.h"
#include "GameStruct_Pet.h"

class Obj_Pet;
class PetManager : public ObjSingleManager
{
public:
	friend class Obj_Pet;

	PetManager( VOID );
	virtual ~PetManager( VOID );

	virtual BOOL HeartBeat( UINT uTime = 0 );
	
	VOID Cleanup();

	Obj_Pet *GetPet( ObjID_t idObj );

	// ���ļ�Load������Ϣ
	BOOL	LoadPet( const CHAR *pszFileName );
	// ɾ������Pet
	BOOL	RemoveAllPet( VOID );
	// ɾ��ĳ��Pet
	BOOL	RemovePet( ObjID_t idObj );
	// ����һ��Pet
	BOOL	CreatePet( INT nDataID, WORLD_POS* pPos, ObjID_t& rPetID );
	//// ��ʼ��һ��Petģ��
	//BOOL	InitPet(INT idModel, ItemContainer& rPetContainer, INT index);
	// ����һ����׽������GUID
	BOOL	CreateGUIDOfPet(_PET_DB *pPetDB, ObjID_t idHuman, ObjID_t idPet);
	// ����һ����������� --bNeedLevelFluctuate �Ƿ���Ҫ�ȼ����� --bReturnToChild �Ƿ�Ҫ���л�ͯ
	BOOL	MakeCapturePetAttrib(_PET_DB *pPetDB, BOOL bNeedLevelFluctuate=TRUE, BOOL bReturnToChild=FALSE);
	// ͨ��DataIDȥ��ֵһ��PET DB
	BOOL	InitPetDB( _PET_DB *pPetDB, INT nDataID );
	VOID	SetScene( Scene* pScene ){ m_pScene = pScene; }
	VOID	SetInitData( _MONSTER_PET_ATTR& oInit );
	const _MONSTER_PET_ATTR& GetInitData();
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

private:
	Scene*	m_pScene;		
	_MONSTER_PET_ATTR m_oInit;
};

#endif	// __PETMANAGER_H__
