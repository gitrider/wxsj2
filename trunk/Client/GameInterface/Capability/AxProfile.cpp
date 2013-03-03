
#include "AxProfile.h"
#include <windows.h>
#include <string>
#include <list>
#include <hash_map>
#include <assert.h>



//////////////////////////////////////////
//
 #define ENABLE_AXPROFILE
//
//////////////////////////////////////////


#define HashMap ::stdext::hash_map


namespace AxProfile
{

	////////////////////////////////////////////////////////////////

	//Ч�ʽڵ�
	struct NodeDefine
	{
		STRING			strName;
		NodeDefine		*pParent;
	};

	//�ڵ�洢�ṹ
	typedef HashMap< STRING, NodeDefine >	NodeDefineMap;

	//һ�β����еĽڵ�
	struct NodeInSampling
	{
		NodeDefine*			pNodeDefine;		//��Buf�е�ָ��
		LARGE_INTEGER		nStartTime;			//��ʼʱ��
		LARGE_INTEGER		nEndTime;			//����ʱ��
	};

	//�����ڵ�洢�ṹ
	typedef HashMap< NodeDefine*, NodeInSampling > NodeInSamplingMap;

	//һ�β���
	struct Sampling
	{
		bool					bInvalid;			//�Ƿ���Ч
													/*
													| ���¼���������β���Ϊ��Ч;
													|	1. ĳ�ڵ�û�б����;
													|
													|
													*/
		LARGE_INTEGER			nStartTime;			//��ʼʱ��
		LARGE_INTEGER			nEndTime;			//����ʱ��
		NodeInSamplingMap		mapSamplingNode;	//�ɼ���Ч�ʽڵ�
	};

	//���������ṹ
	typedef std::list< Sampling >	SamplingList;


	////////////////////////////////////////////////////////////////

	//���е�Ч�ʽڵ��б�
	NodeDefineMap		g_AllNode;
	//�������ݴ洢
	SamplingList		g_SamplingList;
	//��ǰ�Ĳ���
	Sampling*			g_pCurrentSampling = NULL;

	////////////////////////////////////////////////////////////////

	//�رյ�ǰ�ڵ�
	void	_TryCloseCurrentSampling(void);

	////////////////////////////////////////////////////////////////
	void AxProfile_SamplingBegin(void)
	{
	#ifdef ENABLE_AXPROFILE
		//��ǰʱ��
		LARGE_INTEGER tNow;
		::QueryPerformanceCounter(&tNow);

		//�رյ�ǰ����
		if(g_pCurrentSampling) _TryCloseCurrentSampling();

		//��ʼ�²���
		Sampling newSampling;
		newSampling.bInvalid = false;
		newSampling.nStartTime = tNow;
		newSampling.nEndTime.QuadPart = 0;

		g_SamplingList.push_back(newSampling);

		//���õ�ǰ����
		g_pCurrentSampling = &(*(g_SamplingList.rbegin()));
	#endif
	}

	void AxProfile_SamplingEnd(void)
	{
	#ifdef ENABLE_AXPROFILE
		//�رյ�ǰ����
		if(g_pCurrentSampling) _TryCloseCurrentSampling();
	#endif
	}

	void AxProfile_PushNode(const char* szName, const char* szParent)
	{
	#ifdef ENABLE_AXPROFILE
		//����Ƿ�����Ч������
		if(!szName || szName[0] == '\0') 
		{
			assert(false && "AxProfile_PushNode: Invalid Node Name!");
			return;
		}

		//��ǰ��δ��ʼ����
		if(!g_pCurrentSampling)
		{
			assert(false && "AxProfile_PushNode: Must in the Pair of StartSampling and EndSampling!");
			return;
		}

		//��ǰʱ��
		LARGE_INTEGER tNow;
		::QueryPerformanceCounter(&tNow);

		//�������нڵ�
		NodeDefineMap::iterator itFind = g_AllNode.find(szName);
		if(itFind == g_AllNode.end())
		{
			//�����½ڵ㶨��
			NodeDefine newNode;
			newNode.strName = szName;

			//�������ڵ�
			if(!szParent || szParent[0] == '\0')
			{
				newNode.pParent = NULL;
			}
			else
			{
				NodeDefineMap::iterator itParent = g_AllNode.find(szName);
				if(itParent == g_AllNode.end())
				{
					assert(false && "AxProfile_PushNode: Invalid Parent Name!");
					return;
				}

				newNode.pParent = &(itParent->second);
			}

			//���붨��
			g_AllNode.insert(std::make_pair(newNode.strName, newNode));

			itFind = g_AllNode.find(szName);
		}

		//Ѱ�Ҹýڵ��Ƿ��ڲ�����
		NodeInSamplingMap::iterator itNodeInSampling = 
			g_pCurrentSampling->mapSamplingNode.find(&(itFind->second));

		if(itNodeInSampling == g_pCurrentSampling->mapSamplingNode.end())
		{
			//�����½ڵ�
			NodeInSampling newNode;
			newNode.nStartTime = tNow;
			newNode.nEndTime.QuadPart = 0;
			newNode.pNodeDefine = &(itFind->second);

			g_pCurrentSampling->mapSamplingNode.insert(std::make_pair(newNode.pNodeDefine, newNode));
		}	
		else
		{
			// �����Ѿ����ڲ����еĽڵ㣬�����ظ���ʱ
			//...
		}
	#endif
	}

	void AxProfile_PopNode(const char* szName)
	{
#ifdef ENABLE_AXPROFILE

		// ����Ƿ�����Ч������
		if(!szName || szName[0] == '\0') 
		{
			assert(false && "AxProfile_PopNode: Invalid Node Name!");
			return;
		}

		// ��ǰ��δ��ʼ����
		if(!g_pCurrentSampling)
		{
			assert(false && "AxProfile_PopNode: Must in the Pair of StartSampling and EndSampling!");
			return;
		}

		// ��ǰʱ��
		LARGE_INTEGER tNow;
		::QueryPerformanceCounter(&tNow);

		// �������нڵ�
		NodeDefineMap::iterator itFind = g_AllNode.find(szName);
		if(itFind == g_AllNode.end())
		{
			// û���ҵ��ڵ㶨��
			assert(false && "AxProfile_PopNode: Muse Push Node First!");
			return;
		}

		// Ѱ�Ҹýڵ��Ƿ��ڲ�����
		NodeInSamplingMap::iterator itNodeInSampling = g_pCurrentSampling->mapSamplingNode.find(&(itFind->second));
		if(itNodeInSampling == g_pCurrentSampling->mapSamplingNode.end())
		{
			// û���ҵ��ڵ㶨��
			assert(false && "AxProfile_PopNode: Muse Push Node First!");
			return;
		}

		// �趨ֹͣʱ��
		itNodeInSampling->second.nEndTime = tNow;

		//�Ƿ�ﵽ�������
		if(g_SamplingList.size() >= 1000)
		{
			g_SamplingList.erase(g_SamplingList.begin());
		}

#endif
	}

	////////////////////////////////////////////////////////////////
	void _TryCloseCurrentSampling(void)
	{
#ifdef ENABLE_AXPROFILE

		if(!g_pCurrentSampling) return;

		//��ǰʱ��
		LARGE_INTEGER tNow;
		::QueryPerformanceCounter(&tNow);

		g_pCurrentSampling->nEndTime = tNow;

		//����Ƿ�����Ч�ʵ��Ѿ����
		register HashMap< NodeDefine*, NodeInSampling >::iterator it = g_pCurrentSampling->mapSamplingNode.begin();
		HashMap< NodeDefine*, NodeInSampling >::iterator itEnd = g_pCurrentSampling->mapSamplingNode.end();

		g_pCurrentSampling->bInvalid = true;
		for(;it!=itEnd; ++it)
		{
			register NodeInSampling& theNode = it->second;

			if(theNode.nEndTime.QuadPart == 0)
			{
				//�нڵ�δ���
				g_pCurrentSampling->bInvalid = false;
				break;
			}
		}

		//��յ�ǰ�ڵ�
		g_pCurrentSampling = NULL;

#endif
	}

	void AxProfile_Report(void)
	{
#ifdef ENABLE_AXPROFILE

		FILE* fp = fopen("E:\\Report.txt", "w");
		if(!fp) return;

		LARGE_INTEGER gPer;
		QueryPerformanceFrequency(&gPer);

		//printf title
		fprintf(fp, "#\t_TOTAL_\t");
		NodeDefineMap::iterator itNodeDefine = g_AllNode.begin();
		for(;itNodeDefine!=g_AllNode.end(); itNodeDefine++)
		{
			fprintf(fp, "%s\t", itNodeDefine->second.strName.c_str());
		}
		fprintf(fp, "\n");

		int nIndex=0;
		std::list< Sampling >::iterator itSampling = g_SamplingList.begin();
		for(;itSampling!=g_SamplingList.end(); itSampling++)
		{
			fprintf(fp, "%d\t", nIndex);
			Sampling& theSampling = *itSampling;
			if(!theSampling.bInvalid) 
				continue;

			//printf total
			fprintf(fp, "%f\t", (theSampling.nEndTime.QuadPart-theSampling.nStartTime.QuadPart)*1000.0/gPer.QuadPart);

			itNodeDefine = g_AllNode.begin();
			for(;itNodeDefine!=g_AllNode.end(); itNodeDefine++)
			{
				NodeDefine* pNodeDefine = &(itNodeDefine->second);
				
				NodeInSamplingMap::iterator itNodeInSampling = theSampling.mapSamplingNode.find(pNodeDefine);
				if(itNodeInSampling == theSampling.mapSamplingNode.end())
				{
					fprintf(fp, "0\t");
				}
				else
				{
					NodeInSampling& theSamplingNode = itNodeInSampling->second;
	                
					fprintf(fp, "%f\t", (theSamplingNode.nEndTime.QuadPart-theSamplingNode.nStartTime.QuadPart)*1000.0/gPer.QuadPart);
				}
			}

			fprintf(fp, "\n");

			nIndex++;
		}

		fclose(fp);


		g_SamplingList.clear();
		g_pCurrentSampling = 0;

#endif
	}

}