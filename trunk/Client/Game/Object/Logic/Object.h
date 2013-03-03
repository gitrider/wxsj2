
/** Object.h
 *	���߼��࣬��ͼ���޹�
 */ 

#pragma once


#include "..\..\GameCommand.h"
#include "GIObjectManager.h"



struct SObjectInit
{
	fVector3	m_fvPos;
	fVector3	m_fvRot;

	SObjectInit( VOID )
	{
		m_fvPos.x			= -1.f;
		m_fvPos.y			= -1.f;
		m_fvPos.z			= -1.f;

		m_fvRot.x			= -1.f;
		m_fvRot.y			= -1.f;
		m_fvRot.z			= -1.f;
	}

	virtual VOID Reset( VOID )
	{
		m_fvPos.x			= -1.f;
		m_fvPos.y			= -1.f;
		m_fvPos.z			= -1.f;

		m_fvRot.x			= -1.f;
		m_fvRot.y			= -1.f;
		m_fvRot.z			= -1.f;
	}
};


struct SCommand_Object;
struct SCommand_Mouse;
struct _CAMP_DATA;
class tActionItem;

/** ��Ϸ�����е��������
 */
class CObject : public tObject
{
public:

	CObject();
	virtual ~CObject();


	/** ���ݳ�ʼ�����壬��ͬ������Ⱦ�� */
	virtual	VOID				Initial(VOID*);
	/** ���ٸ�����,Ӧ�������һ�������õĺ��� */
	virtual VOID				Destroy(VOID);

public:

	/** ѹ��һ��ָ�� */
	virtual BOOL				PushCommand(const SCommand_Object *pCmd );

protected:

	/** ָ�����, �����ƽ�ɫ��Ψһ�ӿ� */
	virtual RC_RESULT			OnCommand(const SCommand_Object *pCmd )		{ return RC_SKIP; }

public:

	/** �õ������ID */
	virtual INT					GetID(VOID)									{ return m_ID; }


	/** ����ĳ�����״̬ΪEnable */
	virtual VOID				Enable(UINT dwFlag);
	/** ����ĳ�����״̬ΪDisable */
	virtual VOID				Disalbe(UINT dwFlag);
	/** �쿴ĳ��״̬�Ƿ�ΪEnable */
	virtual	BOOL				IsEnable(UINT dwFlag) const					{ return (m_dwStatusFlag & dwFlag) ? true : FALSE; }
	/** �쿴ĳ��״̬�Ƿ�ΪDisable */
	virtual	BOOL				IsDisable(UINT dwFlag) const				{ return (m_dwStatusFlag & dwFlag) ? FALSE : true; }

	BOOL	isVisible(VOID) { return IsEnable(OSF_VISIABLE) ;}
	VOID	setVisible(BOOL setting) { Enable(OSF_VISIABLE) ;}


public:

/** ��������
 */

	/** �Ƿ��ܹ�����Ϊ��Ŀ��ѡ�� */
	virtual BOOL				CanbeSelect(VOID) const						{ return FALSE; }
	/** �Ƿ�����������(player, target, pet, party1, ...); */
	virtual BOOL				IsSpecialObject(STRING& strReturn);

	/** ���ָ��ķ��� */
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** �Ҽ�ָ��ķ��� */
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


/** TripperObj 
 */

	/** �������� */
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const				{ return TOT_NONE; }
	/** �ܷ������� */
	virtual BOOL					Tripper_CanOperation(VOID) const		{ return FALSE; }
	/** ������� */
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const			{ return CURSOR_NORMAL; }
	/** ���뼤�� */
	virtual VOID					Tripper_Active(VOID)					{ }

public:
	
/** Object �߼����
 */

	/** ����Ƿ��ٴ�� */
	virtual VOID				Tick_CheckAlive(VOID);

public:

	/** ������ID */
	virtual VOID				SetServerID(INT nID)						{ m_idServer = nID; }
	virtual INT					GetServerID(VOID) const						{ return m_idServer; }

	/** ���һ�ν��ܷ�������Ϣʱ�� */
	VOID						SetMsgTime(UINT dwTime);
	UINT						GetMsgTime(VOID) const						{ return m_timeMsg; }

	/** �������˵ĵ�ǰOBJ�߼��� */
//	INT							GetLogicCount( VOID ) const					{ return m_nLogicCount; }
//	VOID						SetLogicCount( INT nLogicCount )			{ m_nLogicCount = nLogicCount; }
//	virtual BOOL				IsLogicReady( INT nLogicCount ) const		{ return (m_nLogicCount >= nLogicCount)?(TRUE):(FALSE); }
//	virtual VOID				ResetLogicCount(VOID)						{ m_nLogicCount = -1; }

	INT							GetBaseLogicCount( VOID ) const				{ return m_nBaseLogicCount; }
	VOID						SetBaseLogicCount( INT nLogicCount )		{ m_nBaseLogicCount = nLogicCount; }
	virtual BOOL				IsBaseLogicReady( INT nLogicCount ) const	{ return (m_nBaseLogicCount >= nLogicCount) ? (TRUE) : (FALSE); }
	virtual VOID				ResetBaseLogicCount( VOID )					{ m_nBaseLogicCount = -1; }

	INT							GetActionLogicCount( VOID ) const			{ return m_nActionLogicCount; }
	VOID						SetActionLogicCount( INT nLogicCount )		{ m_nActionLogicCount = nLogicCount; }
	virtual BOOL				IsActionLogicReady( INT nLogicCount ) const	{ return (m_nActionLogicCount >= nLogicCount) ? (TRUE) : (FALSE); }
	virtual VOID				ResetActionLogicCount( VOID )				{ m_nActionLogicCount = -1; }


	/** λ�� */
	virtual VOID				SetPosition(const fVector3& fvPosition);
	const fVector3&				GetPosition(VOID) const						{ return m_fvPosition;			}
	/** ���ݵ�������λ��, fvPosition�зֱ���x��z */
	virtual VOID				SetMapPosition(const fVector2& fvPosition);

	/** ��ת�Ƕ� (����) */
	virtual VOID				SetRotation(const fVector3& fvRotation);
	const fVector3&				GetRotation(VOID) const						{ return m_fvRotation;			}

	/** �ر�λ�� */
	virtual VOID				SetFootPosition(const fVector3& fvPosition);
	const fVector3&				GetFootPosition(VOID) const					{ return m_fvFootPosition;		}

	/** �泯�ķ��򣬼�y����ת�Ƕ� (����) */
	virtual VOID				SetFaceDir(FLOAT fDir);
	virtual FLOAT				GetFaceDir(VOID) const						{ return m_fvRotation.y; }

	/** ��Ⱦ��ָ�� */
	tEntityNode*				GetRenderInterface(VOID)					{ return m_pRenderInterface; }

	/** �Ƿ�����UI��Ⱦ�Ķ��� */
	BOOL						GetFakeObjectFlag(VOID) const				{ return m_bFakeObject; }
	VOID						SetFakeObjectFlag(BOOL bVisibleFlag)		{ m_bFakeObject = bVisibleFlag; }

	/** ��ӪID */
	virtual const _CAMP_DATA*	GetCampData(VOID) const						{ return NULL; }
	virtual ObjID_t				GetOwnerID(VOID) const						{ return INVALID_ID; }


protected:

	/// ����Ⱦ���еĽӿ�
	tEntityNode*		m_pRenderInterface;


protected:
	
	/// �������ڷ������ϵ�ID���ɷ��������أ�
	//  ������ȫ���ܷ��������Ƶľ�̬���壬��ֵΪINVALID_ID
	INT				m_idServer;

	/// ���һ�ν��ܷ�������Ϣʱ��,���ڴӲ�������Ϣ�ı��ؾ�̬����,��ֵΪ����ʱ��
	UINT			m_timeMsg;

	/// �������˵��߼�����
//	INT				m_nLogicCount;
	INT				m_nBaseLogicCount;
	INT				m_nActionLogicCount;

	/// ����״̬��־
	/*
    |    xxxxxxxx xxxxxxxx xxxxxxxx xxxxx000
    |                                    |||_____ 1:�ɼ�  0:���ɼ�
    |                                    ||______ 1:��Ⱦ���� 0:����Ⱦ����
    |                                    |_______ 1:�Ѿ�������ҵ���Ұ��Χ,�����ֵά��һ��ʱ��,��ᱻɾ�� 0:���������Ұ��Χ
    |           
	*/
	UINT			m_dwStatusFlag;
	
	/// ����иñ�־������������ֻ������UI��Ⱦ����FakeObjectManager����
	BOOL			m_bFakeObject;

	/// �������ڿͻ��˵�ΨһID,��Object����ʱ��̬����
	INT				m_ID;
	/// �������ڳ����еĸ���λ��,��ʼ��Ϊ<0.0f, 0.0f, 0.0f>
	fVector3		m_fvPosition;
	/// �������ڳ����е���ת�Ƕ�,��ʼ��Ϊ<0.0f, 0.0f, 0.0f>
	fVector3		m_fvRotation;
	/// ������ĵر�λ�ã��������������׼
	fVector3		m_fvFootPosition;


	//--------------------------------------------------------
	KL_DECLARE_DYNAMIC(CObject);
	KL_DECLARE_LOGICAL(FALSE);

	friend class CObjectManager;

public:

	//---- for Debug
	VOID PushDebugString(LPCTSTR szDebugString);
	std::deque< STRING > m_qDebugString;

	virtual STRING GetDebugDesc(VOID)										{ return STRING(""); }
	//---- for Debug
};
