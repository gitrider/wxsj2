
/**	ObjectManager.h
*/

#ifndef _OBJECT_MANAGER_H
#define _OBJECT_MANAGER_H


#include "GIObjectManager.h"
#include "Type.h"





class CObject;
class CObject_Character;
class CObject_PlayerMySelf;
class CObject_PlayerOther;
class CObject_PlayerNPC;
class CObject_Surface;
class CObjectLoadQueue;
class CObjectDestoryQueue;

/**	������Ϸ�еĶ���
 */
class CObjectManager : public tObjectSystem
{
protected:

	/// ��UI���ĵ��߼�����
	struct OBJECT_BECARED
	{
		INT		id;					/// ����ID
		FLOAT	fLastDistance;		/// ������֮��ľ���
	};

public:

	CObjectManager( VOID );
	virtual ~CObjectManager( VOID );

	virtual VOID Initial( VOID* );
	virtual VOID Release( VOID );
	virtual VOID Tick( VOID );


	static CObjectManager* GetMe( VOID )						{ return s_pMe; }
	
	/** ȡ������Լ� */
	CObject_PlayerMySelf* GetMySelf( VOID )						{ return m_pMySelf; }
	/** ��ȡ��Ŀ��UIģ�� */
	CObject_PlayerOther* GetTarget_Avatar( VOID )				{ return m_pMainTargetAvatar;}	
	
	/** ������ǵ�UIģ�� */
//	CObject_PlayerOther* GetActorAvatar(INT index)				{ if((index < 0) || (index >= 2)) return NULL; return m_pAvatar[index]; }
	/** ��Ĭ�ϵĲ�����ʼ�����ǵ�UIģ�� */
	virtual VOID InitActorAvatar(VOID);
	virtual VOID DestoryActorAvatar(VOID);


	/** �����¶���,������������
	 */
	virtual tObject* NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL);
	virtual CObject_PlayerOther* NewPlayerOther( INT idServer );
	virtual CObject_PlayerNPC* NewPlayerNPC( INT idServer );
	/** ������������ */
	virtual CObject_Surface* NewSurfaceObj( string strClass, LPCTSTR ObjName );

	/** ��������,�����������ϲ�� */
	virtual BOOL DestroyObject(tObject* pObject);

	/** ����ָ�����Ƶ����壨�ӵ��� */
	virtual VOID DetchBullObject( string& name );


	/** ����ID�õ�ĳ���� */
	virtual tObject* FindObject( INT id );
	
	/** ��ȡһ��SERVER OBJ�����ָ�� */
	virtual tObject* FindServerObject( INT idServer );

	/** ͨ����������ȡ��������ֻ�����ӵ��� */
	virtual tObject* FindBullObject( string& objName );

	/**/
	virtual const std::map<string, CObject*>& GetObjectStrMap(){ return m_mBullObject; }

	/** ��ȡ����ĵ�ǰNpc�б��id */
	virtual const std::vector<INT>& GetNpcObjectId(VOID);
	
	/** �����Ʒ */
	virtual tObject_Item* FindItem(INT id);
	
	/** ������������ */
	virtual VOID ClearAllObject( VOID );
	
	/** ������Ŀ��������id�Ƿ���ȡ��ѡ������ */
	virtual VOID SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT);

	//��ȡObject������	20100706 BLL
	virtual LPCTSTR	 GetMySelfOrTargetName(BOOL bTarget = FALSE);
	
	/** ȡ�õ�ǰѡ������ */
	virtual tObject* GetMainTarget(VOID);

	/** ȡ�õ�ǰѡ������ */
	virtual tObject* GetMainTarget(GUID_t& guid);

	/** �������һ��������Ϊ��Ŀ�� */
	void Debug_LockNearestTarget(void);
	
	/** ����ĳojbect��ServerID */
	virtual BOOL SetObjectServerID( INT idObj, INT idServer );
	
	/** ������Ļ�������ѡ�е����� */
	virtual tObject* GetMouseOverObject( INT nX, INT nY, fVector3& fvMouseHitPlan );
	
	/** ����ĳ�����ָ���¼� id-����ID		bCare-���Ļ���ȡ������ */
	virtual VOID CareObject(INT id, BOOL bCare, std::string szSign);
	
	/** �ӱ�����Դ���ж�ȡ��Ʒ���� */
	virtual LPCTSTR ItemNameByTBIndex(UINT idTable);

	/** ��Դ���ض��� */
	CObjectLoadQueue* GetLoadQueue(void)				{ return m_pLoadQueue; }

	/** ���ٶ��� */
	CObjectDestoryQueue* GetDestoryQueue(void)			{ return m_pDestoryQueue; }
	

	/** �����������ֻ�ö�Ӧ��objcet */
	virtual CObject_Character* FindCharacterByName(std::string  szName);
	VOID SetTargetAvatarAsTarget( VOID );
	

	/** ��Ӧ�ֱ��ʸı� */
	void OnScreenSizeChanged(void);
	
	/** ��Ӧ���ǵȼ����,��������npc�ĵȼ� */
	void OnMySelfLevelUp(void);


	/** ���õ�ǰѡ����Ƕ��� */
	void SetCurSelTeam(GUID_t id);
	GUID_t GetCurSelTeam();

	/** ���ö����Ƿ��ڵ�ǰ���ӷ�Χ */
	void SetPresent(BOOL  bIsPresent);
	
	/** ��ǰѡ����Ƿ��Ƕ��� */
	BOOL IsSelTeam();

	/** ѡ��Ķ����Ƿ�����ǰ */
	BOOL IsSelTeamInPresent();


	/** ��ѡ��mainTarget */
	void UnSelMainTarget(void);

	/** ��ĳ����ת��Ŀ����� */
	void FaceToTarget(INT sObj, INT tObj);


private:
	
	INT IDFactory( VOID );

protected:
	
	VOID AttachObject( tObject *pObject );
	VOID DetchObject( tObject *pObject );

	KL_DECLARE_DYNAMIC(CObjectManager);

protected:
	
	static CObjectManager*			s_pMe;

	/// ���ѱ���
	CObject_PlayerMySelf*			m_pMySelf;
	/// ������ڵ�
	CObject*						m_pObjectRoot;
	/// �߼�����ڵ�
	CObject*						m_pLogicalObject;
	/// ������ҽڵ�
	CObject*						m_pPlayerOther;
	/// NPC�ڵ�
	CObject*						m_pNPC;
	/// �ر�����ڵ�
	CObject*						m_pSurface;
	/// ���߼�����ڵ�
	CObject*						m_pAlogicalObject;

	typedef std::map<INT, CObject*> CObjectMap;
	/// ����ID������HASH��
	CObjectMap						m_hmObject;
	/// ����ServerID������HASH��
	CObjectMap						m_hmServerObject;
	
	typedef std::map<string, CObject*> CObjectStrMap;
	// �����ӵ����� Ӧ������֪����
	CObjectStrMap					m_mBullObject;
	
	// ������ص�NPC���ڲ�ID
	// ��Ҫ���ڿͻ���Ԥ�����Զ�����ʩ�ŷ�Χ�ڵĹ�
	std::vector<INT>				m_vNpcObjectIndex;

	/// ��Ŀ�����
	CObject*						m_pMainTarget;


	//====== UIģ��
	// �����е�UI��ص�ģ�ͷ�������ͳһ���й���

	/// ѡ�е���Ŀ��
	CObject_PlayerOther*			m_pMainTargetAvatar;
//	CObject_PlayerOther*			m_pAvatar[2];			/// ��Ů����
															/// ����


	/// Ŀǰ�����ĵ��߼�����
	std::map<std::string, OBJECT_BECARED>	m_mapCaredObject;

	/// ��Դ���ض���
	CObjectLoadQueue*				m_pLoadQueue;
	/// ��Դ���ٶ���
	CObjectDestoryQueue*			m_pDestoryQueue;


	/// ��ǰѡ����Ƿ��Ƕ���
	BOOL							m_bIsSelTeam;

	/// ѡ��Ķ����Ƿ�����ǰ
	BOOL							m_bIsSelTeamInPresent;

	/// ѡ�ж��ѵ�gudi
	GUID_t							m_GUID;	


};	// class


#endif	// _OBJECT_MANAGER_H