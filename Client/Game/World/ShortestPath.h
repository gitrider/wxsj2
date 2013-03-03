/***********************************************************
作者: 方远
日期: 2010-04-19
说明: 跨场景寻路算法
************************************************************/

#pragma once

#include <map>

//场景传送点结构
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

//定义每个场景最多的通路个数
#define MAX_WAY_ONE_SCENE	8

//每个场景的路信息,场景传送点的集合
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

//一条搜索路径的最大结点长度
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
			//出现循环错误
			if (nPathIDArray[i] == nID)
			{
				return false;
			}
			//找到空白的,然后添加进去
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
//系统里每次检索最多有多少的条通路
#define MAX_PATH_NODE	1024

//定义搜索的路径结点ID
typedef int	SearchPathNodeID;

//first代表权重,越小,表示是最短路径,second代表SearchPathNode的索引,是一个通路的索引
typedef std::multimap<int, SearchPathNodeID>	SearchPathMap;

typedef std::vector<SceneTransferData> FindResult;

//状态集合
typedef std::vector<SceneWayInfoID>	StateSet;

class ShortestPath
{
public:
	ShortestPath(void);
	~ShortestPath(void);

public:
	//接口函数
	void	Initailize(INT maxData, INT maxScene);
	void	AddSceneTransData(const SceneTransferData& data);
	bool	FindPath(INT nDestScene,  INT nSrcScene, FindResult& rOutPath);


protected:
	//算法函数
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
	//定义基本信息数据
	SceneTransferData	*m_pTransferData;
	SceneWayInfo			*m_pSceneWayInfo;
	INT								  m_nMaxScene, m_nMaxData;

	//定义算法数据
	SearchPathNode		m_aSearchPathNode[MAX_PATH_NODE];
	SearchPathMap		m_aSearchPathD;
	StateSet						m_aOpenList;
	StateSet						m_aCloseList;
	INT								m_nCurrentSearchPos;
};

extern ShortestPath	g_ShortestPath;
