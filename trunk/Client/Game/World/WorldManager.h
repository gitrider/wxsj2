
/** ��Ϸ����
 */

#pragma once


#include "GIWorldSystem.h"
#include "Scene.h"
#include "GIEventSystem.h"



class CObject_PlayerOther;

class CActivePosManager
{
public:

	CActivePosManager();
	virtual ~CActivePosManager();

	static CActivePosManager* GetMe(VOID) { return s_pMe; }

	void   Initial();
	void   Release();

	void   AddActivePos( bool bOnNPC, const char* pPosName, float fX, float fY, int nSceneID );
	void   AddFlashPos( bool bOnNPC, const char* pPosName, float fX, float fY, int nSceneID );

	void   update();
	
	
protected:

	static CActivePosManager*		s_pMe;
	typedef std::list< MAP_POS_DEFINE >   POS_LIST;
	POS_LIST   m_listActiveObj;
	POS_LIST   m_listFlashObj;

	int			m_nDistance;
public:

	void   GetActiveList( int nSceneID, POS_LIST* pReturnList );
	void   GetFlashList( int nSceneID, POS_LIST* pReturnList );

private:

	void   update( POS_LIST* pList );

};

class CWorldManager : public tWorldSystem
{
public:

	/**
		����ĳ����
		\param nSceneID	
			����ID/��ҳ���id

		\param nCityLevel
			��ҳ��еȼ������С��0����ʾ����ͨ����!
	*/
	virtual BOOL							EnterScene(INT nSceneID, INT nCityLevel, INT nResID);
	//������ҳ���
	virtual BOOL							UpdateUserCity(int nNewLevel);
	///ȡ�õ�ǰ����ĳ���
	virtual tScene*							GetActiveScene(VOID) { return (tScene*)m_pActiveScene; }
	///��Ϸ�������л�����
	virtual VOID							ChangeScene(INT idTargetScene, INT idTargetSceneResID, const fVector2& fvPos, INT nDirection);
	///ȡ�õ�ǰ״̬
	virtual WORLD_STATION					GetStation(VOID) const		{ return m_Station; }
	//�����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������)
	virtual FUNC_GETTERRAINHEIGHT			GetTerrainHeightFunc(void) { return _GetTerrainHeight; }
	//��ǰ������ServerID ��ԴID
	virtual INT								GetActiveSceneID(void) const;

	//��ǰ������ServerID ʵ��ID
	INT										GetActiveDataSceneID(void) const {return m_nSceneID;}
	

public:
	//---- for debug
	//ȡ�ü���Ҫȥ�ĳ���ID
	INT		GetNextSenceID(VOID) const { return m_idNextScene; }
	fVector2	GetNextScenePos(VOID) const { return m_fvNextPos; }			
	FLOAT		GetNextSceneFaceDir(VOID) const { return m_fNextDir; }

public:
	//�ڵ��ʼ��
	virtual VOID	Initial(VOID*);
	//�߼���ѭ����
	virtual VOID	Tick(VOID);
	//�ͷ��Լ�����ӵ�е���Դ
	virtual VOID	Release(VOID);

	virtual std::list< MAP_POS_DEFINE >* GetObjectListByClass( INT nType );
	virtual void setMinimapSerachRange( int nRange ){ m_nSearchRange = nRange; };
	virtual void UpdateMinimapData();
	virtual void MinimapNeedFresh();
	virtual fVector3 GetMyselfPos();
	virtual int GetMyselfDir();
	virtual const CHAR* GetSceneName( INT nSceneID );

public:
	//ϵͳ���𳡾�ת���¼�����
	static VOID	WINAPI	_OnSceneTransEvent(const EVENT* pEvent, UINT dwOwnerData);
	//�����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������)
	static bool WINAPI 	_GetTerrainHeight(float fX, float fZ, float& fY);
	//��GAME���ṩ�ĸ߶Ȳ�ѯ�ӿ�
	static bool WINAPI 	GetTerrainHeight_GamePos(float fX, float fZ, float& fY);
	//---------------------------------------------------------
	//�����ػ��������ڿ��Ƹ�����Ч�Ŀ���
	static VOID	WINAPI	_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);
	
public:

	CWorldManager();
	virtual ~CWorldManager();
	static CWorldManager* GetMe(VOID) { return s_pMe; }

protected:

	static CWorldManager*		s_pMe;

	///��ǰ����ĳ���
	CScene*						m_pActiveScene;
	//��ǰ״̬
	WORLD_STATION				m_Station;

	//--- for debug
	//����Ҫȥ�ĳ���ID
	INT							m_idNextScene;
	//����Ҫȥ�ĳ���λ��
	fVector2					m_fvNextPos;
	//����Ҫȥ�ĳ�������
	FLOAT						m_fNextDir;

	INT			m_nSceneID;
	INT			m_nResID;

	//--- for debug
	bool						m_bAnimyNeedFresh;
	std::list< MAP_POS_DEFINE > m_AnimylistObj;			// �����б�
	std::list< MAP_POS_DEFINE > m_FriendlistObj;		// �����б�


	std::list< MAP_POS_DEFINE > m_FamilylistObj;			// �����б�
	std::list< MAP_POS_DEFINE > m_GuildlistObj;				// ����б�
	std::list< MAP_POS_DEFINE > m_HomelandPlayerlistObj;	// ����������		
	std::list< MAP_POS_DEFINE > m_NoHomelandPlayerlistObj;	// �Ǳ���������

	std::list< MAP_POS_DEFINE > m_ExpObjlistObj;		// �������б�
	std::list< MAP_POS_DEFINE > m_ActivelistObj;		// ������б�
	std::list< MAP_POS_DEFINE > m_ScenePoslistObj;		// �������͵��б�
	std::list< MAP_POS_DEFINE > m_FlashlistObj;			// ������б�

	std::list< MAP_POS_DEFINE > m_ExpNPClistObj;		// NPC�б�
	std::list< MAP_POS_DEFINE > m_MissCompleteNpcObj;	// ����NPC�б�(���)
	std::list< MAP_POS_DEFINE > m_MissNoCompleteNpcObj;	// ����NPC�б�(δ���)
	std::list< MAP_POS_DEFINE > m_MissionNpcObj;	    // ����NPC�б�(�ɽ�)		

	std::list< MAP_POS_DEFINE > m_PetlistObj;			// ������б�

	std::list< MAP_POS_DEFINE >	m_PKlistObj;			// PK����
	std::list< MAP_POS_DEFINE >	m_PKKilllistObj;		// PK���������
	
	CActivePosManager*			m_pActiveManager;
	int		m_nSearchRange;

private:

	bool	CheckIsFriend( INT id );
	void    AddFriend( CObject_PlayerOther* pObj );

protected:

	KL_DECLARE_DYNAMIC(CWorldManager);

};

