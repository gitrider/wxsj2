/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	9:35
	�ļ�����:	GrowPointManager.h
	�ļ�·��:	d:\Prj\Server\Server\Item\GrowPointManager.h
	������:		������
	
	�ļ����ܣ� ��������Ĺ���
			   ÿ������һ��

	�޸ļ�¼��
	
*********************************************************************/


#ifndef		_GROWPOINT_MANAGER_H_
#define		_GROWPOINT_MANAGER_H_
#include	"Type.h"
#include	"LuaInterface.h"

class Scene;


//��������������
struct  GROW_POINT_GROUP_SCENE_DATA 
{
	WORD		GrowPointType;								//����������
	FLOAT		GrowPointX;									//������x����
	FLOAT		GrowPointY;									//������Y����
	INT			GrowPointRandValue;							//�������������ֵ
	BOOL		Used;
	GROW_POINT_GROUP_SCENE_DATA()
	{
		memset(this,0,sizeof(GROW_POINT_GROUP_SCENE_DATA));
	}
};

//��������������
struct GROW_POINT_GROUP_SCENE_SETUP 
{
	WORD		GrowPointType;						//����������
	WORD		GrowPointMaxCount;					//������ͬʱ���ָ���
	INT			GrowPointInterval;					//����ʱ����

	GROW_POINT_GROUP_SCENE_SETUP()
	{
		GrowPointType = -1;
		GrowPointMaxCount = 0;
		GrowPointInterval = 1000000;
	}

};

//��һ���������������
class TypeGrowPointManager
{
public:

	TypeGrowPointManager()
	{
		m_pData				=	NULL;
		m_AddOffSet			=	0;
		m_TypeCount			=	0;
		m_TypeOffSet		=	0;
		m_IntervalPerSeed	=	0;
		m_CurrentElapse		=	0;
		m_bStartCount		=	FALSE;
		m_LastUpdateTime	=	0;
		m_CurrentCount		=	0;
		m_MaxApperaCount	=	0;
		m_bEnable			=	FALSE;		

	}

	~TypeGrowPointManager()
	{
		if(m_pData)
			SAFE_DELETE_ARRAY(m_pData);
	}



	//����������
	GROW_POINT_GROUP_SCENE_DATA*	m_pData;
	//��ǰ����
	INT								m_CurrentCount;
	//���������
	INT								m_MaxApperaCount;
	//�����༭�����������
	INT								m_TypeCount;
	//���ƫ�Ƽ���
	INT								m_AddOffSet;
	//����ƫ�Ƽ���
	INT								m_TypeOffSet;
	//����������
	WORD							m_GrowPointType;
	//�������ƽű����
	ScriptID_t						m_ScriptID;
	//�������ӵļ��ʱ��
	UINT							m_IntervalPerSeed;
	//��ǰ��ʱ��
	UINT							m_CurrentElapse;
	//������ʱ��
	UINT							m_LastUpdateTime;
	//��ʼ����
	BOOL							m_bStartCount;
	//��ǰ���͹������Ƿ񼤻�
	BOOL							m_bEnable;
	//���������λ������
	inline	VOID					AddData(GROW_POINT_GROUP_SCENE_DATA& RefData);

	//���һ��������λ��
	inline	BOOL					CreateGrowPointPos(FLOAT& x,FLOAT& y);
	//�ͷ�һ��������λ��			
	inline  BOOL					ReleaseGrowPointPos(FLOAT x,FLOAT y);

	/*-----------���������������---------------*/

	//�����������
	inline	VOID					SetCount(INT count); 
	//��ȡ�������
	inline	INT						GetCount();
	
	//���������㳡�����ݸ���
	inline	VOID					IncCount();
	//���ӵ�ǰ�Ѿ����ֵĸ���
	inline	VOID					IncCurrentCount();
	//���ٵ�ǰ���ֵĸ���
	inline	VOID					DecCurrentCount();
	
	

	//��ù�������������
	inline	WORD					GetGrowPointType();
	
	//���ù�������������
	inline	VOID					SetGrowPointType(WORD gpType);
	
	//���ù��������ּ��
	inline	VOID					SetIntervalTime(INT iInterval);
	

public:
	/* 
	*	�ض��������������������
	*	��Ҫ��ɶ�������ĸ�������
	*/
	inline	BOOL					DoTicks(UINT uTime);

	/*
	 * * ���������������//
	 */
	VOID							RandSort();
};


struct GP_OWNER
{
	GP_OWNER()
	{
		OwnerID		= INVALID_ID;
		OwnerCount	= 0;
	}

	ObjID_t	OwnerID;
	INT		OwnerCount;
};


struct GROWPOINT_LOAD
{
	INT									m_DataCount;		//ԭʼ���ݸ���
	GROW_POINT_GROUP_SCENE_DATA*		m_pData;			//ԭʼ����
	INT									m_SetupCount;		//�������ݸ���
	GROW_POINT_GROUP_SCENE_SETUP*		m_pSetup;			//��������

};

class GrowPointManager
{

	//���������������
	TypeGrowPointManager*				m_GrowPointMgr[MAX_SCENE_GROW_POINT_NUM];
	//�����������ܺ�
	INT									m_GrowPointCount;
	//��������������
	INT									m_GrowPointTypeCount;
	//����ָ��
	Scene*								m_pScene;
	//�ű��ӿ�
	LuaInterface*						m_pLuaInterface;

	GP_OWNER							m_GPOwner[MAX_SCENE_GP_OWNER];
	INT									m_OwnerCount;
public:

	GrowPointManager()
	{
		memset(m_GrowPointMgr,0,sizeof(TypeGrowPointManager*)*MAX_SCENE_GROW_POINT_NUM);
		m_pLuaInterface	=	0;
		m_OwnerCount	= 0;
		m_GrowPointCount	= 0;
		m_GrowPointTypeCount = 0;

	};

	~GrowPointManager()
	{
		for(INT i =0;i<MAX_SCENE_GROW_POINT_NUM;i++	)
		{
			if(m_GrowPointMgr[i])
				SAFE_DELETE(m_GrowPointMgr[i]);
		}
	};

	/*
	 *	 ���ĳ�����������������
	 *
	 *	 ����˵��
	 *	 GrowPointType	����������
	 *
	 */

	inline	TypeGrowPointManager*		GetGpManagerByGrowPointType(INT GrowPointType);
	

public:
	/*
	 *	���ز���
	 *	����˵��
	 *
	 *	CHAR*	strGrowPointDataFile			ÿ������һ����GrowPoint�����ļ�
	 *												
	 *	CHAR*   strGrowPointSetupFile			ÿ������һ����GrowPoint�����ļ�			
	 *	
	 */
	BOOL		Load(CHAR*	strGrowPointDataFile,CHAR* strGrowPointSetupFile);
		
	BOOL		Load(GROWPOINT_LOAD* pGPLoad,CHAR*	strGrowPointDataFile,CHAR* strGrowPointSetupFile);

	BOOL		Load(GROWPOINT_LOAD* pGPLoad);
	/*
	 *	�������
	 */
	VOID		CleanUp();
	
	
	/*
	 *	���ó���ָ��
	 */
	VOID		SetScene(Scene* pScene)
	{
		m_pScene	=	pScene;
	}

	/*
	 *	��ó���ָ��
	 */
	Scene*		GetScene(){return m_pScene;}

	/* �������
	 *	
	 */
	BOOL		HeartBeat(UINT	uTime);
	/*		 ������������������
	 *
	 *		 ����˵��
	 *				GrowPointType	����������
	 *			
	 *		
	 */
	BOOL		DecGrowPointTypeCount(INT GrowPointType,FLOAT x,FLOAT y);

	/*---------			�ű��ӿڴ���		-----------------*/
	
	/*��̬�����ӿ�
	 *	
	 */
	LuaInterface*	GetLuaInterface(){return m_pLuaInterface;}
	
	/*���ýű��򿪺���,OnOpen
	 *
	 *	����˵��:
	 *			FileID		�ű��ļ����
	 *			SelfID		������id,һ����HumanID ,û������-1
	 *			TargetID	����ID,һ����ItemBoxID
	 *			SceneID		�������
	 */
	INT			CallScriptOpenBoxFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	
	/*
	 *	���ýű����պ���,OnRecycle
	 *
	 *	����˵��:
	 *			FileID		�ű��ļ����
	 *			SelfID		������id,һ����HumanID ,û������-1
	 *			TargetID	����ID,һ����ItemBoxID
	 *			SceneID		�������
	 */
	INT			CallScriptRecycleFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	/*	���ýű����̽�������,OnProcOver
	 *
	 *	����˵��:
	 *			FileID		�ű��ļ����
	 *			SelfID		������id,һ����HumanID ,û������-1
	 *			TargetID	����ID,һ����ItemBoxID
	 *			SceneID		�������
	 *	
	 */

	INT			CallScriptProcOverFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	/*
	 *	���ýű���������,OnCreate
	 *	����˵��:
	 *			FileID		�ű��ļ����
	 *			SelfID		������id,һ����HumanID ,û������-1
	 *			TargetID	����ID,һ����ItemBoxID
	 *			SceneID		�������
	 *	
	 */
	BOOL		CallScriptCreateFunc(ScriptID_t		FileID,FLOAT x,FLOAT y,INT iItemBoxType,SceneID_t SceneId);
private:
	/*			���������ݳ�ʼ��
	 *	
	 *			����1:
	 *			szGrowPointDataFile
	 *	
	 *			����1����
	 *			�����������ļ�
	 *
	 */
	BOOL	    GrowPointDataInit(CHAR*	szGrowPointDataFile);
	/*			���������ó�ʼ��
	 *
	 *			����1:
	 *				szGrowPointSetupFile
	 *
	 *          ����1����
	 *				�����������ļ�
	 */
	BOOL		GrowPointSetupInit(CHAR* szGrowPointSetupFile);
public:
	
	/*
	 *		   �����������������Ϣ���ʷ���
	 */

	BOOL		AddGrowPointOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return FALSE;
		
		BOOL bNewOwner = TRUE;
		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				m_GPOwner[i].OwnerCount++;
				bNewOwner = FALSE;
			}
		}
		
		if(bNewOwner)
		{
			m_GPOwner[m_OwnerCount].OwnerID = OwnerID;
			m_GPOwner[m_OwnerCount].OwnerCount = 1;
			
		}
		return TRUE;
	}

	BOOL		DelGrowPointOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return FALSE;

		BOOL bNewOwner = TRUE;
		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				m_GPOwner[i].OwnerCount--;
				bNewOwner = FALSE;
				if(m_GPOwner[i].OwnerCount==0)
				{
					m_GPOwner[i].OwnerID = INVALID_ID;
					m_OwnerCount--;
				}	
			}
		}

		if(bNewOwner)
		{
			return FALSE;
		}
		return TRUE;
	}

	INT			GetGrowPointCountByOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return -1;

		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				return m_GPOwner[i].OwnerCount;
			
			}
		}
		return -1;
	}



};

#include "GrowPointManager.inl"


#endif