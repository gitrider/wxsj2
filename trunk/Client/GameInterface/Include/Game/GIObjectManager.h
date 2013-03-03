
/**
 */

#pragma once


#include "GINode.h"
#include "GIGfxSystem.h"
#include "GICursorSystem.h"



//===================================================
//�ڱ���ʱȷ�������Ƿ�ӵ���߼�����
#define KL_DECLARE_LOGICAL( bHaveLogic) \
public: \
	virtual inline BOOL HaveLogical(VOID) { return bHaveLogic; }


//========================================================
//�������״̬	( OSF = Object Status Flags )
//          
#define OSF_NONE				(0x0)		//��״̬
#define OSF_VISIABLE			(0x1)		//�Ƿ�ɼ�
#define OSF_OUT_VISUAL_FIELD	(0x2)		//�Ѿ�������ҵ���Ұ��Χ,�����ֵά��һ��ʱ��,��ᱻɾ��
#define OSF_RAY_QUERY			(0x4)		//����ܷ�ѡ��

#define MAX_EXTRA_PARAM			3


//===================================================
//��Ϸ�����е�������࣬����������Object���ǿɼ���
struct _ITEM;
enum ITEM_CLASS;

class KYLIN_API tObject : public tNode
{
public:

	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*)	= 0;
	///�õ������ID
	virtual INT					GetID(VOID)				= 0;		//�ͻ���ID
	virtual INT					GetServerID(VOID)const	= 0;		//�������ͬ����ID

	//-----------------------------------------------------
	///����ĳ�����״̬ΪEnable
	virtual VOID				Enable(UINT dwFlag) = 0;
	///����ĳ�����״̬ΪDisable
	virtual VOID				Disalbe(UINT dwFlag) = 0;
	///�쿴ĳ��״̬�Ƿ�ΪEnable
	virtual	BOOL				IsEnable(UINT dwFlag) const = 0;
	///�쿴ĳ��״̬�Ƿ�ΪDisable
	virtual	BOOL				IsDisable(UINT dwFlag) const = 0;
	//�Ƿ��ܹ�����Ϊ��Ŀ��ѡ��
	virtual BOOL				CanbeSelect(VOID) const = 0;

	//--------------------------------------------------------
	//��Ⱦ��ָ��
	virtual tEntityNode*		GetRenderInterface(VOID) = 0;

public:

//-----------------------------------------------------
//�����Tripper����

	enum TRIPPER_OBJECT_TYPE
	{
		TOT_NONE,			//��Tripper����
		TOT_TRANSPORT,		//ת�͵�
		TOT_ITEMBOX,		//���������
		TOT_RESOURCE,		//������еĿ�����Դ
		TOT_PLATFORM,		//������еĺϳ�����Ҫ��ƽ̨
							//...
	};

	//��������
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	= 0;
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const = 0;
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const = 0;
	//���뼤��
	virtual VOID					Tripper_Active(VOID) = 0;

	KL_DECLARE_DYNAMIC(tObject);
	//�����ö���û���߼�����
	KL_DECLARE_LOGICAL(FALSE);
};


//��Ʒ������ӿ�
struct _ITEM;
class KYLIN_API tObject_Item
{
public:
	//-----------------------------------------------------
	//�ӷ��������������ݱ�ʾ(���ںͷ�����ͨѶ)
	union ITEM_GUID
	{
		//�ϲ����ID
		unsigned __int64	m_idUnion;
		
		//���������������� ref GameStruct_Item.h
		struct ID_FROMSERVER
		{
			USHORT			m_idServer;			//�����: (��)101
			USHORT			m_idWorld;			//����˳���ţ�(��)5
			UINT			m_uSerial;			//��Ʒ���кţ�(��)123429
		}m_idOrg;
	}m_ID;

	//-----------------------------------------------------
	//��Ʒ����
	enum 
	{
		RULE_DROP = 0,	// �Ƿ�ɶ���
		RULE_OVERLAY,	// �Ƿ���ص�
		RULE_QUICK,		// �Ƿ�ɷ�������
		RULE_SALE,		// �Ƿ���Գ��۸�NPC�̵�
		RULE_EXCHANGE,	// �Ƿ���Խ���
		RULE_USE,		// �Ƿ����ʹ��
		RULE_PICKBIND,	// �Ƿ���ʰȡ�
		RULE_EQUIPBIND,	// �Ƿ���װ���
		RULE_UNIQUE,	// �Ƿ���Ψһ


		RULE_IDENT,				//�Ƿ���Ҫ����	��
		RULE_DUMMYITEM,			//�Ƿ��������	��
		RULE_BANK,				//�Ƿ���Դ�������	��
		RULE_CONSUME,			//�Ƿ�����	
	};

	//-----------------------------------------------------
	//��Ʒ����
	enum ITEM_OWNER
	{
		IO_UNKNOWN,

		IO_MYSELF_EQUIP,		//����Լ����ϵ�װ��
		IO_MYSELF_PACKET,		//����Լ����ϵİ���
		IO_MYSELF_BANK,			//����Լ����ϵ�������
		IO_MYSELF_MOUNT,        //����Լ�������                              [7/20/2010 �¾���]
		IO_PLAYEROTHER_EQUIP,	//����������ϵ�װ��
		IO_ITEMBOX,				//�򿪵ı�����
		IO_BOOTH,				//���˵Ļ���
		IO_MYEXBOX,				//�Լ��Ľ��׺�
		IO_OTHEREXBOX,			//�Է��Ľ��׺�
//		IO_ENCHASEPAGE,			// ��ʯ�ϳ�/��Ƕ����
		IO_MISSIONBOX,			//����ݽ���
		IO_MYSTALLBOX,			//�Լ���̯λ��
		IO_OTSTALLBOX,			//�Է���̯λ��
//		IO_APPENDITEM,			//װ���ϸ��ӵı�ʯ
		IO_QUESTVIRTUALITEM,	// �������ģ����⣩��Ʒ��ֻ������ʾ��
		IO_PS_SELFBOX,			//
		IO_PS_OTHERBOX,			//
		IO_ITEMDROP,			//����
		IO_ITEMTRANSFER,	//���ڴ��͵���Ʒ
		IO_ITEMIBSHOP,		//IBShop

	};

	//-----------------------------------------------------
	//��Ʒ���
	// 1,3,5,6,7	�������������
	// 2			������ϰ�����
	// 4			�������������
	//
	/*

		Common/GameDefine_Item.h

	enum ITEM_CLASS
	{
		IC_EQUIP		=1,	//����WEAPON������DEFENCE������ADORN
		IC_MATERIAL		=2,	//ԭ��
		IC_COMITEM		=3,	//ҩƷ
		IC_TASKITEM		=4,	//������Ʒ
		IC_GEM			=5, //��ʯ
		IC_STOREMAP		=6,	//�ر�ͼ
		IC_TALISMAN		=7,	//����
		IC_GUILDITEM	=8,	//�����Ʒ
	};

	*/
public:

	//�ͻ���id
	virtual INT					GetID(VOID) const	= 0;
	//GUID
	virtual VOID				SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial)			= 0;
	virtual VOID				GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail) const	= 0;
	virtual unsigned __int64	GetGUID(VOID) const = 0;

	//��Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const	= 0;

	virtual INT				GetItemTableType(VOID) = 0;

	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			= 0;
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	//��Ʒ����ϸ��ţ�����client�õ�index
	virtual INT				GetParticularID(VOID) const	= 0;
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID)			= 0;
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo)= 0;
	virtual VOID			SetExtraInfo( CHAR* pstrIn ) = 0;
	virtual LPCTSTR	GetExtranInfo() = 0;
	//����
	virtual VOID			SetTypeOwner(ITEM_OWNER owner)	= 0;
	virtual ITEM_OWNER		GetTypeOwner(VOID) const		= 0;

	//��������������
	virtual INT				GetPosIndex(VOID) const			= 0;
	virtual VOID			SetPosIndex(INT nPosIndex)		= 0;

	virtual LPCTSTR GetDropVisualID(VOID) const = 0;
	 //��ʾ������ɫ
	virtual LPCTSTR GetDropVisColor(VOID) const = 0;

	// �õ������Ʒ�ĵȼ�
	virtual INT				GetItemLevelTable(VOID) const = 0;

	//��Ʒ����
	virtual VOID			SetNumber(INT nNum)				= 0;
	virtual INT				GetNumber(VOID) const			= 0;

	//�������,��ʱ����������Ʒ����ʾ
	virtual VOID			SetMax(INT nNum)				= 0;
	virtual INT				GetMax(VOID) const				= 0;

	//��ȡ��������
	virtual INT				GetLayNum(VOID)					= 0;

	//����Դ���е�λ��
	virtual INT				GetIdTable(VOID) const			= 0;
	//��ѯ�߼�����,һ�����ڽű�����
	virtual STRING			GetAttributeValue(LPCTSTR szValueName) = 0;

	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const tObject_Item* pItemSource) = 0;

	// �õ��Ƿ�����(����UI����)
	virtual BOOL			IsLocked()						= 0;
	virtual VOID			SetLock(BOOL bLock)				= 0;

	//��Ʒ������֤
	virtual BOOL			Rule(INT ruletype)				= 0;
	virtual STRING			RuleFailMessage(INT ruletype)	= 0;

	//��Ʒ�Ķ������뱣��
	virtual BOOL			GetByProtect()					= 0;
	virtual VOID			SetByProtect(BOOL bByProtect)	= 0;


	//--------------------------------------------------------------------------------------------------------------------------
	//-- super tool tip ����
	//--

	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel()						   = 0;

	// �õ���Ʒ�;ö�
	virtual INT				GetItemDur()						   = 0;

	// �õ���Ʒ����;ö�
	virtual INT				GetItemMaxDur()						   = 0;

	// �õ���Ʒ��������� 
	virtual INT				GetItemRepairCount()				   = 0;

	// �õ���Ʒ�İ���Ϣ 
	virtual INT				GetItemBindInfo()				       = 0;

	// �õ���Ʒ��2������Ϣ 
	virtual  INT			GetItemSecLevelBindInfo()			   = 0;	

	//�󶨼ӳ���Ϣ
	virtual LPCTSTR			GetItemBindAddInfo()				   = 0;	


	// 20100408 AddCodeBegin
	// �õ���Ʒ����ļ۸�
	virtual  INT			GetItemBasePrice()					   = 0;
	// 20100408 AddCodeEnd

	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice()						   = 0;

	//�õ���Ʒְҵ
	virtual INT             GetItemProfession()                    = 0;

	//�õ���Ʒ�Ƿ�Ψһ
	virtual INT				GetItemRuleOnly()					   = 0;

	// �õ���Ʒ��������
	virtual LPCTSTR			GetManufacturer()				       = 0;

	// �õ���ɫ����
	virtual LPCTSTR			GetBaseWhiteAttrInfo()                 = 0;


	//remove
	// �õ���չ��ɫ����
	//virtual LPCTSTR			GetExtBlueAttrInfo()                   = 0;

	//�õ�װ����չ����
    // virtual LPCTSTR         GetExEquipAttrInfo()                   = 0;

	//�õ���Ʒʹ�õ�Ŀ������
	virtual INT				GetItemTargetType()					   = 0;

	// �õ���Ʒ����������
	virtual LPCTSTR			GetItemTableTypeDesc()				   = 0;

	// �õ���Ʒ�ĵ�һ�����������Ϣ
	virtual INT			GetExtraParam1() = 0;

	// �õ���Ʒ�ĵڶ������������Ϣ
	virtual INT			GetExtraParam2() = 0;

	// �õ���Ʒ�ĵ��������������Ϣ
	virtual INT			GetExtraParam3() = 0;
};

//===================================================
//��Ϸ�����е����������
class KYLIN_API tObjectSystem : public tNode
{
public:

	enum DESTROY_MAIN_TARGET_TYPE
	{
		DEL_OBJECT = 0,		// ���ٶ���
		DEL_RIGHT_CLICK,	// ����Ҽ�����
	};

public:

	/// ����������,������������
	virtual tObject*		NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL) = 0;
	/// ��������,�����������ϲ��
	virtual BOOL			DestroyObject(tObject* pObject) = 0;
	/// ����ID�õ�ĳ����
	virtual tObject*		FindObject(INT id) = 0;
	// ��ȡһ��SERVER OBJ�����ָ��
	virtual tObject*		FindServerObject( INT idServer ) = 0;
	/** ������������ȡ���� */
	virtual const std::vector<INT>& GetNpcObjectId(VOID) = 0;
	// �����Ʒ
	virtual tObject_Item*	FindItem(INT id) = 0;
	// ������Ļ�������ѡ�е�����
	virtual tObject*		GetMouseOverObject(INT nX, INT nY, fVector3& fvMouseHitPlan ) = 0;	
	/// ������Ŀ��������id�Ƿ���ȡ��ѡ������
	virtual VOID			SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT) = 0;
	//ȡ�õ�ǰѡ������
	virtual tObject*		GetMainTarget(VOID)	= 0;
	//����ĳ�����ָ���¼� id-����ID		bCare-���Ļ���ȡ������
	virtual VOID			CareObject(INT id, BOOL bCare, std::string szSign) = 0;
	//�ӱ�����Դ���ж�ȡ��Ʒ����
	virtual LPCTSTR			ItemNameByTBIndex(UINT idTable) = 0;

	virtual VOID InitActorAvatar(VOID) = 0;
	virtual VOID DestoryActorAvatar(VOID) = 0;

	virtual LPCTSTR GetMySelfOrTargetName(BOOL bTarget = FALSE) = 0;

protected:

	KL_DECLARE_DYNAMIC(tObjectSystem);

};
