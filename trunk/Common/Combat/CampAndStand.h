///////////////////////////////////////////////////////////////////////////////
// �ļ�����CampAndStand.h
// ����Ա������
// ����˵������Ӫ���ݱ�ṹ
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file CampAndStand.h
*		\brief the defination camp and stand record structure 
*/

#ifndef CAMPANDSTAND_H
#define CAMPANDSTAND_H

#include "Type.h"
#include "TemplateManager.h"

namespace DBC
{
	class DBCFile;
}

namespace Combat_Module
{
	using namespace DBC;
	namespace Camp_Stand
	{
		class CampStandRecord_T
		{
			public:
				enum
				{
					MAX_ID = 31,
					MAX_SIZE = MAX_ID+1,
				};
				
				CampStandRecord_T();
				~CampStandRecord_T(){};
				BOOL IsInited(VOID) const{return m_bInited;};
				BOOL InitFromDB(const DBCFile* pDB, INT const nRow);
				BOOL StandByCampID(CampID_t const nCampID)const;
			protected:
			private:
				BOOL m_bInited;
				BOOL m_aStands[MAX_SIZE];
		};
	};
};

using namespace Combat_Module;
using namespace Combat_Module::Camp_Stand;
using namespace Combat_Module::Skill_Module;
typedef class TemplateManager_T<CampStandRecord_T> CampAndStandDataMgr_T;

struct _CAMP_DATA;
ENUM_RELATION CalcRelationType( const _CAMP_DATA *pCamp_A, BOOL bHuman_A, const _CAMP_DATA *pCamp_B, BOOL bHuman_B, const CampAndStandDataMgr_T *pCampMgr );

#endif //CAMPANDSTAND_H
