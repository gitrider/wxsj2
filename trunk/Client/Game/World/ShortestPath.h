/***********************************************************
����: ��Զ
����: 2010-04-19
˵��: �糡��Ѱ·�㷨
************************************************************/

#pragma once

#include <map>

//�������͵�ṹ
struct SceneTransferData 
{
	INT	nSceneID;
	INT	xPos;
	INT	yPos;
	INT	nDestSceneID;

	SceneTransferData()
	{
		nSceneID = -1;
		xPos = -1;
		yPos = -1;
		nDestSceneID = -1;
	}
};

typedef int SceneTransferDataID;

//����ÿ����������ͨ·����
#define MAX_WAY_ONE_SCENE	8

//ÿ��������·��Ϣ,�������͵�ļ���
struct SceneWayInfo 
{
	SceneTransferDataID	nWayID[MAX_WAY_ONE_SCENE];
	bool								bValidFlag;

	SceneWayInfo()
	{
		memset(nWayID, -1, sizeof(nWayID));
		bValidFlag = false;
	}

	bool Add(INT id)
	{
		for (INT i=0; i<MAX_WAY_ONE_SCENE; i++)
		{
			if (nWayID[i] == -1)
			{
				nWayID[i] = id;
				return true;
			}
		}
		return false;
	}
	bool IsValid(void)
	{
		return bValidFlag;
	}
	void	SetValid(bool setting)
	{
		bValidFlag = setting;
	}
};

typedef int	SceneWayInfoID;

//һ������·��������㳤��
#define MAX_PATH_ID_ARRAY	256
struct SearchPathNode
{
	SceneTransferDataID	nPathIDArray[MAX_PATH_ID_ARRAY];
	UINT	nWeight;
	INT		nCurrentPos;

	SearchPathNode()
	{
		Clear();
	}
	void	Clear()
	{
		memset(nPathIDArray, -1 , sizeof(nPathIDArray));
		nWeight = INFINITE;
		nCurrentPos = 0;
	}
	bool	IsInUse(void)
	{
		return nWeight != INFINITE;
	}
	void	MarkUnUse(void)
	{
		nWeight = INFINITE;
	}
	void	MarkUse(void)
	{
		nWeight = 0;
		nCurrentPos = 0;
	}
	void	AddWeight(INT w= 1)
	{
		nWeight += w;
	}
	bool	AddPathID(SceneTransferDataID nID)
	{
		for (INT i=0; i<MAX_PATH_ID_ARRAY; i++)
		{
			//����ѭ������
			if (nPathIDArray[i] == nID)
			{
				return false;
			}
			//�ҵ��հ׵�,Ȼ����ӽ�ȥ
			if(nPathIDArray[i] == -1)
			{
				nPathIDArray[i] = nID;
				++nCurrentPos;
				return true;
			}
		}
		return false;
	}
	void	RemoveLastAddID(void)
	{
		--nCurrentPos;
		if ( nCurrentPos >= 0 )
		{
			nPathIDArray[nCurrentPos] = -1;
		}
	}
	SceneTransferDataID GetLastID()
	{
		if(nCurrentPos < 1 || nCurrentPos >MAX_PATH_ID_ARRAY)
		{
			return -1;
		}
		return nPathIDArray[nCurrentPos-1] ;
	}
};
//ϵͳ��ÿ�μ�������ж��ٵ���ͨ·
#define MAX_PATH_NODE	1024

//����������·�����ID
typedef int	SearchPathNodeID;

//first����Ȩ��,ԽС,��ʾ�����·��,second����SearchPathNode������,��һ��ͨ·������
typedef std::multimap<int, SearchPathNodeID>	SearchPathMap;

typedef std::vector<SceneTransferData> FindResult;

//״̬����
typedef std::vector<SceneWayInfoID>	StateSet;

class ShortestPath
{
public:
	ShortestPath(void);
	~ShortestPath(void);

public:
	//�ӿں���
	void	Initailize(INT maxData, INT maxScene);
	void	AddSceneTransData(const SceneTransferData& data);
	bool	FindPath(INT nDestScene,  INT nSrcScene, FindResult& rOutPath);


protected:
	//�㷨����
	INT		AddSearchPathNode(SearchPathNodeID nSearchPathNodeID, SceneWayInfoID nSceneWayInfoID);
	void		AddFirstSearchPathNode(SceneWayInfoID nSceneWayInfoID);
	void	AddToOpenList(SceneWayInfoID nSceneWayInfoID);
	void	RemoveFromOpenList(SceneWayInfoID nSceneWayInfoID);
	bool	IsInOpenList(SceneWayInfoID nSceneWayInfoID);
	bool	IsOpenListEmpty(void) { return m_aOpenList.size() == 0; }
	void	AddToCloseList(SceneWayInfoID nSceneWayInfoID);
	bool	IsInCloseList(SceneWayInfoID nSceneWayInfoID);
	bool	IsCloseListEmpty(void) { return m_aCloseList.size() > 0; }
	SceneWayInfoID	GetSceneWayInfoIDFromSearchPathNodeID(SearchPathNodeID nID);
	SceneWayInfoID	GetSceneWayInfoIDFromSceneTransferDataID(SceneTransferDataID nID);
	
	bool	IsValidSceneID(INT nSceneID);
	bool	IsEnd(SearchPathNodeID nShortest, INT nDestScene);

private:
	//���������Ϣ����
	SceneTransferData	*m_pTransferData;
	SceneWayInfo			*m_pSceneWayInfo;
	INT								  m_nMaxScene, m_nMaxData;

	//�����㷨����
	SearchPathNode		m_aSearchPathNode[MAX_PATH_NODE];
	SearchPathMap		m_aSearchPathD;
	StateSet						m_aOpenList;
	StateSet						m_aCloseList;
	INT								m_nCurrentSearchPos;
};

extern ShortestPath	g_ShortestPath;
