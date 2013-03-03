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
	// Parameter: const CHAR * pszFileName，场景所属的怪物配置文件的相对路径的文件名
	// 解析指定的怪物配置文件，并根据配置内容，创建怪物
	// 每个map只有唯一的怪物配置文件，并且每个map只会加载怪物一次
	// 当怪物死亡时，此怪物对象并没有被删除，而只是置为死亡状态，此对象会根据复活规则
	// 按照一次性初始化的属性复活
	//************************************
	BOOL LoadMonster( const CHAR *pszFileName );
	BOOL RemoveAllMonster( VOID );
private:
	InitMonsterVisitor m_MonsterVisitor;
};

#endif	// __MONSTERMANAGER_H__
