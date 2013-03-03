// BusRule.h
//
// 功能：Bus系统的规则
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
	// 创建与删除
	// bAthwart		:		是否为逆向的路径
	static ObjID_t CreateBusByPatrolPathID(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, INT nPatrolPathID, BOOL bAthwart);
	static ObjID_t CreateBus(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, const WORLD_POS *pPos, FLOAT fDir);
	static BOOL DeleteBus(SceneID_t nSceneID, ObjID_t nBusObjID);

	// 得到车的ObjID
	static ObjID_t GetBusObjIDByGUID(SceneID_t nSceneID, GUID_t uGUID);

	// 乘客上车
	static ORESULT TestAddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);
	static ORESULT AddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);

	// 乘客下车
	static ORESULT TestRemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);
	static ORESULT RemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID);

	// 卸下所有乘客
	static VOID DebusAllPassenger(SceneID_t nSceneID, ObjID_t nBusObjID);

	// 开动
	static VOID StartPatrolPath(SceneID_t nSceneID, ObjID_t nBusObjID);
};

#endif	// __BUSRULE_H__
