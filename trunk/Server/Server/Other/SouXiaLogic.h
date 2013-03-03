/************************************************************************/
/* 功能:捜侠录的逻辑操作处理,目前拆解和炼化部分放到这里
*/
/************************************************************************/

#ifndef _SOUXIA_LOGIC_H_
#define _SOUXIA_LOGIC_H_

class SouXiaLogic
{
public:
	SouXiaLogic()	{}
	~SouXiaLogic()	{}
	
	//拆解
	static	SHORT	DismantleSouXiaSkill(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// 拆解捜侠录技能页
	static	SHORT	DismantleSouXiaProduct(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);	// 拆解捜侠录配方页
	static	SHORT	DismantleSouXiaPet(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// 拆解捜侠录神兽召唤技能
	static	SHORT	DismantleSouXiaZuoJi(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// 拆解捜侠录坐骑召唤技能

	//炼化
	static BOOL RefiningSouXia(Obj_Human* pHuman, Item* pSouXiaItem1, Item* pSouXiaItem2, BYTE refiningRule, SOUXIA_DATA& randSouXiaData, const SOUXIA_PAGE_TYPE& type);
	
	//缝合
private:
	//炼化
	static SHORT RefiningSouXiaSkill(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// 炼化捜侠录技能
	static SHORT RefiningSouXiaProduct(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);	// 炼化捜侠录配方
	static SHORT RefiningSouXiaPet(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// 炼化捜侠录神兽召唤
	static SHORT RefiningSouXiaZuoJi(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// 炼化捜侠录坐骑召唤
};

#endif // _SOUXIA_LOGIC_H_

