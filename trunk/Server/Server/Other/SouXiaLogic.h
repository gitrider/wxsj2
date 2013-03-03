/************************************************************************/
/* ����:����¼���߼���������,Ŀǰ�����������ַŵ�����
*/
/************************************************************************/

#ifndef _SOUXIA_LOGIC_H_
#define _SOUXIA_LOGIC_H_

class SouXiaLogic
{
public:
	SouXiaLogic()	{}
	~SouXiaLogic()	{}
	
	//���
	static	SHORT	DismantleSouXiaSkill(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// ������¼����ҳ
	static	SHORT	DismantleSouXiaProduct(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);	// ������¼�䷽ҳ
	static	SHORT	DismantleSouXiaPet(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// ������¼�����ٻ�����
	static	SHORT	DismantleSouXiaZuoJi(BYTE rule,  SOUXIA_DATA& dismantleSouXiaData, SOUXIA_DATA* souXiaData, BYTE& newPages, const BYTE curSouXiaCount);		// ������¼�����ٻ�����

	//����
	static BOOL RefiningSouXia(Obj_Human* pHuman, Item* pSouXiaItem1, Item* pSouXiaItem2, BYTE refiningRule, SOUXIA_DATA& randSouXiaData, const SOUXIA_PAGE_TYPE& type);
	
	//���
private:
	//����
	static SHORT RefiningSouXiaSkill(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// ��������¼����
	static SHORT RefiningSouXiaProduct(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);	// ��������¼�䷽
	static SHORT RefiningSouXiaPet(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// ��������¼�����ٻ�
	static SHORT RefiningSouXiaZuoJi(BYTE rule, SOUXIA_DATA& randSouXiaData, SOUXIA_DATA& souXiaData, SOUXIA_DATA& souXiaData2, BYTE maxPages);		// ��������¼�����ٻ�
};

#endif // _SOUXIA_LOGIC_H_

