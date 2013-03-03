#include "StdAfx.h"
#include "ShortestPath.h"

ShortestPath	g_ShortestPath;

ShortestPath::ShortestPath(void)
{
	m_pTransferData = 0;
	m_pSceneWayInfo =0;
	m_nMaxScene = 0;
	m_nMaxData =0;
	m_nCurrentSearchPos = 0;
}

ShortestPath::~ShortestPath(void)
{
	if(m_pSceneWayInfo)
	{
		delete[] m_pSceneWayInfo;
		m_pSceneWayInfo = NULL;
	}

	if(m_pTransferData)
	{
		delete[] m_pTransferData;
		m_pTransferData = NULL;
	}
}


void	ShortestPath::Initailize(INT maxData, INT maxScene)
{
	if (maxData>0 && maxScene > 0)
	{
		m_pTransferData = new SceneTransferData[maxData];
		m_pSceneWayInfo = new SceneWayInfo[maxScene];
		m_nMaxScene = maxScene;
		m_nMaxData = maxData;
	}
	else
	{
		throw "ShortestPath::Initailize Invalid Params";
	}
}
void	ShortestPath::AddSceneTransData(const SceneTransferData& data)
{
	static INT nPos = 0;
	if( nPos < m_nMaxData)
	{
		m_pTransferData[nPos++] = data;
		if(data.nSceneID < m_nMaxScene)
		{
			if( !m_pSceneWayInfo[data.nSceneID].Add(nPos-1) )
			{
				throw "ShortestPath::AddSceneTransData m_pSceneWayInfo[data.nSceneID].Add return false";
			}
			m_pSceneWayInfo[data.nSceneID].SetValid(true);
		}
		else
		{
			throw "ShortestPath::AddSceneTransData ";
		}
	}
	else
	{
		throw "ShortestPath::AddSceneTransData out bound of m_pTransferData";
	}
}

bool	ShortestPath::IsValidSceneID(INT nSceneID)
{
	if (nSceneID >-1 && nSceneID <m_nMaxScene)
	{
		//if (m_pSceneWayInfo[nSceneID].IsValid())
		{
			return true;
		}
	}
	return false;
}

bool	ShortestPath::IsEnd(SearchPathNodeID nShortesID, INT nDestScene)
{
	SceneTransferDataID nLastID = m_aSearchPathNode[nShortesID].GetLastID();
	if(nLastID < 0)
	{
		return false;
	}
	return m_pTransferData[nLastID].nDestSceneID == nDestScene;
}

SceneWayInfoID	ShortestPath::GetSceneWayInfoIDFromSearchPathNodeID(SearchPathNodeID nID)
{
	if(nID >= 0 && nID <MAX_PATH_ID_ARRAY)
	{
		SceneTransferDataID nTransID = m_aSearchPathNode[nID].GetLastID();
		if ( nTransID > -1 && nTransID <m_nMaxData)
		{
			return m_pTransferData[nTransID].nDestSceneID;
		}
	}
	return -1;
}

SceneWayInfoID	ShortestPath::GetSceneWayInfoIDFromSceneTransferDataID(SceneTransferDataID nID)
{
	//m_pSceneWayInfo以SceneID为下标,所以可以这样使用
	if(nID >= 0 && nID <m_nMaxData)
	{
		return m_pTransferData[nID].nDestSceneID;
	}
	return -1;
}

bool	ShortestPath::FindPath(INT nDestScene,  INT nSrcScene, FindResult& rOutPath)
{
	//不是有效的场景,那么返回失败
	if ( !IsValidSceneID(nDestScene) || !IsValidSceneID(nSrcScene))
	{
		return false;
	}

	//添加原场景到OpenList里
	AddToOpenList(nSrcScene);
	//添加
	AddFirstSearchPathNode(nSrcScene);

	//循环查找,直到找到或遍历完了为止
	bool bFind = false;
	SearchPathNodeID nShortesID = -1;
	while( !IsOpenListEmpty() && !bFind && m_aSearchPathD.size() > 0)
	{
		//找到代价最小的
		nShortesID = m_aSearchPathD.begin()->second;
		//然后删除之
		m_aSearchPathD.erase(m_aSearchPathD.begin());

		//检查是否是终点
		if(  IsEnd(nShortesID, nDestScene) )
		{
			bFind = true;
			break;
		}

		//添加最佳路线进入
		SceneWayInfoID nID = GetSceneWayInfoIDFromSearchPathNodeID(nShortesID);
		if( nID < 0 ) break;
		AddSearchPathNode(nShortesID, nID);
	}
	if (bFind)
	{
		if(nShortesID >=0 && nShortesID < m_nCurrentSearchPos )
		{
			rOutPath.clear();
			for (INT i=0; i<MAX_PATH_ID_ARRAY; i++)
			{
				SceneTransferDataID nID = m_aSearchPathNode[nShortesID].nPathIDArray[i];
				if (nID<0)
				{
					break;
				}
				//添加路径
				rOutPath.push_back(m_pTransferData[nID]);
			}
		}
	}
	return bFind;
}

//算法函数

//把某个场景的所有的路径加入到列表里面
INT		ShortestPath::AddSearchPathNode(SearchPathNodeID nSearchPathNodeID, SceneWayInfoID nSceneWayInfoID)
{
	//在已经使用了的范围内
	if(nSearchPathNodeID >=0 && nSearchPathNodeID < m_nCurrentSearchPos && IsValidSceneID(nSceneWayInfoID))
	{
		SearchPathNode node;
		//拷贝一份
		node = m_aSearchPathNode[nSearchPathNodeID];
		//删除并加入到Close里面
		RemoveFromOpenList(nSceneWayInfoID);
		AddToCloseList(nSceneWayInfoID);
		//八个方向
		for (INT i=0; i<MAX_WAY_ONE_SCENE; i++)
		{
			if( m_pSceneWayInfo[nSceneWayInfoID].nWayID[i] != -1 )
			{
				node.AddPathID(m_pSceneWayInfo[nSceneWayInfoID].nWayID[i] );
				node.AddWeight();
				if (m_nCurrentSearchPos < MAX_PATH_NODE)
				{
					m_aSearchPathNode[m_nCurrentSearchPos] = node;
				}
				//加入到OpenList里
				SceneWayInfoID wayID = GetSceneWayInfoIDFromSceneTransferDataID(m_pSceneWayInfo[nSceneWayInfoID].nWayID[i]);
				AddToOpenList(wayID);
				//添加到权重列表里,目的是为了排序,找到权重最小的就是第一个
				m_aSearchPathD.insert(std::make_pair(node.nWeight, m_nCurrentSearchPos++));
				//删除刚才添加的,然后继续使用
				node.RemoveLastAddID();
				node.AddWeight(-1);
			}
			else
			{
				return i;
			}
		}
	}
	return 0;
}

void		ShortestPath::AddFirstSearchPathNode(SceneWayInfoID nSceneWayInfoID)
{
	m_nCurrentSearchPos = 0;
	m_aSearchPathNode[m_nCurrentSearchPos].Clear();
	m_aSearchPathD.clear();
	m_aSearchPathNode[m_nCurrentSearchPos].MarkUse();
	//m_aSearchPathD.insert(std::make_pair(m_aSearchPathNode[m_nCurrentSearchPos].nWeight, m_nCurrentSearchPos));
	++m_nCurrentSearchPos;
	AddSearchPathNode(0,nSceneWayInfoID);
}

void	ShortestPath::AddToOpenList(SceneWayInfoID nSceneWayInfoID)
{
	//不能重复添加
	if( std::find(m_aOpenList.begin(), m_aOpenList.end(), nSceneWayInfoID) != m_aOpenList.end() )
	{
		return;
	}
	m_aOpenList.push_back(nSceneWayInfoID);
}
void	ShortestPath::RemoveFromOpenList(SceneWayInfoID nSceneWayInfoID)
{
	StateSet::iterator it = m_aOpenList.begin();
	StateSet::iterator ed = m_aOpenList.begin();
	for (; it != ed; it++)
	{
		if( (*it) == nSceneWayInfoID )
		{
			m_aOpenList.erase(it);
			return;
		}
	}
}
bool	ShortestPath::IsInOpenList(SceneWayInfoID nSceneWayInfoID)
{
	StateSet::iterator it = m_aOpenList.begin();
	StateSet::iterator ed = m_aOpenList.begin();
	for (; it != ed; it++)
	{
		if( (*it) == nSceneWayInfoID )
		{
			return true;
		}
	}
	return false;
}
void	ShortestPath::AddToCloseList(SceneWayInfoID nSceneWayInfoID)
{
	//不能重复添加
	if( std::find(m_aCloseList.begin(), m_aCloseList.end(), nSceneWayInfoID) != m_aCloseList.end() )
	{
		return;
	}
	m_aCloseList.push_back(nSceneWayInfoID);
}
bool	ShortestPath::IsInCloseList(SceneWayInfoID nSceneWayInfoID)
{
	StateSet::iterator it = m_aCloseList.begin();
	StateSet::iterator ed = m_aCloseList.begin();
	for (; it != ed; it++)
	{
		if( (*it) == nSceneWayInfoID )
		{
			return true;
		}
	}
	return false;
}