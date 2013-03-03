// MonsterManager.h
// 
//////////////////////////////////////////////////////

#ifndef __MONSTERMANAGER_H__
#define __MONSTERMANAGER_H__

#include "Type.h"
#include "ObjSingleManager.h"
#include "InitMonsterVisitor.h"
class Obj_Monster;

struct _MONSTER_INIT;
class MonsterManager : public ObjSingleManager
{
public:
	MonsterManager( VOID );
	virtual ~MonsterManager( VOID );

	Obj_Monster *GetMonster( ObjID_t idObj );

	virtual BOOL HeartBeat( UINT uTime = 0 );

	ObjID_t		FindMonsterByGUID( GUID_t guid ) ;

	ObjID_t		FindMonsterByDataID( UINT uDataID ) ;

	BOOL InitMonsterData(_MONSTER_INIT *pInit, INT nDataID);
	InitMonsterVisitor* GetMonsterVisitor()
	{
		return &m_MonsterVisitor;
	}

public:
	//************************************
	// Method:    LoadMonster
	// FullName:  MonsterManager::LoadMonster
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: const CHAR * pszFileName�����������Ĺ��������ļ������·�����ļ���
	// ����ָ���Ĺ��������ļ����������������ݣ���������
	// ÿ��mapֻ��Ψһ�Ĺ��������ļ�������ÿ��mapֻ����ع���һ��
	// ����������ʱ���˹������û�б�ɾ������ֻ����Ϊ����״̬���˶������ݸ������
	// ����һ���Գ�ʼ�������Ը���
	//************************************
	BOOL LoadMonster( const CHAR *pszFileName );
	BOOL RemoveAllMonster( VOID );
private:
	InitMonsterVisitor m_MonsterVisitor;
};

#endif	// __MONSTERMANAGER_H__
