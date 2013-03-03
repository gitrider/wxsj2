#include "stdafx.h"
#include "ItemHelper.h"
#include "ItemTable.h"
#include "WXSJ_DBC.h"
#include "ItemHelper.h"
#include "Log.h"
#include "Obj_ItemBox.h"
#include "ItemManager.h"
#include "FileDef.h"
#include "SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"

ItemTable	g_ItemTable;

using namespace DBC;


#define SAFE_VALUE(x) ((x>0) && (x!=-1))?x:0

INT CompareMonsterDropTB(const VOID* pArg1,const VOID* pArg2)
{	
	__ENTER_FUNCTION

	MONSTER_DROP_TB* pTB1	=	(MONSTER_DROP_TB*)pArg1;
	MONSTER_DROP_TB* pTB2	=	(MONSTER_DROP_TB*)pArg2;

	if(pTB1->m_DropId>pTB2->m_DropId)
		return 1;
	else if(pTB1->m_DropId<pTB2->m_DropId)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

//----------------------------------------------------------
INT CompareEquipTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		GUID_t	guid1,guid2;

	guid1	= ((EQUIP_TB*)pArg1)->m_IndexID;
	guid2	= ((EQUIP_TB*)pArg2)->m_IndexID;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

INT CompareDropAttTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT iDelta1,iDelta2;

	iDelta1		=	((DROP_ATT_TB*)pArg1)->m_DeltaLevel;
	iDelta2		=	((DROP_ATT_TB*)pArg2)->m_DeltaLevel;

	if(iDelta1>iDelta2)
		return	 1;
	else if(iDelta1<iDelta2)
	    return	-1;
	else	
		return	 0;
	__LEAVE_FUNCTION	
	return 0 ;
}



INT CompareGemInfoTBByTI(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT	TableIndex1,TableIndex2;

	TableIndex1	= ((GEMINFO_TB*)pArg1)->m_nTableIndex;
	TableIndex2	= ((GEMINFO_TB*)pArg2)->m_nTableIndex;

	if(TableIndex1>TableIndex2)
		return 1;
	else if(TableIndex1<TableIndex2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareEquipSetTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT Serial1,Serial2;

	Serial1	=	((EQUIP_SET_TB*)pArg1)->m_nEquipSetSerial;
	Serial2	=	((EQUIP_SET_TB*)pArg2)->m_nEquipSetSerial;

	if(Serial1>Serial2)
		return 1;
	else if(Serial1<Serial2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareExtraBagInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		GUID_t	guid1,guid2;

	guid1	= ((EXTRABAG_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((EXTRABAG_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

INT CompareMountInfoTB(const VOID* pArg1,const VOID* pArg2)        // [7/8/2010 陈军龙]
{
	__ENTER_FUNCTION

		GUID_t	guid1,guid2;

	guid1	= ((MOUNT_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((MOUNT_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

INT CompareChangePaiInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		GUID_t	guid1,guid2;

	guid1	= ((CHANGEPAI_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((CHANGEPAI_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

INT CompareCommItemInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((COMMITEM_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((COMMITEM_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareSpecialRepairInfoTBByTI(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		INT	TableIndex1,TableIndex2;

	TableIndex1	= ((EQUIP_SPECIALREPAIR_TB*)pArg1)->m_ItemID;
	TableIndex2	= ((EQUIP_SPECIALREPAIR_TB*)pArg2)->m_ItemID;

	if(TableIndex1>TableIndex2)
		return 1;
	else if(TableIndex1<TableIndex2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}


INT CompareProductInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	UINT	Serial1,Serial2;

	Serial1	= ((ITEM_PRODUCT_INFO_TB*)pArg1)->m_ProductID;
	Serial2	= ((ITEM_PRODUCT_INFO_TB*)pArg2)->m_ProductID;

	if(Serial1>Serial2)
		return 1;
	else if(Serial1<Serial2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}


INT CompareTaskItemInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		GUID_t	guid1,guid2;

	guid1	= ((TASKITEM_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((TASKITEM_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

INT	CompareStoreMapInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((STORE_MAP_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((STORE_MAP_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}
// add by gh for souxia 2010/05/05
INT	CompareInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((SOUXIA_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((SOUXIA_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}
// end of add
ItemTable::ItemTable( )
{
	m_pEquipTableData				=   NULL;
	m_pAttLevelTableData			=   NULL;
	m_pEquipAttSetTableData			=	NULL;
	

	m_pTaskItemInfoData				=   NULL;	

	m_pDropBoxTableData				=	NULL;
	m_pMonsterDropTableData			=	NULL;
	m_pGemInfoData					=	NULL;
	m_EquipHoleInfoData				=   NULL;
	m_EquipLeveupInfoData			=	NULL;
	m_EquipBindInfoData				=	NULL;
	m_pProductInfoData				=	NULL;
	m_EquipSpecialRepairData		=   NULL;
	m_MaterialLevelInfoData			=   NULL;
	m_EquipExtraInfoData			=   NULL;	
	m_pExtraBagInfoData				=   NULL;
	m_EquipChainInfoData			=   NULL;
	m_EquipSoulInfoData				=   NULL;
	m_pChangePaiInfoData			=	NULL;
	
	m_pCommItemInfoData				=	NULL;
	m_pDropAttData					=	NULL;
	m_pDropPosData					=	NULL;

	m_pEquipSetData					=	NULL;
	m_pItemRulerData				=	NULL;
	m_pStoreMapData					=	NULL;
	m_pSouXiaData					=	NULL;
	m_pSoulBeadData				=	NULL;

	memset(m_nEquipPointInfo, 0, sizeof(UINT)*HEQUIP_NUMBER);
	memset(m_nPaiInfo, 0, sizeof(PAI_INFO)*EQUIP_PAI_NUM);
	memset(m_nZIPai, 0, sizeof(UINT)*EQUIP_PAI_NUM);
	memset(m_nNoZIPai, 0, sizeof(UINT)*EQUIP_PAI_NUM);
}

ItemTable::~ItemTable( )
{
	CleanUp( ) ;
}

VOID	ItemTable::CleanUp( )
{
	__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pEquipTableData ) ;
	SAFE_DELETE_ARRAY( m_pAttLevelTableData ) ;
	SAFE_DELETE_ARRAY( m_pEquipAttSetTableData ) ;
	SAFE_DELETE_ARRAY( m_pTaskItemInfoData );
	SAFE_DELETE_ARRAY( m_pDropBoxTableData ) ;
	SAFE_DELETE_ARRAY( m_pMonsterDropTableData ) ;
	SAFE_DELETE_ARRAY( m_pGemInfoData);
	SAFE_DELETE_ARRAY( m_EquipHoleInfoData);
	SAFE_DELETE_ARRAY( m_EquipLeveupInfoData);
	SAFE_DELETE_ARRAY( m_EquipBindInfoData);
	SAFE_DELETE_ARRAY( m_EquipSpecialRepairData);
	SAFE_DELETE_ARRAY( m_pProductInfoData);
	SAFE_DELETE_ARRAY( m_MaterialLevelInfoData);
	SAFE_DELETE_ARRAY( m_EquipExtraInfoData);
	SAFE_DELETE_ARRAY( m_pExtraBagInfoData);
	SAFE_DELETE_ARRAY( m_EquipChainInfoData);
	SAFE_DELETE_ARRAY( m_EquipSoulInfoData);
	SAFE_DELETE_ARRAY( m_pChangePaiInfoData);
	
	SAFE_DELETE_ARRAY(m_pCommItemInfoData);
	SAFE_DELETE_ARRAY(m_pDropAttData);
	SAFE_DELETE_ARRAY(m_pDropPosData);
	SAFE_DELETE_ARRAY(m_pEquipSetData);
	SAFE_DELETE_ARRAY(m_pItemRulerData);
	SAFE_DELETE_ARRAY(m_pStoreMapData);
	SAFE_DELETE_ARRAY(m_pSouXiaData);
	SAFE_DELETE_ARRAY(m_pSoulBeadData);
	SAFE_DELETE_ARRAY(m_pMountAttInfoData);       // [7/8/2010 陈军龙]

	__LEAVE_FUNCTION	
}



BOOL	ItemTable::Init()
{
	__ENTER_FUNCTION

	m_nHashOffSet			=	0;

	m_nAttLevelCount		=	0;
	m_nEquipAttSetCount		=	0;
	m_nEquipCount			=	0;
	m_nTaskItemCount		=   0;


	m_nDropBoxCount			=	0;
	m_nMonsterDropCount		=	0;
	m_nLeveupInfoCount		=	0;
	m_nEquipBindInfoCount	=	0;
	m_nSpecialRepairCount	=   0;
	m_nProductCount			=	0;
	m_nMaterialLevelInfoCount=  0;
	m_nEquipExtraInfoCount  =   0;
	m_nExtraBagInfoCount	=   0;
	m_nEquipChainInfoCount	=	0;
	m_nEquipSoulInfoCount	=	0;
	m_nChangePaiInfoCount	=	0;

	m_nGemCount				=	0;
	m_nGemCount				=   0;

	m_nCommItemCount		=	0;
	
	m_nDropAttCount			=	0;
	m_nDropPosCount			=   0;
	m_nEquipSetCount		=	0;

	m_nItemRulerCount		=	0;
	m_nStoreMapCount		=	0;

	m_nZiPaiCount			=	0;
	m_nNoZIPaiCount			=   0;
	
	InitEquipTable();
	InitEquipHoleInfoTable();
	InitEquipLevelupInfoTable();
	InitEquipBindInfoTable();
	InitSpecialRepairInfoTable();
	InitAttLevelTable();
	InitEquipATTSetTable();
	InitProductInfoTable();
	InitMaterialLevelInfoTable();
	InitEquipExtraInfoTable();
	InitEquipPointInfoTable();
	InitChangePaiInfoTable();
	InitPaiInfoTable();
	InitEquipChainInfoTable();
	InitEquipSoulInfoTable();
	InitExtraBagInfoTable();

	InitMonsterDropTable();
	InitGemInfoTable();
	
	InitCommItemInfoTable();
	InitTaskItemInfoTable();

	InitDropAttTable();
	InitItemDropPos();
	InitEquipSetAttrTable();
	InitItemRulerTable();
	InitStoreMapTable();
	InitSouXiaTable();
	InitSoulBeadTable();
	InitMountAttrTable();        // [7/8/2010 陈军龙]

	__LEAVE_FUNCTION	

	return	TRUE;
}

//加载属性组表
VOID	ItemTable::InitEquipATTSetTable()
{
	__ENTER_FUNCTION

	enum ATTSetTBData
	{
		TBIndex				= 0,
		TBName				= 1,
		TBProbability		= 2,
		TBReqLevel			,
		TBAttBegin			,
	};


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ATT_SET_BASE );

	INT iTableCount		= ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	m_nEquipAttSetCount			=	iTableCount;
	m_pEquipAttSetTableData		=	new EQUIP_ATT_SET_TB[m_nEquipAttSetCount];

	INT SumRate = 0;
	for(INT i =0;i<iTableCount;i++)
	{
		INT nSetID = ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		Assert(nSetID != i);

		m_pEquipAttSetTableData[i].m_IndexID				= 		nSetID;
		m_pEquipAttSetTableData[i].m_AttSetRate				= 		ThirdFile.Search_Posistion(i,TBProbability)->iValue;
		m_pEquipAttSetTableData[i].m_ReqLevel				= 		ThirdFile.Search_Posistion(i,TBReqLevel)->iValue;
		SumRate											   +=       m_pEquipAttSetTableData[i].m_AttSetRate;
		m_pEquipAttSetTableData[i].m_SumRate				=       SumRate;
		
		INT iATTID		= -1;
		INT	iColumn		= TBAttBegin;
		for(int nAtt=0; nAtt<MAX_EQUIP_EXT_ATTR; ++nAtt)
		{
			iATTID = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			if(iATTID>=0)
			{
				m_pEquipAttSetTableData[i].m_Attr[m_pEquipAttSetTableData[i].m_AttCount]	= (ITEM_ATTRIBUTE)iATTID;
				++m_pEquipAttSetTableData[i].m_AttCount;
			}
			++iColumn;
		}

	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipATTSetTable()  Ok!") ;

	__LEAVE_FUNCTION	
}

//加载属性等级表
VOID	ItemTable::InitAttLevelTable()
{
	__ENTER_FUNCTION

	enum ATTAttLevelData
	{
		TBIndex				= 0,
		TBAttBegin			= 2,
	};


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ATT_LEVEL_BASE );

	INT iTableCount		= ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	m_nAttLevelCount			=	iTableCount;
	m_pAttLevelTableData		=	new EQUIP_ATT_LEVEL_TB[MAX_ATT_COUNT];


	for(INT i =0;i<iTableCount;i++)
	{
		INT iATTID		= -1;
		iATTID				= 		ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pAttLevelTableData[iATTID].m_AttID	= (ITEM_ATTRIBUTE)iATTID;

		INT iATTValue	=  0;
		INT	iColumn		= TBAttBegin;
		for(int nAtt=1; nAtt<= MAX_EQUIP_LEVEL; ++nAtt)
		{
			iATTValue = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			if(iATTValue>0)
			{
				m_pAttLevelTableData[iATTID].m_Attr[nAtt]	= iATTValue;
			}
			iColumn = iColumn + 1;
		}

	}

	Log::SaveLog( SERVER_LOGFILE, "InitAttLevelTable()  Ok!") ;

	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitEquipTable()
{
	__ENTER_FUNCTION

	enum ItemTBData
	{
		TBIndex				=0,	//ID标识
		TBClass				,	//基本类型
		TBType				,	//类别
		TBItemIndex			,	//物品号
		TBEquipPoint		,	//装备点
		TBRulerID			,	//适应规则
		TBSet				,	//装备
		TBReqLevel			,	//(需求等级)
		TBBasePrice			,	//基本价格
		TBMaxDur			,	//Max耐久
		TBLevel				,	//星级
		TBBaseGem			,	//天然孔数目
		TBMaxGem			,	//最大孔数目
		TBReqJob			,	//职业需求
		TBPaiType			,	//牌型
		TBScore				,	//分数

		TBBaseNearAtt		,	//基础远程内功攻击
		TBBaseFarAtt		,	//基础远程外功攻击

		TBBaseMgicNearAtt	,	//基础近程内功攻击
		TBBaseMgicFarAtt	,	//基础近程外功攻击

		TBBaseNearDefence	,	//基础远程内功防御
		TBBaseFarDefence	,	//基础远程外功防御

		TBBaseMagicNearDecence	,//基础近程内功防御
		TBBaseMagicFarDecence	,//基础近程外功防御

		TBAttBegin			,	//影响属性
		TBAFFSkillBegin		=TBAttBegin+MAX_EQUIP_EXT_ATTR,	//附加技能数目
		TBEffSkillBegin		=TBAFFSkillBegin+MAX_EQUIP_AFFIX_SKILL,	//影响技能
	};

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT( FILE_EQUIP_BASE );
	Assert(ret);
	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	m_nEquipCount	    =	iTableCount;
	m_pEquipTableData	=	new EQUIP_TB[iTableCount];


	for(INT i =0;i<iTableCount;i++)
	{
		INT iValue	=	0;
		INT iColumn	=	0;
		
		m_pEquipTableData[i].m_IndexID				= 		ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pEquipTableData[i].m_ItemType.m_Class		= 		ThirdFile.Search_Posistion(i,TBClass)->iValue; 
		m_pEquipTableData[i].m_ItemType.m_Type		= 		ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pEquipTableData[i].m_ItemType.m_Index		= 		ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		m_pEquipTableData[i].m_EquipPoint			= 		ThirdFile.Search_Posistion(i,TBEquipPoint)->iValue;
		m_pEquipTableData[i].m_RulerID				= 		ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pEquipTableData[i].m_EquipSetNum			= 		ThirdFile.Search_Posistion(i,TBSet)->iValue;

		m_pEquipTableData[i].m_BasePrice			= 		ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pEquipTableData[i].m_MaxDur				= 		ThirdFile.Search_Posistion(i,TBMaxDur)->iValue;
		m_pEquipTableData[i].m_Level				= 		ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pEquipTableData[i].m_BaseGemHole			= 		ThirdFile.Search_Posistion(i,TBBaseGem)->iValue;

		INT iMaxGem = ThirdFile.Search_Posistion(i,TBMaxGem)->iValue;
		m_pEquipTableData[i].m_MAXGemHole			= 		SAFE_VALUE(iMaxGem);
		
		{
			//初始化数据
			for( INT nID=0; nID < MATTRIBUTE_NUMBER; ++nID )
			{
				m_pEquipTableData[i].m_PrefessionReq[nID] = MATTRIBUTE_UNKNOW;
			}

			const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,TBReqJob)->pString;

			SplitStringLevelOne pSplitL1;
			SplitStringLevelTwo pSplitL2;
			pSplitL1.Reset();
			pSplitL2.Reset();
			pSplitL1.Init( '|', &pSplitL2 );
			pSplitL2.Init( '~', NULL );
			pSplitL1.DoSplit( pSplitString );
			INT iLineCount	= pSplitL2.GetResultLineCount();

			Assert(iLineCount > 0 && iLineCount <= MATTRIBUTE_NUMBER);

			for( INT nID=0; nID<iLineCount; ++nID )
			{
				m_pEquipTableData[i].m_PrefessionReq[nID] = atoi(pSplitL2.GetResultLine(nID));
				Assert(m_pEquipTableData[i].m_PrefessionReq[nID] >= MATTRIBUTE_SHAOLIN_GWS && m_pEquipTableData[i].m_PrefessionReq[nID] <= MATTRIBUTE_WUMENPAI);
			}

#ifdef CHECK_DATA_TABLE

#endif
		}
		//牌型
		if (ThirdFile.Search_Posistion(i,TBPaiType)->iValue > 0 )
		{
			m_pEquipTableData[i].m_PaiType			= 		ThirdFile.Search_Posistion(i,TBPaiType)->iValue;
		}					
		m_pEquipTableData[i].m_EquipScore			= 		ThirdFile.Search_Posistion(i,TBScore)->iValue;				//装备分
		m_pEquipTableData[i].m_NearAttack			=		ThirdFile.Search_Posistion(i,TBBaseNearAtt)->iValue;
		m_pEquipTableData[i].m_FarAttack			=		ThirdFile.Search_Posistion(i,TBBaseFarAtt)->iValue;

		m_pEquipTableData[i].m_MagicNearAttack		=		ThirdFile.Search_Posistion(i,TBBaseMgicNearAtt)->iValue;
		m_pEquipTableData[i].m_MagicFarAttack		=		ThirdFile.Search_Posistion(i,TBBaseMgicFarAtt)->iValue;

		m_pEquipTableData[i].m_NearDefence			=		ThirdFile.Search_Posistion(i,TBBaseNearDefence)->iValue;
		m_pEquipTableData[i].m_FarDefence			=		ThirdFile.Search_Posistion(i,TBBaseFarDefence)->iValue;

		m_pEquipTableData[i].m_MagicNearDefense		=		ThirdFile.Search_Posistion(i,TBBaseMagicNearDecence)->iValue;
		m_pEquipTableData[i].m_MagicFarDefense		=		ThirdFile.Search_Posistion(i,TBBaseMagicFarDecence)->iValue;

		{
			const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,TBReqLevel)->pString;

			SplitStringLevelOne pSplitL1;
			SplitStringLevelTwo pSplitL2;
			pSplitL1.Reset();
			pSplitL2.Reset();
			pSplitL1.Init( '|', &pSplitL2 );
			pSplitL2.Init( '~', NULL );
			pSplitL1.DoSplit( pSplitString );
			INT iLineCount	= pSplitL2.GetResultLineCount();

			for( INT nID=0; nID<iLineCount; ++nID )
			{
				m_pEquipTableData[i].m_ReqLevel[nID] = atoi(pSplitL2.GetResultLine(nID));
				Assert(m_pEquipTableData[i].m_ReqLevel[nID]<=MAX_EQUIP_LEVEL);
				++m_pEquipTableData[i].m_ReqLevelCount;
			}
			Assert(m_pEquipTableData[i].m_ReqLevelCount>0);
		}

		INT iATTID		= -1;
		INT iATTValue	= -1;
		iColumn	= TBAttBegin;
		for(int nAtt=0; nAtt<MAX_EQUIP_EXT_ATTR; ++nAtt)
		{
			iATTID = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			++iColumn;
			iATTValue = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			if((iATTID>=0)&&(iATTValue>0))
			{
				m_pEquipTableData[i].m_Attr[m_pEquipTableData[i].m_AttCount].m_AttrType			=	(ITEM_ATTRIBUTE)iATTID;
				m_pEquipTableData[i].m_Attr[m_pEquipTableData[i].m_AttCount].m_Value.m_Value	=	iATTValue;
				++m_pEquipTableData[i].m_AttCount;
			}
			++iColumn;
		}
		
		INT iSkillID	= -1;
		INT nSkillLevel = 0;
		iColumn	= TBAFFSkillBegin;
		for(int nSkill=0; nSkill<MAX_EQUIP_AFFIX_SKILL; ++nSkill)
		{
			iSkillID = ThirdFile.Search_Posistion(i, iColumn)->iValue; 
			iColumn = iColumn + 1;
			nSkillLevel = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			if((nSkillLevel>0)&&(iSkillID>0))
			{
				m_pEquipTableData[i].m_AffixSkill[m_pEquipTableData[i].m_AffSkillCount].m_nSkillID				=	iSkillID;
				m_pEquipTableData[i].m_AffixSkill[m_pEquipTableData[i].m_AffSkillCount].m_nLevel				=	nSkillLevel;
				++m_pEquipTableData[i].m_AffSkillCount;
			}
		}

		iSkillID	= -1;
		nSkillLevel = 0;
		iColumn	= TBAFFSkillBegin;
		for(int nSkill=0; nSkill<MAX_EQUIP_EFFECT_SKILL; ++nSkill)
		{
			iSkillID = ThirdFile.Search_Posistion(i, iColumn)->iValue; 
			iColumn = iColumn + 1;
			nSkillLevel = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
			if((nSkillLevel>0)&&(iSkillID>0))
			{
				m_pEquipTableData[i].m_EffectSkill[m_pEquipTableData[i].m_EffSkillCount].m_nSkillID				=	iSkillID;
				m_pEquipTableData[i].m_EffectSkill[m_pEquipTableData[i].m_EffSkillCount].m_nLevel				=	nSkillLevel;
				++m_pEquipTableData[i].m_EffSkillCount;
			}
		}
	}
	Log::SaveLog( SERVER_LOGFILE, "InitEquipTable()  Ok!") ;

	__LEAVE_FUNCTION
}
//---------------------------------------------------------------------------
VOID	ItemTable::InitGemInfoTable()
{
	__ENTER_FUNCTION

	enum	GemInfoTBData
	{	
		TBTableIndex		=	0,
		TBClass				,
		TBType				,
		TBIndex				,
		TBResID				,
		TBRulerID			,
		TBEquipType			,
		TBName				,
		TBDesc				,
		TBPrice				,
		TBSellPrice			,
		TBBroadCast			,
		TBEffectBegin		,
		TBGemReqBegin		=   TBEffectBegin+MAX_GEM_ATT*2,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_GEM_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nGemCount			=	iTableCount;
	m_pGemInfoData		=	new GEMINFO_TB[iTableCount];

	INT iColumn			=	0;
	for(uint i = 0;i<m_nGemCount;i++)
	{
		m_pGemInfoData[i].m_nTableIndex			= ThirdFile.Search_Posistion(i,TBTableIndex)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Class	= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Type		= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Index	= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pGemInfoData[i].m_nRulerID			= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pGemInfoData[i].m_nPrice				= ThirdFile.Search_Posistion(i,TBPrice)->iValue;
		m_pGemInfoData[i].m_nSellPrice			= ThirdFile.Search_Posistion(i,TBSellPrice)->iValue;
		m_pGemInfoData[i].m_bBroadCast			= ThirdFile.Search_Posistion(i,TBBroadCast)->iValue;

		//需要解析字符串
		const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,TBEquipType)->pString;

		SplitStringLevelOne pSplitL1;
		SplitStringLevelTwo pSplitL2;
		pSplitL1.Reset();
		pSplitL2.Reset();
		pSplitL1.Init( '|', &pSplitL2 );
		pSplitL2.Init( '~', NULL );
		pSplitL1.DoSplit( pSplitString );

		INT iLineCount	= pSplitL2.GetResultLineCount();
		Assert(iLineCount<MAX_BASE_EQUIP_COUNT);
		for( INT iLine=0; iLine<iLineCount; ++iLine )
		{
			INT nPos = atoi(pSplitL2.GetResultLine(iLine));

			if (EquipCanAddHole((EQUIP_TYPE)nPos))
			{
				m_pGemInfoData[i].m_EquipType[nPos] = 1;
			}
			else
			{
				Assert(0);
			}
		}
		INT iColumn		=	TBEffectBegin;
		for(uint j = 0; j<MAX_GEM_ATT;j++)
		{
			Assert(iColumn<iTableColumn);

			INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;

			if(iValue>=0)
			{
				m_pGemInfoData[i].m_GenAttr[j].m_AttrType = (ITEM_ATTRIBUTE)ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				m_pGemInfoData[i].m_GenAttr[j].m_Value.m_Value = ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				++m_pGemInfoData[i].m_AttCount;
			}
			else
				break;
		}
		iColumn		=	TBGemReqBegin;
		for(uint j = 0; j<MAX_GEM_REQ;j++)
		{
			Assert(iColumn<iTableColumn);

			INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;

			if(iValue>=0)
			{
				m_pGemInfoData[i].m_GemReqAttr[j].m_GemType = (ITEM_ATTRIBUTE)ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				m_pGemInfoData[i].m_GemReqAttr[j].m_Value.m_Value = ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				++m_pGemInfoData[i].m_GemReqCount;
			}
			else
				break;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "InitGemInfoTable()  Ok!") ;
	__LEAVE_FUNCTION
}

VOID	ItemTable::InitEquipHoleInfoTable()
{
	__ENTER_FUNCTION

	enum	EquipHoleInfoTBData
	{	
		TBHoleNum			=	0,
		TBReqItem			,
		TBReqItemNum		,
		TBReqMoney			,
		TBRate				,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EUIPHOLE_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nEquipHoleInfoCount			=	iTableCount;
	Assert(m_nEquipHoleInfoCount==MAX_ITEM_GEM);

	m_EquipHoleInfoData					=	new EQUIPHOLE_INFO_TB[iTableCount];
	INT iColumn			=	0;
	for(uint i = 0;i<m_nEquipHoleInfoCount;i++)
	{
		Assert(i == (ThirdFile.Search_Posistion(i,TBHoleNum)->iValue-1));
		m_EquipHoleInfoData[i].m_ReqItem			= ThirdFile.Search_Posistion(i,TBReqItem)->iValue;
		m_EquipHoleInfoData[i].m_ReqItemNum			= ThirdFile.Search_Posistion(i,TBReqItemNum)->iValue;
		m_EquipHoleInfoData[i].m_ReqMoney			= ThirdFile.Search_Posistion(i,TBReqMoney)->iValue;
		m_EquipHoleInfoData[i].m_Rate				= ThirdFile.Search_Posistion(i,TBRate)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipHoleInfoTable()  Ok!") ;
	__LEAVE_FUNCTION
}

VOID	ItemTable::InitEquipLevelupInfoTable()
{
	__ENTER_FUNCTION

	enum	EquipLevelupInfoTBData
	{	
		TBLevel				=	0,
		TBLowLevelReqItem	,
		TBHighLevelReqItem	,
		TBOtherLevelReqItem ,
		TBItemNum			,
		TBPunishment		= TBItemNum + MAX_LEVELUP_GEMREQ_NUM,
		TBReqMoney			,
		TBEffectRate		,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EUIPLEVEUP_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nLeveupInfoCount			=	iTableCount;
	Assert(m_nLeveupInfoCount==MAX_EQUIP_LEVELUP_NUM);

	m_EquipLeveupInfoData					=	new EQUIP_LEVELUP_TB[iTableCount];
	INT iColumn			=	0;
	for(uint i = 0;i<m_nLeveupInfoCount;i++)
	{
		Assert(i == (ThirdFile.Search_Posistion(i,TBLevel)->iValue-1));
		m_EquipLeveupInfoData[i].m_LowLevelGemID	= ThirdFile.Search_Posistion(i,TBLowLevelReqItem)->iValue;
		m_EquipLeveupInfoData[i].m_HighLevelGemID	= ThirdFile.Search_Posistion(i,TBHighLevelReqItem)->iValue;
		m_EquipLeveupInfoData[i].m_OtherLevelGemID  = ThirdFile.Search_Posistion(i,TBOtherLevelReqItem)->iValue;
		for(uint j=0; j<MAX_LEVELUP_GEMREQ_NUM; ++j)
		{
			m_EquipLeveupInfoData[i].m_SuccessRate[j]	= ThirdFile.Search_Posistion(i,j+TBItemNum)->iValue;
		}
		m_EquipLeveupInfoData[i].m_ReqMoney			= ThirdFile.Search_Posistion(i,TBReqMoney)->iValue;
		m_EquipLeveupInfoData[i].m_Punishment		= ThirdFile.Search_Posistion(i,TBPunishment)->iValue;
		m_EquipLeveupInfoData[i].m_EffectRate		= ThirdFile.Search_Posistion(i,TBEffectRate)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipLevelupInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitEquipBindInfoTable()
{
	__ENTER_FUNCTION

	enum	EquipLevelupInfoTBData
	{	
		TBBindLevel			=	0,
		TBReqItem			,
		TBEffectRate		,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EUIPBIND_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nEquipBindInfoCount			=	iTableCount;
	Assert(m_nEquipBindInfoCount==MAX_EQUIP_BIND_LEVEL);

	m_EquipBindInfoData					=	new EQUIP_BIND_TB[iTableCount];
	INT iColumn			=	0;
	for(uint i = 0;i<m_nEquipBindInfoCount;i++)
	{
		Assert(i == (ThirdFile.Search_Posistion(i,TBBindLevel)->iValue-1));
		m_EquipBindInfoData[i].m_GemID			= ThirdFile.Search_Posistion(i,TBReqItem)->iValue;
		m_EquipBindInfoData[i].m_EffectRate		= ThirdFile.Search_Posistion(i,TBEffectRate)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipBindInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitSpecialRepairInfoTable()
{
	__ENTER_FUNCTION

		enum	SpecialRepairInfoTBData
	{	
		TBReqItem			=0,
		TBRepairPoint		,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SPECIALREPAIR_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nSpecialRepairCount			=	iTableCount;
	Assert(m_nSpecialRepairCount==1);

	m_EquipSpecialRepairData				=	new EQUIP_SPECIALREPAIR_TB[iTableCount];
	for(uint i = 0;i<m_nSpecialRepairCount;i++)
	{
		m_EquipSpecialRepairData[i].m_ItemID			= ThirdFile.Search_Posistion(i,TBReqItem)->iValue;
		m_EquipSpecialRepairData[i].m_RecoverPoint		= ThirdFile.Search_Posistion(i,TBRepairPoint)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitSpecialRepairInfoTable()  Ok!") ;
	__LEAVE_FUNCTION
}

VOID	ItemTable::InitProductInfoTable()
{
	__ENTER_FUNCTION

	enum    ProductCreateInfo
	{
		TBMaterialType		=0,
		TBMaterialNum		,
		TBCreateNeedLevel	,
		TBSplitRate			,
		TBCreatInfoNum		,
	};

	enum    ProductRemouldInfo
	{
		TBRemouldMaterialType	 =0,
		TBRemouldMaterialNum	,
		TBRemouldNeedLevel		,
		TBRemouldInfoNum		,
	};
	enum	ProductInfoTBData
	{	
		TBPrescriptionID	=	0,//产品ID	
		TBProductID			,//配方号	
		TBPrescriptionName	,//配方名称
		TBProductNum		,//产品数量
		TBCreateInfo		,//材料类1
		TBRemouldInfo		= TBCreateInfo + TBCreatInfoNum*MAX_CREATE_MATERIAL_NUM,
		TBOperateTime		= TBRemouldInfo + TBRemouldInfoNum*MAX_REMOULD_MATERIAL_NUM,
		TBScriptID		,
		TBCreateMoney		,
		TBRemouldMoney		,
		TBNeedNPC			,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PRODUCT_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nProductCount			=	iTableCount;
	m_pProductInfoData				=	new ITEM_PRODUCT_INFO_TB[iTableCount];

	for(uint i = 0;i<m_nProductCount;i++)
	{
		m_pProductInfoData[i].m_ProductID			= ThirdFile.Search_Posistion(i,TBProductID)->iValue;		//产品ID	
		m_pProductInfoData[i].m_PrescriptionID		= ThirdFile.Search_Posistion(i,TBPrescriptionID)->iValue;	//配方号	
		m_pProductInfoData[i].m_ProductNum			= ThirdFile.Search_Posistion(i,TBProductNum)->iValue;		//产品数量
		

		for (int j=0; j<MAX_CREATE_MATERIAL_NUM; ++j)
		{
			if (ThirdFile.Search_Posistion(i,TBCreateInfo+TBMaterialType+j*TBCreatInfoNum)->iValue != -1)
			{
				m_pProductInfoData[i].m_CreateInfo[j].m_MaterialType		= ThirdFile.Search_Posistion(i,TBCreateInfo+TBMaterialType+j*TBCreatInfoNum)->iValue;	//材料类型
				m_pProductInfoData[i].m_CreateInfo[j].m_MaterialNum			= ThirdFile.Search_Posistion(i,TBCreateInfo+TBMaterialNum+j*TBCreatInfoNum)->iValue;;	//材料数量	
				m_pProductInfoData[i].m_CreateInfo[j].m_NeedLevel			= ThirdFile.Search_Posistion(i,TBCreateInfo+TBCreateNeedLevel+j*TBCreatInfoNum)->iValue;;	//材料最低等级	
				m_pProductInfoData[i].m_CreateInfo[j].m_SplitRate			= ThirdFile.Search_Posistion(i,TBCreateInfo+TBSplitRate+j*TBCreatInfoNum)->iValue;;	//拆解损耗比例	

				++m_pProductInfoData[i].m_nCreateInfoCount;
			}
			else
				break;
		}
	  
		for (int j=0; j<MAX_REMOULD_MATERIAL_NUM; ++j)
		{
			if (ThirdFile.Search_Posistion(i,TBRemouldInfo+TBRemouldMaterialType+j*TBRemouldInfoNum)->iValue != -1)
			{
				m_pProductInfoData[i].m_RemouldInfo[j].m_MaterialType		= ThirdFile.Search_Posistion(i,TBRemouldInfo+TBRemouldMaterialType+j*TBRemouldInfoNum)->iValue;	//材料类型
				m_pProductInfoData[i].m_RemouldInfo[j].m_MaterialNum		= ThirdFile.Search_Posistion(i,TBRemouldInfo+TBRemouldMaterialNum+j*TBRemouldInfoNum)->iValue;	//材料数量
				m_pProductInfoData[i].m_RemouldInfo[j].m_NeedLevel			= ThirdFile.Search_Posistion(i,TBRemouldInfo+TBRemouldNeedLevel+j*TBRemouldInfoNum)->iValue;	//材料最低等级	

				++m_pProductInfoData[i].m_nRemouldInfoCount;
			}
			
		}
		m_pProductInfoData[i].m_OperateTime			= ThirdFile.Search_Posistion(i,TBOperateTime)->iValue;
		m_pProductInfoData[i].m_ScriptID			= ThirdFile.Search_Posistion(i,TBScriptID)->iValue;
		m_pProductInfoData[i].m_CreateMoney			= ThirdFile.Search_Posistion(i,TBCreateMoney)->iValue;
		m_pProductInfoData[i].m_RemouldMoney		= ThirdFile.Search_Posistion(i,TBRemouldMoney)->iValue;
		// add by gh 是否需要NPC的判定参数
		m_pProductInfoData[i].m_NeedNPC				= ThirdFile.Search_Posistion(i,TBNeedNPC)->iValue;
	}
	Log::SaveLog( SERVER_LOGFILE, "InitProductInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitMaterialLevelInfoTable()
{
	__ENTER_FUNCTION

		enum	MaterialLevelInfoTBData
	{	
		TBBindLevel			=	0,
		TBVaule				,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MATERIALLEVEL_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nMaterialLevelInfoCount			=	iTableCount;
	Assert(m_nMaterialLevelInfoCount==MAX_VALIDMATERIAL_LEVEL);

	m_MaterialLevelInfoData			=	new MATERIAL_LEVEL_INFO_TB[iTableCount];
	INT iColumn			=	0;
	for(uint i = 0;i<m_nMaterialLevelInfoCount;i++)
	{
		Assert(i == (ThirdFile.Search_Posistion(i,TBBindLevel)->iValue-1));
		m_MaterialLevelInfoData[i].m_MaterialValue		= ThirdFile.Search_Posistion(i,TBVaule)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitMaterialLevelInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitEquipExtraInfoTable()
{
	__ENTER_FUNCTION

		enum	GemInfoTBData
	{	
		TBEquipScore		=	0,
		TBExtraAttSetBegin	,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EQUIPEXTRAATT_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nEquipExtraInfoCount		=	iTableCount;
	m_EquipExtraInfoData		=	new EQUIPEXTRATT_TB[iTableCount];

	INT iColumn			=	0;
	for(INT i = 0;i<iTableCount;i++)
	{
		m_EquipExtraInfoData[i].m_nEquipScore			= ThirdFile.Search_Posistion(i,TBEquipScore)->iValue;
		
		for (UINT m=0; m<MAX_JOB_SUM; ++m)
		{
			SplitStringLevelOne pSplitL1;
			for(UINT j=0; j<MAX_ITEM_NATUREATTR; ++j)
			{
				//需要解析字符串
				const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,TBExtraAttSetBegin+m*MAX_ITEM_NATUREATTR+j)->pString;
				SplitStringLevelOne pSplitL1;
				SplitStringLevelTwo pSplitL2;
				pSplitL1.Reset();
				pSplitL2.Reset();
				pSplitL1.Init( '|', &pSplitL2 );
				pSplitL2.Init( '~', NULL );
				pSplitL1.DoSplit( pSplitString );

				INT iLineCount	= pSplitL2.GetResultLineCount();
				Assert(iLineCount<20);
				BOOL bValid = FALSE;
				for( INT iLine=0; iLine<iLineCount; ++iLine )
				{
					INT nAtt = atoi(pSplitL2.GetResultLine(iLine));
					if(nAtt <= -1)
					{
						break;
					}
					m_EquipExtraInfoData[i].m_EquipExtrAtt[m].m_GenAttr[m_EquipExtraInfoData[i].m_EquipExtrAtt[m].m_ValidCount].m_Attr[iLine] = nAtt;
					++m_EquipExtraInfoData[i].m_EquipExtrAtt[m].m_GenAttr[m_EquipExtraInfoData[i].m_EquipExtrAtt[m].m_ValidCount].m_AttrCount; 
					bValid = TRUE;
				}
				if (bValid)
				{
					++m_EquipExtraInfoData[i].m_EquipExtrAtt[m].m_ValidCount;
				}

			}
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipExtraInfoTable()  Ok!") ;
	__LEAVE_FUNCTION
}

VOID	ItemTable::InitEquipPointInfoTable()
{
	__ENTER_FUNCTION

		enum	EquipPointInfoTBData
	{	
		TBEquipPoint		=	0,
		TBVaule				=   2,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EQUIPPOINT_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	Assert(iTableCount<=HEQUIP_NUMBER); // modify by gh 2010/05/07

	INT iColumn			=	0;
	for(INT i = 0;i<iTableCount;i++)
	{
		uint EquipPoint = ThirdFile.Search_Posistion(i,TBEquipPoint)->iValue;
		Assert(EquipPoint<HEQUIP_NUMBER);
		m_nEquipPointInfo[EquipPoint]	= ThirdFile.Search_Posistion(i,TBVaule)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipPointInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitExtraBagInfoTable()
{
	__ENTER_FUNCTION

	enum	ExtraBagTBData
	{	
		TBItemIndex			=	0,
		TBClass				,	//基本类型
		TBType				,	//类别
		TBIndex				,	//物品号
		TBResID				,
		TBName				,
		TBDesc				,
		TBRulerID			,
		TBBasePrice			,
		TBSellPrice			,
		TBTime				,
		TBVaule				,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EXTRABAG_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nExtraBagInfoCount		=	iTableCount;
	m_pExtraBagInfoData			=	new EXTRABAG_INFO_TB[iTableCount];

	INT iColumn			=	0;
	for(INT i = 0;i<iTableCount;i++)
	{
		m_pExtraBagInfoData[i].m_nTableIndex		= ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		m_pExtraBagInfoData[i].m_ItemType.m_Class	= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pExtraBagInfoData[i].m_ItemType.m_Type	= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pExtraBagInfoData[i].m_ItemType.m_Index	= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pExtraBagInfoData[i].m_nRulerID			= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;	 //规则ID
		m_pExtraBagInfoData[i].m_nBasePrice			= ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pExtraBagInfoData[i].m_nSellPrice			= ThirdFile.Search_Posistion(i,TBSellPrice)->iValue;
		m_pExtraBagInfoData[i].m_nValidTime			= ThirdFile.Search_Posistion(i,TBTime)->iValue;
		Assert(ThirdFile.Search_Posistion(i,TBVaule)->iValue <= MAX_SINGLEBAG_SIZE);
		m_pExtraBagInfoData[i].m_nSize				 = ThirdFile.Search_Posistion(i,TBVaule)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitExtraBagInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitEquipChainInfoTable()
{
	__ENTER_FUNCTION

	enum	EquipChainTBData
	{	
		TBID			=	0,
		TBName				,	//基本类型
		TBReqPai1			,	//
		TBReqPai2			,	//
		TBReqPai3			,
		TBAttID				,
		TBAttValue			,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CHAIN_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nEquipChainInfoCount		=	iTableCount;
	m_EquipChainInfoData		=	new EQUIPCHAIN_INFO_TB[iTableCount];

	INT iColumn			=	0;
	INT nValue			=   -1;
	BOOL  bTrue			=  TRUE;
	for(INT i = 0;i<iTableCount;i++)
	{
		nValue	= ThirdFile.Search_Posistion(i,TBID)->iValue;
		if (nValue != -1)
		{
			m_EquipChainInfoData[i].m_ChainID	= ThirdFile.Search_Posistion(i,TBID)->iValue;
		}

		nValue	= ThirdFile.Search_Posistion(i,TBReqPai1)->iValue;
		if (nValue != -1)
		{
			m_EquipChainInfoData[i].m_ReqPai1				= ThirdFile.Search_Posistion(i,TBReqPai1)->iValue;
			m_EquipChainInfoData[i].m_Flat.UpdateBits(m_EquipChainInfoData[i].m_ReqPai1, bTrue);
		}
		
		nValue	= ThirdFile.Search_Posistion(i,TBReqPai2)->iValue;
		if (nValue != -1)
		{
			m_EquipChainInfoData[i].m_ReqPai2				= ThirdFile.Search_Posistion(i,TBReqPai2)->iValue;
			m_EquipChainInfoData[i].m_Flat.UpdateBits(m_EquipChainInfoData[i].m_ReqPai2, bTrue);
		}

		nValue	= ThirdFile.Search_Posistion(i,TBReqPai3)->iValue;
		if (nValue != -1)
		{
			m_EquipChainInfoData[i].m_ReqPai3				= ThirdFile.Search_Posistion(i,TBReqPai3)->iValue;
			m_EquipChainInfoData[i].m_Flat.UpdateBits(m_EquipChainInfoData[i].m_ReqPai3, bTrue);
		}
		m_EquipChainInfoData[i].m_Att.m_AttrType		= ThirdFile.Search_Posistion(i,TBAttID)->iValue;	 //规则ID
		m_EquipChainInfoData[i].m_Att.m_Value.m_Value	= ThirdFile.Search_Posistion(i,TBAttValue)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipChainInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitEquipSoulInfoTable()
{
	__ENTER_FUNCTION

	enum	EquipSoulTBData
	{	
		TBID			=	0,
		TBName				,	//基本类型
		TBReqChain1			,	//
		TBReqChain2			,	//
		TBReqChain3			,
		TBReqChain4			,
		TBReqChain5			,
		TBAttID				,
		TBAttValue			,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SOUL_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nEquipSoulInfoCount		=	iTableCount;
	m_EquipSoulInfoData			=	new EQUIPSOUL_INFO_TB[iTableCount];

	INT iColumn			=	0;
	INT nValue			=   -1;
	for(INT i = 0;i<iTableCount;i++)
	{
		nValue = ThirdFile.Search_Posistion(i,TBID)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_SoulID					= ThirdFile.Search_Posistion(i,TBID)->iValue;
		}
		nValue = ThirdFile.Search_Posistion(i,TBReqChain1)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_ChainID1				= ThirdFile.Search_Posistion(i,TBReqChain1)->iValue;

			if ((m_EquipSoulInfoData[i].m_ChainID1 != EQUIPPAI_ANYSHUN)
				&&(m_EquipSoulInfoData[i].m_ChainID1 != EQUIPPAI_ANYKE))
			{
				if (m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID1] == 0)
				{
					m_EquipSoulInfoData[i].ValidSoul[m_EquipSoulInfoData[i].ValidSoulNum] = m_EquipSoulInfoData[i].m_ChainID1;
					++m_EquipSoulInfoData[i].ValidSoulNum;
				}
				++m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID1];
			}
			if (m_EquipSoulInfoData[i].m_ChainID1 == EQUIPPAI_ANYSHUN)
			{
				++m_EquipSoulInfoData[i].m_AnyShunNum;
			}
			else if (m_EquipSoulInfoData[i].m_ChainID1 == EQUIPPAI_ANYKE)
			{
				++m_EquipSoulInfoData[i].m_AnyKeNum;
			}
		}
		
		nValue = ThirdFile.Search_Posistion(i,TBReqChain2)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_ChainID2				= ThirdFile.Search_Posistion(i,TBReqChain2)->iValue;

			if ((m_EquipSoulInfoData[i].m_ChainID2 != EQUIPPAI_ANYSHUN)
				&&(m_EquipSoulInfoData[i].m_ChainID2 != EQUIPPAI_ANYKE))
			{
				if (m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID2] == 0)
				{
					m_EquipSoulInfoData[i].ValidSoul[m_EquipSoulInfoData[i].ValidSoulNum] = m_EquipSoulInfoData[i].m_ChainID2;
					++m_EquipSoulInfoData[i].ValidSoulNum;
				}
				++m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID2];
			}
			if (m_EquipSoulInfoData[i].m_ChainID2 == EQUIPPAI_ANYSHUN)
			{
				++m_EquipSoulInfoData[i].m_AnyShunNum;
			}
			else if (m_EquipSoulInfoData[i].m_ChainID2 == EQUIPPAI_ANYKE)
			{
				++m_EquipSoulInfoData[i].m_AnyKeNum;
			}
		}

		nValue = ThirdFile.Search_Posistion(i,TBReqChain3)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_ChainID3				= ThirdFile.Search_Posistion(i,TBReqChain3)->iValue;

			if ((m_EquipSoulInfoData[i].m_ChainID3 != EQUIPPAI_ANYSHUN)
				&&(m_EquipSoulInfoData[i].m_ChainID3 != EQUIPPAI_ANYKE))
			{
				if (m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID3] == 0)
				{
					m_EquipSoulInfoData[i].ValidSoul[m_EquipSoulInfoData[i].ValidSoulNum] = m_EquipSoulInfoData[i].m_ChainID3;
					++m_EquipSoulInfoData[i].ValidSoulNum;
				}
				++m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID3];
			}
			if (m_EquipSoulInfoData[i].m_ChainID3 == EQUIPPAI_ANYSHUN)
			{
				++m_EquipSoulInfoData[i].m_AnyShunNum;
			}
			else if (m_EquipSoulInfoData[i].m_ChainID3 == EQUIPPAI_ANYKE)
			{
				++m_EquipSoulInfoData[i].m_AnyKeNum;
			}
		}

		nValue = ThirdFile.Search_Posistion(i,TBReqChain4)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_ChainID4				= ThirdFile.Search_Posistion(i,TBReqChain4)->iValue;

			if ((m_EquipSoulInfoData[i].m_ChainID4 != EQUIPPAI_ANYSHUN)
				&&(m_EquipSoulInfoData[i].m_ChainID4 != EQUIPPAI_ANYKE))
			{
				if (m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID4] == 0)
				{
					m_EquipSoulInfoData[i].ValidSoul[m_EquipSoulInfoData[i].ValidSoulNum] = m_EquipSoulInfoData[i].m_ChainID4;
					++m_EquipSoulInfoData[i].ValidSoulNum;
				}
				++m_EquipSoulInfoData[i].SoulType[m_EquipSoulInfoData[i].m_ChainID4];
			}
			if (m_EquipSoulInfoData[i].m_ChainID4 == EQUIPPAI_ANYSHUN)
			{
				++m_EquipSoulInfoData[i].m_AnyShunNum;
			}
			else if (m_EquipSoulInfoData[i].m_ChainID4 == EQUIPPAI_ANYKE)
			{
				++m_EquipSoulInfoData[i].m_AnyKeNum;
			}
		}
		nValue = ThirdFile.Search_Posistion(i,TBReqChain5)->iValue;
		if (nValue != -1)
		{
			m_EquipSoulInfoData[i].m_ChainID5				= ThirdFile.Search_Posistion(i,TBReqChain5)->iValue;
		}
		
		iColumn	= TBAttID;
		for (uint j=0; j<MAX_SOUL_EFFECTATT; ++j)
		{
			if (ThirdFile.Search_Posistion(i,iColumn)->iValue == -1)
			{
				break;
			}
			m_EquipSoulInfoData[i].m_Att[j].m_AttrType			= ThirdFile.Search_Posistion(i,iColumn)->iValue;	 //规则ID
			++iColumn;
			m_EquipSoulInfoData[i].m_Att[j].m_Value.m_Value	= ThirdFile.Search_Posistion(i,iColumn)->iValue;
			++iColumn;
			++m_EquipSoulInfoData[i].m_EffectAttCount;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "InitEquipSoulInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitChangePaiInfoTable()
{
	__ENTER_FUNCTION

		enum	ChangePaiTBData
	{	
		TBItemIndex			=	0,
		TBDesc				,	//说明
		TBPaiType			,	//
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CHANGEPAI_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nChangePaiInfoCount			=	iTableCount;
	m_pChangePaiInfoData			=	new CHANGEPAI_INFO_TB[iTableCount];

	INT iColumn			=	0;
	for(INT i = 0;i<iTableCount;i++)
	{
		m_pChangePaiInfoData[i].m_nTableIndex		= ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		if (ThirdFile.Search_Posistion(i,TBPaiType)->iValue>0)
		{
			m_pChangePaiInfoData[i].m_DestPaiType		= ThirdFile.Search_Posistion(i,TBPaiType)->iValue;	
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "InitChangePaiInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitPaiInfoTable()
{
	__ENTER_FUNCTION

		enum	PaiInfoTBData
	{	
		TBID			=	0,
		TBDesc				,	//说明
		TBAttID				,
		TBAttValue			,
		TBRate				,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PAI_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	Assert(iTableCount<EQUIP_PAI_NUM);

	INT iColumn			=	0;
	for(INT i = 0;i<iTableCount;i++)
	{
		INT nType = ThirdFile.Search_Posistion(i,TBID)->iValue;
		Assert(nType != -1);
		Assert(nType<EQUIP_PAI_NUM);
		m_nPaiInfo[nType].m_nPaiID				= ThirdFile.Search_Posistion(i,TBID)->iValue;

		m_nPaiInfo[nType].m_Att.m_AttrType		= ThirdFile.Search_Posistion(i,TBAttID)->iValue;
		m_nPaiInfo[nType].m_Att.m_Value.m_Value	= ThirdFile.Search_Posistion(i,TBAttValue)->iValue;
		m_nPaiInfo[nType].m_nRate				= ThirdFile.Search_Posistion(i,TBRate)->iValue;
		
		if ((m_nPaiInfo[nType].m_nPaiID	>= EQUIPPAI_DONG)&&(m_nPaiInfo[nType].m_nPaiID	<= EQUIPPAI_BAI))
		{
			m_nZIPai[m_nZiPaiCount] = m_nPaiInfo[nType].m_nPaiID;
			++m_nZiPaiCount;
		}
		if ((m_nPaiInfo[nType].m_nPaiID	>= EQUIPPAI_1TIAO)&&(m_nPaiInfo[nType].m_nPaiID	<= EQUIPPAI_9WAN))
		{
			m_nNoZIPai[m_nNoZIPaiCount] = m_nPaiInfo[nType].m_nPaiID;
			++m_nNoZIPaiCount;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "InitPaiInfoTable()  Ok!") ;
	__LEAVE_FUNCTION	
}

VOID	ItemTable::InitCommItemInfoTable()
{
	__ENTER_FUNCTION
	
	enum	CommonItemInfoTBData
	{
		TBIndex					=		0,
		TBClass					=		1,
		TBType					,
		TBMedicIndex			,
		TBResourceID			,
		TBName					,
		TBDesc					,
		TBLevel					,
		TBBasePrice				,
		TBSellPrice				,
		TBRulerID				,
		TBTileNumber			,
		TBScriptID				,
		TBSkillID				,
		TBReqSkill				,
		TBReqSkillLevel			,
		TBHoldNumber			,
		TBTargetType			,
		TBbBroadcast			,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_COMMON_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nCommItemCount			=	iTableCount;
	m_pCommItemInfoData		=	new COMMITEM_INFO_TB[iTableCount];
	
	INT iColumn		=	0;
	
	Assert(TBTargetType<iTableColumn);
	for(uint i = 0;i<m_nCommItemCount;i++)
	{
		m_pCommItemInfoData[i].m_nTableIndex					= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Class				= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Type				= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Index				= ThirdFile.Search_Posistion(i,TBMedicIndex)->iValue;

		m_pCommItemInfoData[i].m_nLevel							= ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pCommItemInfoData[i].m_nBasePrice						= ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pCommItemInfoData[i].m_nSellPrice						= ThirdFile.Search_Posistion(i,TBSellPrice)->iValue;
		m_pCommItemInfoData[i].m_nRulerID						= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;	
		m_pCommItemInfoData[i].m_nLayedNum						= ThirdFile.Search_Posistion(i,TBTileNumber)->iValue;
		m_pCommItemInfoData[i].m_nScriptID						= ThirdFile.Search_Posistion(i,TBScriptID)->iValue;	 		
		m_pCommItemInfoData[i].m_nSkillID						= ThirdFile.Search_Posistion(i,TBSkillID)->iValue;	 		

		m_pCommItemInfoData[i].m_nReqSkill						= ThirdFile.Search_Posistion(i,TBReqSkill)->iValue;	 		
		m_pCommItemInfoData[i].m_nReqSkillLevel					= ThirdFile.Search_Posistion(i,TBReqSkillLevel)->iValue;	 		
		m_pCommItemInfoData[i].m_HoldNum						= ThirdFile.Search_Posistion(i,TBHoldNumber)->iValue;	 		
		m_pCommItemInfoData[i].m_TargetType						= ThirdFile.Search_Posistion(i,TBTargetType)->iValue;	 		

		if(ThirdFile.Search_Posistion(i,TBbBroadcast)->iValue ==1)
			m_pCommItemInfoData[i].m_bBroadcast							= TRUE;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitCommonItemInfoTable()  Ok!") ;
	__LEAVE_FUNCTION

}

VOID	ItemTable::InitTaskItemInfoTable()
{
	__ENTER_FUNCTION

	enum	TaskItemInfoTBData
	{
		TBIndex					=		0,
		TBClass					=		1,
		TBType					,
		TBResourceID			,
		TBMedicIndex			,
		TBName					,
		TBDesc					,
		TBLevel					,
		TBBasePrice				,
		TBSellPrice				,
		TBRulerID				,
		TBTileNumber			,
		TBScriptID				,
		TBSkillID				,
		TBReqSkill				,
		TBReqSkillLevel			,
		TBHoldNumber			,
		TBTargetType			,
		TBbBroadcast			,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_TASK_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nTaskItemCount			=	iTableCount;
	m_pTaskItemInfoData		=	new TASKITEM_INFO_TB[iTableCount];

	INT iColumn		=	0;

	Assert(TBTargetType<iTableColumn);
	for(uint i = 0;i<m_nTaskItemCount;i++)
	{
		m_pTaskItemInfoData[i].m_nTableIndex					= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pTaskItemInfoData[i].m_ItemType.m_Class				= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pTaskItemInfoData[i].m_ItemType.m_Type				= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pTaskItemInfoData[i].m_ItemType.m_Index				= ThirdFile.Search_Posistion(i,TBMedicIndex)->iValue;

		m_pTaskItemInfoData[i].m_nLevel							= ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pTaskItemInfoData[i].m_nBasePrice						= ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pTaskItemInfoData[i].m_nSellPrice						= ThirdFile.Search_Posistion(i,TBSellPrice)->iValue;
		m_pTaskItemInfoData[i].m_nRulerID						= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;	
		m_pTaskItemInfoData[i].m_nLayedNum						= ThirdFile.Search_Posistion(i,TBTileNumber)->iValue;
		m_pTaskItemInfoData[i].m_nScriptID						= ThirdFile.Search_Posistion(i,TBScriptID)->iValue;	 		
		m_pTaskItemInfoData[i].m_nSkillID						= ThirdFile.Search_Posistion(i,TBSkillID)->iValue;	 		

		m_pTaskItemInfoData[i].m_nReqSkill						= ThirdFile.Search_Posistion(i,TBReqSkill)->iValue;	 		
		m_pTaskItemInfoData[i].m_nReqSkillLevel					= ThirdFile.Search_Posistion(i,TBReqSkillLevel)->iValue;	 		
		m_pTaskItemInfoData[i].m_HoldNum						= ThirdFile.Search_Posistion(i,TBHoldNumber)->iValue;	 		
		m_pTaskItemInfoData[i].m_TargetType						= ThirdFile.Search_Posistion(i,TBTargetType)->iValue;	 		

		if(ThirdFile.Search_Posistion(i,TBbBroadcast)->iValue ==1)
			m_pTaskItemInfoData[i].m_bBroadcast							= TRUE;
	}

	Log::SaveLog( SERVER_LOGFILE, "InitTaskItemInfoTable()  Ok!") ;
	__LEAVE_FUNCTION

}

VOID	ItemTable::InitItemRulerTable()
{
	__ENTER_FUNCTION

	enum	ItemRulerTBData
	{
		TBRulerIndex	=	0,
		TBTile			=	2,
		TBDiscard		,
		TBShortCut		,
		TBSell			,
		TBExchange		,
		TBUse			,
		TBPickBind		,
		TBEquipBind		,
		TBUnique		,
		TBIdent			,	
		TBDummyItem		,
		TBBank			,
		TBConsume		,
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ITEM_RULE );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nItemRulerCount		=	iTableCount;
	m_pItemRulerData		=	new ITEM_RULER_TB[iTableCount];	

	INT iColumn		=	0;


	Assert(TBUnique<iTableColumn);

	for(uint i = 0;i<m_nItemRulerCount;i++)
	{
		static BOOL bInit = FALSE;		
		
		m_pItemRulerData[i].m_RulerIndex	=	ThirdFile.Search_Posistion(i,TBRulerIndex)->iValue;
		
		if(!bInit)
		{
			m_nHashOffSet	=	m_pItemRulerData[i].m_RulerIndex - i;
		}
		Assert(m_pItemRulerData[i].m_RulerIndex == i+m_nHashOffSet);//连续性检查

		m_pItemRulerData[i].m_Discard		=	ThirdFile.Search_Posistion(i,TBDiscard)->iValue	== 1 ;
		m_pItemRulerData[i].m_Tile			=	ThirdFile.Search_Posistion(i,TBTile)->iValue	== 1;
		m_pItemRulerData[i].m_ShortCut		=	ThirdFile.Search_Posistion(i,TBShortCut)->iValue	== 1;
		m_pItemRulerData[i].m_CanSell		=	ThirdFile.Search_Posistion(i,TBSell)->iValue	==1;
		m_pItemRulerData[i].m_CanExchange	=	ThirdFile.Search_Posistion(i,TBExchange)->iValue	== 1;
		m_pItemRulerData[i].m_CanUse		=	ThirdFile.Search_Posistion(i,TBUse)->iValue	== 1;
		m_pItemRulerData[i].m_PickBind		=	ThirdFile.Search_Posistion(i,TBPickBind)->iValue == 1;
		m_pItemRulerData[i].m_EquipBind		=	ThirdFile.Search_Posistion(i,TBEquipBind)->iValue == 1;
		m_pItemRulerData[i].m_Unique		=	ThirdFile.Search_Posistion(i,TBUnique)->iValue	== 1;

		m_pItemRulerData[i].m_Ident			=	ThirdFile.Search_Posistion(i,TBIdent)->iValue	== 1;
		m_pItemRulerData[i].m_DummyItem		=	ThirdFile.Search_Posistion(i,TBDummyItem)->iValue == 1;
		m_pItemRulerData[i].m_Bank			=	ThirdFile.Search_Posistion(i,TBBank)->iValue == 1;
		m_pItemRulerData[i].m_Consume		=	ThirdFile.Search_Posistion(i,TBConsume)->iValue	== 1;

	}
	
	Log::SaveLog( SERVER_LOGFILE, "InitItemRulerTable()  Ok!") ;

	__LEAVE_FUNCTION
}

VOID	ItemTable::InitStoreMapTable()
{
	__ENTER_FUNCTION	

	enum	StoreMapTBData
	{
		TBIndex					=		0,
		TBClass					=		1,
		//TBQuality				=		2,
		TBType					,
		TBResourceID			,
		TBMapIndex				,
		TBName					,
		TBDesc					,
		TBLevel					,
		TBRulerID				,
		TBXPos					,
		TBZPos					,
		TBSceneID				,
		TBGrowPointType			,
		TBUseScript				
	};

	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_STORE_MAP_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nStoreMapCount		=	iTableCount;
	m_pStoreMapData			=	new STORE_MAP_INFO_TB[iTableCount];

	INT iColumn		=	0;

	Assert(TBSceneID<iTableColumn);

	for(uint i = 0 ;i<m_nStoreMapCount;i++)
	{
		m_pStoreMapData[i].m_nTableIndex		=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Class	=	ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Type	=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Index	=	ThirdFile.Search_Posistion(i,TBMapIndex)->iValue;
		m_pStoreMapData[i].m_ResourceID			=	ThirdFile.Search_Posistion(i,TBResourceID)->iValue;
		m_pStoreMapData[i].m_nLevel				=	ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pStoreMapData[i].m_nRulerID			=	ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pStoreMapData[i].m_xPos				=	ThirdFile.Search_Posistion(i,TBXPos)->fValue;
		m_pStoreMapData[i].m_zPos				=	ThirdFile.Search_Posistion(i,TBZPos)->fValue;
		m_pStoreMapData[i].m_SceneID			=	ThirdFile.Search_Posistion(i,TBSceneID)->iValue;
		m_pStoreMapData[i].m_GrowPointType		=	ThirdFile.Search_Posistion(i,TBGrowPointType)->iValue;
		m_pStoreMapData[i].m_ScriptID			=	ThirdFile.Search_Posistion(i,TBUseScript)->iValue;

	}
	
	__LEAVE_FUNCTION
}
// add by gh 2010/05/10
VOID	ItemTable::InitSouXiaTable()
{
	__ENTER_FUNCTION	

	enum	SouXiaTBData
	{
		TBIndex					=		0,
		TBNeedLevel				=		2,
		TBType					,
		TBSubType0				,
		TBSubType0Param0		,
		TBSubType0Param4		= TBSubType0Param0 + MAX_SUB_TYPE0,
		TBSubType1				,
		TBSubType1Param0		,
		TBSubType1Param3	=	TBSubType1Param0 + MAX_SUB_TYPE1,
		TBSubType2				,
		TBSubType2Param0		,
		TBSubType2Param7	=	TBSubType2Param0 + MAX_SUB_TYPE2,
		TBSubType3				,
		TBSubType3Param0		,
		TBSubType3Param7	=	TBSubType3Param0 + MAX_SUB_TYPE3,
		TBSubType4				,
		TBUseTime				,				
	};


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SOUXIA_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nSouXiaCount		=	iTableCount;
	m_pSouXiaData			=	new SOUXIA_INFO_TB[iTableCount];

	INT iColumn		=	0;

	Assert(TBUseTime<=iTableColumn);

	const CHAR*  pSplitString1 = NULL;

	for(uint i = 0 ;i<m_nSouXiaCount;i++)
	{
		m_pSouXiaData[i].m_nTableIndex		=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pSouXiaData[i].m_nNeedLevel		=	ThirdFile.Search_Posistion(i,TBNeedLevel)->iValue;
		m_pSouXiaData[i].m_nUseType			=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pSouXiaData[i].m_nSouXiaLevel		=	ThirdFile.Search_Posistion(i,TBSubType0)->iValue;

		for (int m=0; m<=MAX_SUB_TYPE0; ++m)
		{
			m_pSouXiaData[i].m_nMaxPages[m] = ThirdFile.Search_Posistion(i,TBSubType0Param0+m)->iValue;
		}

		m_pSouXiaData[i].m_nMemoryType	=	ThirdFile.Search_Posistion(i,TBSubType1)->iValue;

		for (int m=0; m<=MAX_SUB_TYPE1; ++m)
		{
			//const char*  cTmp	=	ThirdFile.Search_Posistion(i,TBSubType1Param0+m)->pString;
			//m_pSouXiaData[i].m_nMemoryContent[m] = atoi(cTmp);
			//需要解析字符串  支持|或者~
			const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,TBSubType1Param0+m)->pString;
			SplitStringLevelOne pSplitL1;
			SplitStringLevelTwo pSplitL2;
			pSplitL1.Reset();
			pSplitL2.Reset();
			pSplitL1.Init( '|', &pSplitL2 );
			pSplitL2.Init( '~', NULL );
			pSplitL1.DoSplit( pSplitString );

			INT iLineCount	= pSplitL2.GetResultLineCount();
			Assert(iLineCount <= MAX_MEMORY_BASE_COUNT);

			INT	array[MAX_MEMORY_BASE_COUNT]	= {0};

			for( INT iLine=0; iLine<iLineCount; ++iLine )
			{
				array[iLine] = atoi(pSplitL2.GetResultLine(iLine));
			}

			//打乱数组次序，对表格数据随机
			INT randNum=0, tmpValue=0;
			srand( (unsigned int)time(NULL)); 
			for(INT j=0; j<iLineCount; ++j)   
			{   
				randNum = rand()%iLineCount;  

				tmpValue		= array[randNum];
				array[randNum]	= array[j];
				array[j]		= tmpValue;
			}    
			
			m_pSouXiaData[i].m_nMemoryContent[m] =	array[0];//最少有一个-1
		}

		m_pSouXiaData[i].m_nDismantleLevel	=	ThirdFile.Search_Posistion(i,TBSubType2)->iValue;

		for (int m=0; m<4; ++m)//与定义的MAX_SUB_TYPE2宏对应
		{
			for (int n=0; n<2; ++n)
			{
				m_pSouXiaData[i].m_nDismantleRule[m][n]	=	ThirdFile.Search_Posistion(i,TBSubType2Param0 + m * 2 + n)->iValue;
			}
		}
		
		m_pSouXiaData[i].m_nRefiningLevel	=	ThirdFile.Search_Posistion(i,TBSubType3)->iValue;

		for (int m=0; m<4; ++m)//与定义的MAX_SUB_TYPE3宏对应
		{
			for (int n=0; n<2; ++n)
			{
				m_pSouXiaData[i].m_nRefiningRule[m][n]	=	ThirdFile.Search_Posistion(i,TBSubType3Param0 + m * 2 + n)->iValue;
			}
		}
		m_pSouXiaData[i].m_nSewRate		=	ThirdFile.Search_Posistion(i,TBSubType4)->iValue;
		m_pSouXiaData[i].m_nUseTime		=	ThirdFile.Search_Posistion(i,TBUseTime)->iValue;
	}

	__LEAVE_FUNCTION
}

// add by gh 2010/05/25
VOID	ItemTable::InitSoulBeadTable()
{
	__ENTER_FUNCTION	

	enum	SoulBeadTBData
	{
		TBIndex					=		0,
		TBRuleId				=		7,
		TBValidTime				,	
		TBImpactID				,
	};


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SOUL_BEAD_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nSoulBeadCount	=	iTableCount;
	m_pSoulBeadData	=	new SOUL_BEAD_INFO_TB[iTableCount];

	INT iColumn		=	0;

	Assert(TBValidTime<=iTableColumn);

	const CHAR*  pSplitString1 = NULL;

	for(uint i = 0 ;i<m_nSoulBeadCount;i++)
	{
		m_pSoulBeadData[i].m_nTableIndex		=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pSoulBeadData[i].m_nRuleID			=	ThirdFile.Search_Posistion(i,TBRuleId)->iValue;
		m_pSoulBeadData[i].m_nValidTime		=	ThirdFile.Search_Posistion(i,TBValidTime)->iValue;
		m_pSoulBeadData[i].m_nImpactId			=	ThirdFile.Search_Posistion(i,TBImpactID)->iValue;		
	}

	__LEAVE_FUNCTION
}
// end of add

ITEM_RULER_TB*	ItemTable::GetRuleValueByID(INT& iIndex)
{

	__ENTER_FUNCTION

	INT iOffSet = iIndex-m_nHashOffSet;
	
	if((iOffSet>=0)&&(iOffSet<(INT)m_nItemRulerCount))
	{
		return &m_pItemRulerData[iOffSet];
	}

	return NULL;

	__LEAVE_FUNCTION

	return NULL;

}

VOID	ItemTable::InitMonsterDropTable()
{
	__ENTER_FUNCTION

		enum	MonsterDropTBData
		{
			TBDropSetID					=			0,
			TBMonsterDropContent		=			1
		};

		DBCFile ThirdFile(0);
		BOOL ret = ThirdFile.OpenFromTXT( FILE_MONSTER_DROP_CONTENT );
		Assert(ret);

		INT iTableCount		=	ThirdFile.GetRecordsNum();
		INT iTableColumn	=	ThirdFile.GetFieldsNum();

		Assert(iTableCount>0);

		m_nMonsterDropCount			=	iTableCount;
		m_pMonsterDropTableData		=	new	MONSTER_DROP_TB[iTableCount];

		INT iColumn		=		0;
		for(INT i =0;i<iTableCount;i++)
		{
			m_pMonsterDropTableData[i].m_DropId		=	ThirdFile.Search_Posistion(i,TBDropSetID)->iValue;
				
			iColumn = TBMonsterDropContent;
			for(INT j= 0 ;j<MAX_MONSTER_DROPBOX;j++)
			{
				m_pMonsterDropTableData[i].m_Drops[j].m_nItemValue		=	ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				if(m_pMonsterDropTableData[i].m_Drops[j].m_nItemValue == -1)
					break;
				
				m_pMonsterDropTableData[i].m_Drops[j].m_nCycle			=	ThirdFile.Search_Posistion(i,iColumn++)->iValue;
				//需要解析字符串
				const CHAR*  pSplitString	=	ThirdFile.Search_Posistion(i,iColumn++)->pString;

				SplitStringLevelOne pSplitL1;
				SplitStringLevelTwo pSplitL2;
				pSplitL1.Reset();
				pSplitL2.Reset();
				pSplitL1.Init( '|', &pSplitL2 );
				pSplitL2.Init( '~', NULL );
				pSplitL1.DoSplit( pSplitString );
				INT iLineCount	= pSplitL2.GetResultLineCount();
				m_pMonsterDropTableData[i].m_Drops[j].m_pDropContent = new INT[iLineCount];

				for( INT nID=0; nID<iLineCount; ++nID )
				{
					m_pMonsterDropTableData[i].m_Drops[j].m_pDropContent[nID] = atoi(pSplitL2.GetResultLine(nID));
				}
				m_pMonsterDropTableData[i].m_Drops[j].m_nItemCount = iLineCount;
				++m_pMonsterDropTableData[i].m_ValidCount;
			}
		}


		Log::SaveLog( SERVER_LOGFILE, "InitMonsterDropTable()  Ok!") ;

		__LEAVE_FUNCTION
}


VOID	ItemTable::InitDropAttTable()
{
	__ENTER_FUNCTION
	
	enum	DropAttTBData
	{
			TBDeltaLevel			=		0,
			TBRate					=		1,
			TBExpRate				=		2,
			TBTeamRate				=		3,
			TBTeamExpRate			=		4,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DROP_ATTENUATION );
	Assert(ret);
		
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nDropAttCount		=	iTableCount;
	m_pDropAttData		=	new DROP_ATT_TB[iTableCount];

	Assert(iTableColumn>TBRate);
	Assert(m_pDropAttData);

	for(INT i = 0 ;i<iTableCount;i++)
	{
		m_pDropAttData[i].m_DeltaLevel		=	ThirdFile.Search_Posistion(i,TBDeltaLevel)->iValue;
		m_pDropAttData[i].m_AttValue		=	ThirdFile.Search_Posistion(i,TBRate)->fValue;	
		m_pDropAttData[i].m_ExpValue		=	ThirdFile.Search_Posistion(i,TBExpRate)->fValue;
		m_pDropAttData[i].m_TeamAttValue	=	ThirdFile.Search_Posistion(i,TBTeamRate)->fValue;
		m_pDropAttData[i].m_TeamExpValue	=	ThirdFile.Search_Posistion(i,TBTeamExpRate)->fValue;
	}


	Log::SaveLog( SERVER_LOGFILE, "InitDropAttTable()  Ok!") ;

	__LEAVE_FUNCTION
}

VOID    ItemTable::InitItemDropPos()
{
	__ENTER_FUNCTION

	enum	DropPosTBData
	{
		TBDropX					=		1,
		TBDropY					=		2,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DROP_POS );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();


	m_nDropPosCount		=	iTableCount;
	m_pDropPosData		=	new DROP_POS_TB[iTableCount];

	Assert(iTableColumn>TBDropY);
	Assert(m_pDropPosData);

	for(INT i = 0 ;i<iTableCount;i++)
	{
		m_pDropPosData[i].m_DropX		=	ThirdFile.Search_Posistion(i,TBDropX)->fValue;
		m_pDropPosData[i].m_DropZ		=	ThirdFile.Search_Posistion(i,TBDropY)->fValue;	
	}

	Log::SaveLog( SERVER_LOGFILE, "InitItemDropPos()  Ok!") ;

	__LEAVE_FUNCTION
}

VOID	ItemTable::InitEquipSetAttrTable()
{
	__ENTER_FUNCTION
	
	enum	EquipSetAttr
	{
		TBEquipSetSerial		=		0,
		TBDes					,
		TBSetCount				,
		TBSetDes				,
		TBEquipSetAttrBegin		,
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EQUIP_SET_ATTR );
	Assert(ret);
	
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();
	
	m_nEquipSetCount	=	iTableCount;

	m_pEquipSetData		=	new EQUIP_SET_TB[iTableCount];
	
	Assert(iTableColumn>TBEquipSetAttrBegin);
	Assert(m_pEquipSetData);
	
	INT		iColumn			=	0;
	INT		iValue			=	0;

	for(uint i =0;i<m_nEquipSetCount;i++)
	{
		m_pEquipSetData[i].m_nEquipSetSerial	=	ThirdFile.Search_Posistion(i,TBEquipSetSerial)->iValue;
		m_pEquipSetData[i].m_nEquipSetCount		=	ThirdFile.Search_Posistion(i,TBSetCount)->iValue;
		
		INT iAttrCount =	0;
		iColumn		=	TBEquipSetAttrBegin;
		for(INT j = 0;j<MAX_ITEM_SET_ATTR;j++)
		{
			Assert(iColumn<iTableColumn);
			
			iValue		=	ThirdFile.Search_Posistion(i,iColumn)->iValue;
			if((iValue>=0)&&(iAttrCount<MAX_ITEM_SET_ATTR))
			{
				m_pEquipSetData[i].m_ItemAttr[j].m_AttrType			=	(ITEM_ATTRIBUTE)iValue;
				++iColumn;
				m_pEquipSetData[i].m_ItemAttr[j].m_Value.m_Value	=	ThirdFile.Search_Posistion(i,iColumn)->iValue;
				++iColumn;
				m_pEquipSetData[i].m_ActiveAttrNeedNum[j]	=	ThirdFile.Search_Posistion(i,iColumn)->iValue;
				++iColumn;
				++iAttrCount;
			}
			else
				break;
			
		}
		m_pEquipSetData[i].m_nAttrCount	= iAttrCount;
	}

	__LEAVE_FUNCTION
}


VOID	ItemTable::InitMountAttrTable()    // [7/8/2010 陈军龙]
{
	__ENTER_FUNCTION

	enum	MountAttr
	{
		TBMountSerial		=  0,
		TBName					,
		TBDes					,
        TBIcon                  ,
		TBRulerID               ,
        TBNeedLevel             ,
		TBAddtionSpeed			,
		TBLife                  ,
		TBBindType              ,
		TBPrice                 ,
		TBCharMountID           ,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MOUNT_ATTR );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nMountAttInfoCount	=	iTableCount;

	m_pMountAttInfoData		=	new MOUNT_INFO_TB[iTableCount];

	Assert(iTableColumn>TBAddtionSpeed);
	Assert(m_pMountAttInfoData);

	INT		iColumn			=	0;
	INT		iValue			=	0;

	for(uint i =0;i<m_nMountAttInfoCount;i++)
	{
	    m_pMountAttInfoData[i].m_nTableIndex	=	ThirdFile.Search_Posistion(i,TBMountSerial)->iValue;
		m_pMountAttInfoData[i].m_nRule       	=	ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pMountAttInfoData[i].m_nNeedLevel   	=	ThirdFile.Search_Posistion(i,TBNeedLevel)->iValue;
		m_pMountAttInfoData[i].m_nAddtionSpeed	=	ThirdFile.Search_Posistion(i,TBAddtionSpeed)->iValue;
		m_pMountAttInfoData[i].m_nLife	        =	ThirdFile.Search_Posistion(i,TBLife)->iValue;
		m_pMountAttInfoData[i].m_nBindType   	=	ThirdFile.Search_Posistion(i,TBBindType)->iValue;
		m_pMountAttInfoData[i].m_nPrice     	=	ThirdFile.Search_Posistion(i,TBPrice)->iValue;
	}

	__LEAVE_FUNCTION
}

MONSTER_DROP_TB*	ItemTable::GetMonsterDropTB(INT iDropId)
{
	__ENTER_FUNCTION
	
		MONSTER_DROP_TB	tb;
		tb.m_DropId	=	iDropId;
		
		MONSTER_DROP_TB* pResult  = (MONSTER_DROP_TB*)bsearch(&tb,
										m_pMonsterDropTableData,
										m_nMonsterDropCount,
										sizeof(MONSTER_DROP_TB),
										(INT (*)(const VOID*, const VOID*))CompareMonsterDropTB);

		if(pResult)
			return pResult;
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Search Obj_Monster Drop iDropId:%d Get Errors", 
				iDropId ) ;
		}
		
	__LEAVE_FUNCTION

	return 0;
}

DROP_POS_TB*			ItemTable::GetDropPosTB(INT nDropPoint)
{
	__ENTER_FUNCTION

	if((UINT)nDropPoint< m_nDropPosCount)
	{
		return &m_pDropPosData[nDropPoint];
	}

	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

UINT ItemTable::GetEquipPointValue(HUMAN_EQUIP equipPoint)
{
	Assert(equipPoint<HEQUIP_NUMBER);
	return m_nEquipPointInfo[equipPoint];
}

PAI_INFO* ItemTable::GetPaiInfo(UINT nType)
{
	if (nType>EQUIP_PAI_NUM)
	{
		return NULL;
	}
	return &m_nPaiInfo[nType];
}

UINT ItemTable::GetZiPai(UINT nSeed)
{
	if (nSeed>m_nZiPaiCount)
	{
		return INVALID_PAI_TYPE;
	}
	return m_nZIPai[nSeed];
}


UINT ItemTable::GetNoZiPai(UINT nSeed)
{
	if (nSeed>m_nNoZIPaiCount)
	{
		return INVALID_PAI_TYPE;
	}
	return m_nNoZIPai[nSeed];
}

UINT ItemTable::GetZiPaiCount()
{
	return m_nZiPaiCount;
}

UINT ItemTable::GetNoZiPaiCount()
{
	return m_nNoZIPaiCount;
}

EQUIP_TB*			ItemTable::GetEquipTB(UINT itemSerial)
{

	__ENTER_FUNCTION

	EQUIP_TB	tb;
	tb.m_IndexID		=	itemSerial;

	EQUIP_TB*	pResult	=	(EQUIP_TB*)bsearch(&tb,
		m_pEquipTableData,
		m_nEquipCount,
		sizeof(EQUIP_TB),
		(INT (*)(const VOID*, const VOID*))CompareEquipTB);

	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search Equip ItemSerial:%d  Get Errors", 
			itemSerial) ;
	}


	__LEAVE_FUNCTION
		return	0;
}

EQUIP_ATT_LEVEL_TB*			ItemTable::GetAttLevelValue(ITEM_ATTRIBUTE iAtt)
{
	__ENTER_FUNCTION

	if((iAtt>=0)&&(iAtt<(INT)MAX_ATT_COUNT))
	{
		return &m_pAttLevelTableData[iAtt];
	}

	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

EQUIP_ATT_SET_TB*			ItemTable::GetAttSetTB(INT nSetID)
{
	__ENTER_FUNCTION

	if((nSetID>0)&&(nSetID<(INT)m_nEquipSetCount))
	{
		return &m_pEquipAttSetTableData[nSetID];
	}

	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

EQUIP_ATT_SET_TB*			ItemTable::GetAttSetTBByRate(INT nRate)
{
	__ENTER_FUNCTION

#define		MAX_EQUIP_QUALITY_RAND	10000


		INT SumInt		=	0;

	for(UINT i = 0;i<m_nEquipAttSetCount;++i)
	{
		if(m_pEquipAttSetTableData[i].m_AttSetRate>0)
		{
			if(SumInt+m_pEquipAttSetTableData[i].m_AttSetRate>=nRate)
				return &m_pEquipAttSetTableData[i];
			else
				SumInt	+=	m_pEquipAttSetTableData[i].m_AttSetRate;
		}
		else
			continue;
	}
	return	NULL;

	__LEAVE_FUNCTION
}

UINT			ItemTable::GetSumRateByReqLevel(INT nLevel)
{
	__ENTER_FUNCTION

	for(INT i = m_nEquipAttSetCount-1; i>=0; --i)
	{
		if(nLevel > m_pEquipAttSetTableData[i].m_ReqLevel)
		{
				return m_pEquipAttSetTableData[i].m_SumRate;
		}
	}
	Assert(0);

	__LEAVE_FUNCTION
		return	0;
}

DROP_ATT_TB*	ItemTable::GetDropAttTB(INT iDeltaLevel)
{
	__ENTER_FUNCTION
		DROP_ATT_TB	tb;

		tb.m_DeltaLevel	=	iDeltaLevel;


		DROP_ATT_TB*	pResult		=	(DROP_ATT_TB*)bsearch(&tb,
															  m_pDropAttData,
															  m_nDropAttCount,
															  sizeof(DROP_ATT_TB),
															  (INT (*)(const VOID*, const VOID*))CompareDropAttTB);

		if(pResult)	
			return pResult;
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Search DropAttTB Delta Level: %d Get Errors", 
				iDeltaLevel) ;
		}

	__LEAVE_FUNCTION
		return	0;
}

GEMINFO_TB*		ItemTable::GetGemInfoTB(UINT TableIndex)
{
	__ENTER_FUNCTION
	GEMINFO_TB tb;

	tb.m_nTableIndex	= TableIndex;


	GEMINFO_TB* pResult  =  (GEMINFO_TB*)bsearch(&tb,
												m_pGemInfoData,
												m_nGemCount,
												sizeof(GEMINFO_TB),
												(INT (*)(const VOID*, const VOID*))CompareGemInfoTBByTI);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search GemInfoTB Index:%d  Get Errors", 
			TableIndex) ;

	}

	__LEAVE_FUNCTION
	return	0;
}

EQUIPHOLE_INFO_TB*			ItemTable::GetEquipHoleInfoTB(UINT	nHoleNum)
{
	__ENTER_FUNCTION

	if((nHoleNum<=MAX_ITEM_GEM)&&(nHoleNum!=0))
	{
		return &m_EquipHoleInfoData[--nHoleNum];
	}
	
	__LEAVE_FUNCTION
		return NULL;
}

EQUIPCHAIN_INFO_TB*			ItemTable::GetEquipChainInfoTB(UINT	nIndex)
{
	__ENTER_FUNCTION

	if (nIndex<m_nEquipChainInfoCount)
	{
		return &m_EquipChainInfoData[nIndex];
	}
	return NULL;	
	__LEAVE_FUNCTION
			return NULL;
}

EQUIPSOUL_INFO_TB*			ItemTable::GetEquipSoulInfoTB(UINT	nIndex)
{
	__ENTER_FUNCTION

		if (nIndex<m_nEquipSoulInfoCount)
		{
			return &m_EquipSoulInfoData[nIndex];
		}
		return NULL;	
		__LEAVE_FUNCTION
			return NULL;
}

EQUIP_LEVELUP_TB*			ItemTable::GetEquipLeveupTB(UINT nLevel)
{
	__ENTER_FUNCTION
		
		if((nLevel<=MAX_EQUIP_LEVELUP_NUM)&&(nLevel!=0))
		{
			return &m_EquipLeveupInfoData[--nLevel];
		}

		__LEAVE_FUNCTION
			return NULL;
}

EQUIP_BIND_TB*			ItemTable::GetEquipBindTB(UINT nLevel)
{
	__ENTER_FUNCTION

		if((nLevel<=MAX_EQUIP_BIND_LEVEL)&&(nLevel!=0))
		{
			return &m_EquipBindInfoData[--nLevel];
		}

		__LEAVE_FUNCTION
			return NULL;
}

EQUIP_SPECIALREPAIR_TB*		ItemTable::GetSpecialRepairTB(/*UINT itemSerial*/)
{
	__ENTER_FUNCTION
		/*EQUIP_SPECIALREPAIR_TB tb;

	tb.m_ItemID	= itemSerial;


	EQUIP_SPECIALREPAIR_TB* pResult  =  (EQUIP_SPECIALREPAIR_TB*)bsearch(&tb,
		m_EquipSpecialRepairData,
		m_nSpecialRepairCount,
		sizeof(EQUIP_SPECIALREPAIR_TB),
		(INT (*)(const VOID*, const VOID*))CompareSpecialRepairInfoTBByTI);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search EQUIP_SPECIALREPAIR_TB Index:%d  Get Errors", 
			itemSerial) ;

	}*/
	if (m_nSpecialRepairCount < 1)
	{
		return NULL;
	}
	return &m_EquipSpecialRepairData[0];
	
	__LEAVE_FUNCTION
		return	0;
}

ITEM_PRODUCT_INFO_TB*	ItemTable::GetItemProductInfoTB(UINT nProductID)
{
	__ENTER_FUNCTION

	ITEM_PRODUCT_INFO_TB tb;
	tb.m_ProductID		=	nProductID;

	ITEM_PRODUCT_INFO_TB* pResult = (ITEM_PRODUCT_INFO_TB*)bsearch(&tb,
		m_pProductInfoData,
		m_nProductCount,
		sizeof(ITEM_PRODUCT_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareProductInfoTB);
	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search CompareProductInfoTB nProductID:%d ", 
			nProductID) ;
		return 0;
	}

		__LEAVE_FUNCTION
			return NULL;
}

MATERIAL_LEVEL_INFO_TB*			ItemTable::GetMaterialInfoTB(UINT nLevel)
{
	__ENTER_FUNCTION

		if((nLevel<=MAX_VALIDMATERIAL_LEVEL)&&(nLevel!=0))
		{
			return &m_MaterialLevelInfoData[--nLevel];
		}

		__LEAVE_FUNCTION
			return NULL;
}

EQUIPEXTRATT_TB*			ItemTable::GetEquipExtraAttInfoTB(UINT nScore)
{
	__ENTER_FUNCTION
		
		for (int i=0; i<MAX_VALIDMATERIAL_LEVEL; ++i)
		{
			if (nScore<=m_EquipExtraInfoData[i].m_nEquipScore)
			{
				return &m_EquipExtraInfoData[i];
			}
		}
		
	__LEAVE_FUNCTION
			return NULL;
}

EXTRABAG_INFO_TB*			ItemTable::GetExtraBagInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

		EXTRABAG_INFO_TB tb;
	tb.m_nTableIndex		=	itemSerial;

	EXTRABAG_INFO_TB* pResult = (EXTRABAG_INFO_TB*)bsearch(&tb,
		m_pExtraBagInfoData,
		m_nExtraBagInfoCount,
		sizeof(EXTRABAG_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareExtraBagInfoTB);
	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search GetExtraBagInfoTB itemSerial:%d  Get Errors", 
			itemSerial) ;
		return 0;
	}

	__LEAVE_FUNCTION
		return 0;
}

// [7/14/2010 陈军龙]
MOUNT_INFO_TB*			ItemTable::GetMountInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

		MOUNT_INFO_TB tb;
	tb.m_nTableIndex		=	itemSerial;

	MOUNT_INFO_TB* pResult = (MOUNT_INFO_TB*)bsearch(&tb,
		m_pMountAttInfoData,
		m_nMountAttInfoCount,
		sizeof(MOUNT_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareMountInfoTB);
	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search GetMountInfoTB itemSerial:%d  Get Errors", 
			itemSerial) ;
		return 0;
	}

	__LEAVE_FUNCTION
		return 0;
}

CHANGEPAI_INFO_TB*			ItemTable::GetChangePaiInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

		CHANGEPAI_INFO_TB tb;
	tb.m_nTableIndex		=	itemSerial;

	CHANGEPAI_INFO_TB* pResult = (CHANGEPAI_INFO_TB*)bsearch(&tb,
		m_pChangePaiInfoData,
		m_nChangePaiInfoCount,
		sizeof(CHANGEPAI_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareChangePaiInfoTB);
	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search GetChangePaiInfoTB itemSerial:%d  Get Errors", 
			itemSerial) ;
		return 0;
	}

	__LEAVE_FUNCTION
		return 0;
}

COMMITEM_INFO_TB*		ItemTable::GetCommItemInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

		COMMITEM_INFO_TB tb;
			tb.m_nTableIndex		=	itemSerial;
		
		COMMITEM_INFO_TB* pResult = (COMMITEM_INFO_TB*)bsearch(&tb,
														 m_pCommItemInfoData,
														 m_nCommItemCount,
														 sizeof(COMMITEM_INFO_TB),
														(INT (*)(const VOID*, const VOID*))CompareCommItemInfoTB);
		if(pResult)
				return	pResult;
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Search CommItemInfoTB itemSerial:%d  Get Errors", 
						itemSerial) ;
			return 0;
		}

	__LEAVE_FUNCTION
		return 0;
}

TASKITEM_INFO_TB*		ItemTable::GetTaskItemInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

	TASKITEM_INFO_TB tb;
	tb.m_nTableIndex		=	itemSerial;

	TASKITEM_INFO_TB* pResult = (TASKITEM_INFO_TB*)bsearch(&tb,
		m_pTaskItemInfoData,
		m_nTaskItemCount,
		sizeof(TASKITEM_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareTaskItemInfoTB);
	if(pResult)
		return	pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search TaskItemInfoTB itemSerial:%d  Type:%d Get Errors", 
			itemSerial) ;
		return 0;
	}

	__LEAVE_FUNCTION
		return 0;

}

STORE_MAP_INFO_TB* ItemTable::GetStoreMapTB(UINT itemSerial)
{
	__ENTER_FUNCTION

		STORE_MAP_INFO_TB	tb;
		tb.m_nTableIndex	=	 itemSerial;

		STORE_MAP_INFO_TB* pResult = (STORE_MAP_INFO_TB*)bsearch(&tb,
																m_pStoreMapData,
																m_nStoreMapCount,
																sizeof(STORE_MAP_INFO_TB),
																(INT (*)(const VOID*, const VOID*))CompareStoreMapInfoTB);
		if(pResult)
			return		pResult;
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Search StoreMapInfoTB itemSerial:%d Get Errors", 
						itemSerial) ;
		}

	__LEAVE_FUNCTION
		return	0;
}

// add by gh 2010/05/05
SOUXIA_INFO_TB*	ItemTable::GetSouXiaTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

	SOUXIA_INFO_TB	tb;
	tb.m_nTableIndex	=	 itemSerial;

	SOUXIA_INFO_TB* pResult = (SOUXIA_INFO_TB*)bsearch(&tb,
		m_pSouXiaData,
		m_nSouXiaCount,
		sizeof(SOUXIA_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareInfoTB);
	if(pResult)
		return		pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search SOUXIA_INFO_TB itemSerial:%d Get Errors", 
			itemSerial) ;
	}

	__LEAVE_FUNCTION
		return	0;
}

// add by gh 2010/05/05
SOUL_BEAD_INFO_TB*	ItemTable::GetSoulBeadTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

	SOUL_BEAD_INFO_TB	tb;
	tb.m_nTableIndex	=	 itemSerial;

	SOUL_BEAD_INFO_TB* pResult = (SOUL_BEAD_INFO_TB*)bsearch(&tb,
		m_pSoulBeadData,
		m_nSoulBeadCount,
		sizeof(SOUL_BEAD_INFO_TB),
		(INT (*)(const VOID*, const VOID*))CompareInfoTB);

	if(pResult)
		return		pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search SOUXIA_INFO_TB itemSerial:%d Get Errors", 
			itemSerial) ;
	}

	__LEAVE_FUNCTION
		return	0;
}
// end of add
EQUIP_SET_TB*		ItemTable::GetEquipSetTB(INT EquipSetSerial)
{
	__ENTER_FUNCTION
		
		EQUIP_SET_TB	tb;

		tb.m_nEquipSetSerial	=	EquipSetSerial;

		EQUIP_SET_TB* pResult = (EQUIP_SET_TB*)bsearch(&tb,
														m_pEquipSetData,
														m_nEquipSetCount,
														sizeof(EQUIP_SET_TB),
														(INT (*)(const VOID*, const VOID*))CompareEquipSetTB);
		if(pResult)
				return pResult;
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Search EquipSetTB Serial:%d Get Errors", 
				EquipSetSerial) ;
		}

	__LEAVE_FUNCTION
		return	0;
}


BYTE	GetItemTileMax(_ITEM_TYPE& it)
{
	__ENTER_FUNCTION

	if(it.m_Class==ICLASS_EQUIP)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_GEM)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_STOREMAP)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_EXTRABAG)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_COMITEM || it.m_Class == ICLASS_IDENT)
	{
		COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else if(it.m_Class==ICLASS_MATERIAL)
	{
		COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else if(it.m_Class==ICLASS_TASKITEM)
	{
		TASKITEM_INFO_TB*	pGET		=		g_ItemTable.GetTaskItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else if(it.m_Class==ICLASS_SOUL_BEAD)//add by gh 2010/06/28
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_MOUNT)  //  [7/14/2010 陈军龙]
	{
		MOUNT_INFO_TB*	pGET		=		g_ItemTable.GetMountInfoTB(it.ToSerial());
		if(pGET)
		{
			return 1;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else
	{
		AssertEx(FALSE,"Not Implement Type in GetItemTileMax()");	// Not Implement Type
		return 0;
	}

	__LEAVE_FUNCTION
	return 0 ;
}

BOOL EquipCanAddHole(EQUIP_TYPE equip_type)
{
	//2 4 6 8 10 12 14 16
	if (EQUIP_GUN == equip_type 
		|| EQUIP_BANG == equip_type 
		|| EQUIP_JIAN == equip_type 
		|| EQUIP_DADAO == equip_type
		|| EQUIP_SHANZI == equip_type
		|| EQUIP_HULU == equip_type
		|| EQUIP_DIZI == equip_type
		|| EQUIP_CI == equip_type
		|| EQUIP_CAP == equip_type
		|| EQUIP_NECKLACE == equip_type
		|| EQUIP_SHOULDER == equip_type
		|| EQUIP_ARMOR == equip_type
		|| EQUIP_CUFF == equip_type
		|| EQUIP_HAND == equip_type
		|| EQUIP_SASH == equip_type
		|| EQUIP_BOOT == equip_type
		|| EQUIP_RING == equip_type
		|| EQUIP_ADORN == equip_type
		|| EQUIP_SUIT == equip_type
		)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}