// BusRule.h
//
// ���ܣ�Busϵͳ�Ĺ���
// 
///////////////////////////////////////////////////////

#ifndef __BUSRULE_H__
#define __BUSRULE_H__

#include "Type.h"
#include "GameDefine_Result.h"

class Obj_Human;
class BusRule
{
public:
	// ������ɾ��
	// bAthwart		:		�Ƿ�Ϊ�����·��
	static ObjID_t CreateBusByPatrolPathID(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, INT nPatrolPathID, BOOL bAthwart);
	static ObjID_t CreateBus(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, const WORLD_POS *pPos, FLOAT fDir);
	static BOOL DeleteBus(SceneID_t nSceneID, ObjID_t nBusObjID);

	// �õ�����ObjID
	static ObjID_t GetBusObjIDByGUID(SceneID_t nSceneID, GUID_t uGUID);

	// �˿��ϳ�
	static ORESULT TestAddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);
	static ORESULT AddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);

	// �˿��³�
	static ORESULT TestRemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);
	static ORESULT RemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);

	// ж�����г˿�
	static VOID DebusAllPassenger(SceneID_t nSceneID, ObjID_t nBusObjID);

	// ����
	static VOID StartPatrolPath(SceneID_t nSceneID, ObjID_t nBusObjID);
};

#endif	// __BUSRULE_H__
