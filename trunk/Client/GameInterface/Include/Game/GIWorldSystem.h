
/** GIWorldSystem.h
 */

#pragma once


#include "GINode.h"
#include "GIGfxSystem.h"
#include "GISound.h"
#include "GIObjectManager.h"



//======================================================================
/// ��Ϸ�����ӿ�
struct _DBC_SCENE_DEFINE;

struct MAP_POS_DEFINE
{
	fVector3			pos;
	DWORD				dwSceneID;		// ���ڳ���id
	char				name[ 128 ];
	INT					nServerID;
	char				utf8name[128];	//�Ż�����
	bool				opti;
	
	MAP_POS_DEFINE()
	{
		opti = false;
		memset(utf8name,0,128);
	}
	
	void Set_Name(const char* strName);
	void Set_Pos_Name(const char* strName);
	void Set_Pos_Utf8Name(const char* strName);
};

/// �����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������)
typedef bool (__stdcall* FUNC_GETTERRAINHEIGHT)(float x, float z, float& y);

class tScene
{
public:

	/** ȡ�ó������� */
	virtual const _DBC_SCENE_DEFINE* GetSceneDefine(VOID) const  = 0;
	/** ����λ���Ƿ�Ϸ� */
	virtual BOOL IsValidPosition(const fVector2& fvPos) = 0;
	/** �����߼�����ȡ�õ��θ߶ȣ����������棬�����������Ƿ�������FALSE */
	virtual BOOL GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight) = 0;

};

//======================================================================
//��Ϸ����ӿ�
//

enum MINIMAP_TYPE
{	
	MINIMAP_TYPE_ANIMY		= 0,	// ����
	MINIMAP_TYPE_TEAM		= 1,	// ����

	MINIMAP_TYPE_FAMILY		 = 2,	// ����
	MINIMAP_TYPE_GUILD	     = 3,	// ���
	MINIMAP_TYPE_HOMELAND	 = 4,	// ����������	
	MINIMAP_TYPE_NOHOMELAND  = 5,	// �Ǳ���������		

	MINIMAP_TYPE_EXPOBJ			= 6,	// ������
	MINIMAP_TYPE_ACTIVE			= 7,	// ������
	MINIMAP_TYPE_SCENEPOS 		= 8,	// �������͵�
	MINIMAP_TYPE_FLASH			= 9,	// �����

	MINIMAP_TYPE_EXPNPC			 = 10,	// ��ͨnpc
	MINIMAP_TYPE_MISCOMNPC		 = 11,	// ����NPC( ��� )
	MINIMAP_TYPE_MISNOCOMNPC	 = 12,  // ����NPC( δ��� )
	MINIMAP_TYPE_ACCEPTNPC		 = 13,	// ����NPC( �ɽ� )	

	MINIMAP_TYPE_NUM,	

	MINIMAP_TYPE_MYSELF,// ����Լ�
	MINIMAP_TYPE_PET,	// ����
	
};

class KYLIN_API tWorldSystem : public tNode
{
public:

	enum WORLD_STATION
	{
		WS_NOT_ENTER,				//û�н����κγ���
		WS_ENTER_ING,				//���ڽ���һ������(�Ѿ�������CGEnterScene)
		WS_ENTER,					//�ɹ�����һ������
		WS_ASK_CHANGE_SCANE,		//Ҫ���л���һ���µĳ���(�Ѿ�������CGAskChangeScene)
	};	

public:

	/**	����ĳ����
	 *	\param nSceneID	
	 *		����ID/��ҳ���id
	 *	\param nCityLevel
	 *		��ҳ��еȼ������С��0����ʾ����ͨ����!
	 */
	virtual BOOL EnterScene(INT nSceneID, INT nCityLevel, INT nResID)= 0;
	/** ȡ�õ�ǰ����ĳ��� */
	virtual tScene* GetActiveScene(void) = 0;
	/** ȡ�õ�ǰ״̬ */
	virtual WORLD_STATION GetStation(void) const = 0;
	/** �����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������) */
	virtual FUNC_GETTERRAINHEIGHT GetTerrainHeightFunc(void) = 0;
	/** ��ǰ������ServerID */
	virtual INT GetActiveSceneID(void) const = 0;


	virtual std::list< MAP_POS_DEFINE >* GetObjectListByClass( INT nType ) = 0;
	virtual void setMinimapSerachRange( int nRange ) = 0;
	virtual void UpdateMinimapData() = 0;
	virtual void MinimapNeedFresh() = 0;


	virtual fVector3 GetMyselfPos() = 0;
	virtual int GetMyselfDir() = 0;
	virtual const CHAR* GetSceneName( INT nSceneID ) = 0;


	KL_DECLARE_DYNAMIC(tWorldSystem);

};
