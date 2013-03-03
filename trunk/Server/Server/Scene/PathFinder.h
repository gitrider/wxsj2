//-----------------------------------------------------------------------------
// Game Server , Route Module
// Copyright (C)  Inc.
//-----------------------------------------------------------------------------

#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "Type.h"

//AI�������߸�����Ŀ
#define MAX_ROUTES 1024 * 2
#define EIGHT_DIRECTIONS 8

class Map;

class PathFinder
{
private:
	struct _D2XZLOOKUP
	{
		WORD costmultiplier;
		short zx;
		BYTE route;
	} DZX[EIGHT_DIRECTIONS];

	struct AIROUTE
	{
		WORD count;//������0ʱ,�������ݲ�������

		//������ʼ��
		WORD walkpoint;
		UINT startzx;
		UINT endzx;

		BYTE route[MAX_ROUTES]; 
	};

	struct _WORLD
	{
		BYTE state : 4; 
		BYTE route : 4; 
	};

	struct _NODES
	{
		WORD f; 
		WORD g; 

		UINT zx;
	};
	
	struct _NAVMAP_HEAD 
	{
		WORD width;
		WORD height;
	};	


	enum
	{
		EMPTY_NODE = 0,
		MIN_NODE   = 1,
		NO_ROUTE   = EIGHT_DIRECTIONS,
		MAX_HEAP_LEAFS = MAX_ROUTES,
	};

	enum
	{
		UNKNOWN = 0,
		IMPASSABLE = 1,
		OPEN = 2,
		CLOSED = 3,
	};

	enum
	{
		FINDING    = 0,
		NO_PATH    = 1,
		PATH_FOUND = 2,
	};
	enum
	{
		ROOT_HEAP = 1
	};

private:
#ifdef _DEBUG
	CHAR mFileName[MAX_FILE_PATH];
	Map* mOwner;
#endif
	//����ͼ�ĺ��������ĳߴ�
	INT mWidth;
	INT mHeight;
	//һ�����ӵĳߴ�
	FLOAT mGridSize;
	FLOAT mInvGridSize;
	//��׼��λ��
	FLOAT mLeftTopx;
	FLOAT mLeftTopz;

	_WORLD* mWorld;
	_WORLD* mWorkWorld;
	_NODES* mNodes;

	WORD mLastHeapLeaf;
	WORD mHeap[MAX_HEAP_LEAFS];
	
	WORD mBestFNode;
	WORD mFreeNode;
	INT mMaxNode;

	UINT mStartzx,mEndzx;
	WORD mStartx,mStartz,mEndx,mEndz;
	FLOAT mfStartX,mfStartZ,mfEndX,mfEndZ;

	//ֱ��У��
	INT mCallTimes;
	INT	mGridNum;
	INT mGrids[MAX_ROUTES];

	WORLD_POS mFirstCanGoPos;
	INT mDistance;
	INT curStep;

public:
	PathFinder(Map* owner,const CHAR* filename,uint& mx,uint& mz);
	virtual ~PathFinder();

	VOID Reset(INT startz,INT endz);
	//ǰ��������Ϊ����,3/4����Ϊ���,
	BOOL FindPath(WORLD_POS* startPt,WORLD_POS* endPos,WORLD_POS* posNode,INT& numNode,BOOL bLine = FALSE,INT maxRoute = MAX_ROUTES);
	VOID PackRoute(WORLD_POS* posNode,INT& numNode);
	INT GetDistance();
	BOOL IsCanGo(const WORLD_POS& pos);

private:
	inline WORD Distance(const UINT zx);	
	inline WORD RealDistance(const UINT zxFirst,const UINT zxSecond);
	inline INT LEFT(INT k);
	inline INT RIGHT(INT k);
	inline INT PARENT(INT k);
	inline BOOL NOTEMPTY_UP(INT k);
	inline BOOL NOTEMPTY_DOWN(INT k);
	inline VOID SwapHeap(const INT k1, const INT k2);

	VOID InsertNodeToHeap(WORD node);
	VOID RemoveRootFromHeap();

	VOID ReadNavMap(const CHAR* filename,uint& mx,uint& mz);

	VOID InitEightDirections();
	BOOL IsStraightLine(FLOAT mAStarBeginPtx,FLOAT mAStarBeginPty,FLOAT mAStarEndPtx,FLOAT mAStarEndPty,BOOL edit);
	VOID EditAStarPath(FLOAT startPtx,FLOAT startPty,WORLD_POS* posNode,INT& numNode);
	VOID EditAStarPathOpt(FLOAT startPtx,FLOAT startPty,WORLD_POS* posNode,INT& numNode);
};

#endif
