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
	//m_pSceneWayInfo��SceneIDΪ�±�,���Կ�������ʹ��
	if(nID >= 0 && nID <m_nMaxData)
	{
		return m_pTransferData[nID].nDestSceneID;
	}
	return -1;
}

bool	ShortestPath::FindPath(INT nDestScene,  INT nSrcScene, FindResult& rOutPath)
{
	//������Ч�ĳ���,��ô����ʧ��
	if ( !IsValidSceneID(nDestScene) || !IsValidSceneID(nSrcScene))
	{
		return false;
	}

	//���ԭ������OpenList��
	AddToOpenList(nSrcScene);
	//���
	AddFirstSearchPathNode(nSrcScene);

	//ѭ������,ֱ���ҵ����������Ϊֹ
	bool bFind = false;
	SearchPathNodeID nShortesID = -1;
	while( !IsOpenListEmpty() && !bFind && m_aSearchPathD.size() > 0)
	{
		//�ҵ�������С��
		nShortesID = m_aSearchPathD.begin()->second;
		//Ȼ��ɾ��֮
		m_aSearchPathD.erase(m_aSearchPathD.begin());

		//����Ƿ����յ�
		if(  IsEnd(nShortesID, nDestScene) )
		{
			bFind = true;
			break;
		}

		//������·�߽���
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
				//���·��
				rOutPath.push_back(m_pTransferData[nID]);
			}
		}
	}
	return bFind;
}

//�㷨����

//��ĳ�����������е�·�����뵽�б�����
INT		ShortestPath::AddSearchPathNode(SearchPathNodeID nSearchPathNodeID, SceneWayInfoID nSceneWayInfoID)
{
	//���Ѿ�ʹ���˵ķ�Χ��
	if(nSearchPathNodeID >=0 && nSearchPathNodeID < m_nCurrentSearchPos && IsValidSceneID(nSceneWayInfoID))
	{
		SearchPathNode node;
		//����һ��
		node = m_aSearchPathNode[nSearchPathNodeID];
		//ɾ�������뵽Close����
		RemoveFromOpenList(nSceneWayInfoID);
		AddToCloseList(nSceneWayInfoID);
		//�˸�����
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
				//���뵽OpenList��
				SceneWayInfoID wayID = GetSceneWayInfoIDFromSceneTransferDataID(m_pSceneWayInfo[nSceneWayInfoID].nWayID[i]);
				AddToOpenList(wayID);
				//��ӵ�Ȩ���б���,Ŀ����Ϊ������,�ҵ�Ȩ����С�ľ��ǵ�һ��
				m_aSearchPathD.insert(std::make_pair(node.nWeight, m_nCurrentSearchPos++));
				//ɾ���ղ���ӵ�,Ȼ�����ʹ��
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
	//�����ظ����
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
	//�����ظ����
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