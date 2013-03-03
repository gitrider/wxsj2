/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   10:12
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact072.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact072
	file ext:	h
	author:		Richard
	
	purpose:	瞬移技能专用Impact
*********************************************************************/

#ifndef StdImpact072_h__
#define StdImpact072_h__


#include "ImpactLogic.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact072_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_072,
				};
				class OwnImpactParamIndex_T
				{
				public:
					enum
					{
						IDX_DISTANCE=0,
					};
				};
				class ImpactDataDescriptorIndex_T
				{
				public:
					enum
					{
						IDX_DISTANCE=0,
					};
				};
				StdImpact072_T() {};
				~StdImpact072_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				INT GetDistance(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
							(rImp.GetDataIndex(), 
							ImpactDataDescriptorIndex_T::IDX_DISTANCE,
							rImp.GetSkillLevel()
							);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;

			protected:
				VOID GetRealPos( Obj_Character &rMe, WORLD_POS &sFinalPos, WORLD_POS &sPosTag, WORLD_POS &sDir ) const;
				VOID GetFinalPos( OWN_IMPACT& rImp, Obj_Character &rMe, WORLD_POS& sFinalPos, WORLD_POS& sDir, WORLD_POS& sPosTag, WORLD_POS& oTargetPos, FLOAT fDistance ) const;
				BOOL GetTargetPos( OWN_IMPACT& rImp, Obj_Character &rMe, WORLD_POS& oTargetPos, FLOAT& fDistance ) const;

				FLOAT GetFinalDistance( Obj_Character &rMe, WORLD_POS &oMyPos, WORLD_POS &oTargetPos, FLOAT fMaxDistance ) const;
			private:
		};
	};
};
#endif // StdImpact072_h__
