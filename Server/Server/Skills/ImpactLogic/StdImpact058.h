/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   12:08
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact058.h
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact058
	file ext:	h
	author:		Richard
	
	purpose:	先知技能使用后，被击者身上的持续效果, 为持续性伤害

				1 此次效果实现持续性的按周期的伤害

				2 特别之处在于m_uUniquidID 中记录造成此效果的小球的全局ID

				3 m_uUniquidID在效果消散时要发送给client

				4 在 Obj_Character::Impact_OnImpactFadeOut中，向client发送
				  效果消散消息的地方负责获取
*********************************************************************/
#ifndef StdImpact058_h__24_11_2007_14_42
#define StdImpact058_h__24_11_2007_14_42

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

 
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact058_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_058,
				};
				class OwnImpactParamIndex_T
				{
				public:
					enum
					{
						IDX_HP=0,
						IDX_MP,
						IDX_RAGE,
						IDX_STRIKE_POINT,				
					};
				};
				StdImpact058_T();
				~StdImpact058_T();
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				//是否为持续性效果
				virtual BOOL IsOverTimed(VOID)  const {return TRUE;}
				//是否为周期性发作效果
				virtual BOOL IsIntervaled(VOID) const {return TRUE;}
				//当发作时间到了会调用此消息函数, 在此方法中实现具体的对拥有者的伤害
				virtual VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;

			public: //message handler
				//效果激活时的消息函数
				virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				//效果消散时的消息函数,在此处为向client发送消息并携带m_uUniquidID信息
				//向小球的发送者(发送小球的效果对象)通知,以使其维护小球的正确计数
				virtual VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
				INT GetHpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HP);}
				VOID SetHpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HP, nValue);}

				INT GetMpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_MP);}
				VOID SetMpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_MP, nValue);}

				INT GetRageModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE);}
				VOID SetRageModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE, nValue);}

			private:

		} DI_IntervalOverHurt_T;
	};
};

#endif //StdImpact058_h__24_11_2007_14_43
