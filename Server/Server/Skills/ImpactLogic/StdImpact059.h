/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   13:56
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact059.h
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact059
	file ext:	h
	author:		Richard
	
	purpose:	先知(或同类型)技能使用并生效后,维护使用者自身的此技能的属性和状态

				1 首先读取Impact表格中小球的总数量记录在m_uBallCount中

				2 当前的小球全局唯一ID初始化为0,为非法ID

				3 从全局小球计数器中获取才能获取小球的全局ID,不可在此类中自行计算
                  否则有可能会有重复ID,造成错误

			    4 每当周期性发作时OnIntervalOver消息句柄会被调用,在OnIntervalOver
				  里会对目标执行搜索,找到有效目标时,判断目标是否已经中了相同的效果
				  如果有则忽略目标,否则,判断小球数量是否>0,如果是,向目标发送效果,
				  并将小球计数减1

			    5 在自身消散时无需任何特殊处理

				6 OnActive 不需要特殊处理

				7 产生此类效果的技能逻辑ID应为IMPACTS_TO_TARGET,目标应该是自己

				8 在 skl_template 表格中目标选择逻辑应该是 TARGET_SELF（0）

				9 在此类技能使用时，只在使用者身上注册此效果即可，具体的攻击等行为
				  由本效果对象负责实现
*********************************************************************/
#ifndef StdImpact059_h__24_11_2007_14_43
#define StdImpact059_h__24_11_2007_14_43

#include "ImpactLogic.h"


namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact059_T : public ImpactLogic_T
		{
			struct StdImpact059_Ball
			{
				UINT m_uUniqueID;
				Obj_Character* m_pCharacter;
				StdImpact059_Ball()
				{
					memset( this, 0, sizeof(StdImpact059_Ball) );
				}
			};
			public:
				enum
				{
					ID = STD_IMPACT_059,
				};
				StdImpact059_T();
				~StdImpact059_T();
				//从std_impact表格的存储对象总读取小球的数量
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				//是否为持续性技能
				BOOL IsOverTimed(VOID)  const { return TRUE; }
				//是否周期性发作
				BOOL IsIntervaled(VOID) const { return TRUE;}

			public: //message handler
				//效果激活时的消息函数
				virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				//当发作时间到了会调用此消息函数,搜索有效目标,向目标注册效果,维护
				//小球的计数
				virtual VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
				//从场景中搜索合法的目标放在rTargets中，rTargets为输出参数
				BOOL ScanUnitForTarget(OWN_IMPACT& rImp, Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets) const;
				//根据击中概率等信息判断合法的目标是否真的被击中
				BOOL HitThisTarget(Obj_Character& rMe, Obj_Character& rTar, SkillID_t nSkillID) const;
				//实际的概率计算
				BOOL IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const;
				VOID BroadcastTargetListMessage( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& rHitFlagsForObjList ) const;
				INT GetSubImpactDataIndexByIndex(OWN_IMPACT const& rImp, INT nIndex) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						nIndex,
						rImp.GetSkillLevel()
						);
				}
				//判断是否可以向目标投送效果，主要有两点需要考虑:
				// 1 目标身上是否已经具有这个Buff
				// 2 这个Buff是否是投送者发出的
				//如果 1 不满足则允许投送
				//如果 1 满足 2 不满足则允许投送，否则不能投送
				BOOL IsCanSendToUnit( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* pChar ) const;
				//************************************
				// Method:    GetAccuracy
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::GetAccuracy
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: SkillID_t nSkillID
				// 由于多个效果会公用一个效果对象，所以在此对象中不能保存效果处理相关的属性
				// 必须在每一次发作时从OWN_IMPACT对象中获取相关的信息。此方法负责根据OWN_IMPACT
				// 对象中的skillID获取命中率并暂时保存在m_iAccuracy中。m_iAccuracy不能直接使用
				//************************************
				VOID GetAccuracy( SkillID_t nSkillID ) const;
				//************************************
				// Method:    DoHitTarget
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::DoHitTarget
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: [in]OWN_IMPACT& rImp
				// Parameter: [in]Obj_Character& rMe
				// Parameter: [out]OBJLIST & rTargets 目标列表
				// Parameter: [out]HitFlagsForOBJLIST_T & HitFlagList 是否击中的标记列表，与rTargets一一对应
				// Parameter: [out]UINT & nCount 实际击中的目标个数
				//************************************
				VOID DoHitTarget( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& HitFlagList, UINT& nCount ) const;
				//************************************
				// Method:    SendLayerChangeMsg
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::SendLayerChangeMsg
				// Access:    protected 
				// Returns:   VOID
				// Qualifier:
				// Parameter: OWN_IMPACT & rImp
				// Parameter: Obj_Character & rMe
				// 向客户端发送buff层数变化的消息
				//************************************
				VOID SendLayerChangeMsg( OWN_IMPACT& rImp, Obj_Character& rMe ) const;

				//************************************
				// Method:    ComputeLayerChange
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::ComputeLayerChange
				// Access:    protected 
				// Returns:   VOID
				// Qualifier: const
				// Parameter: OWN_IMPACT & rImp
				// Parameter: Obj_Character & rMe
				// Parameter: INT nCount
				// Parameter: INT iCurrentLayer
				// 处理buff层的改变
				//************************************
				VOID ComputeLayerChange( OWN_IMPACT& rImp, Obj_Character& rMe, INT nCount, INT iCurrentLayer ) const;

				//************************************
				// Method:    LayerFadeOut
				// FullName:  Combat_Module::Impact_Module::StdImpact059_T::LayerFadeOut
				// Access:    protected 
				// Returns:   BOOL 如果层数有变化为TRUE,否则为FALSE
				// Qualifier: const
				// Parameter: OWN_IMPACT & rImp
				//************************************
				BOOL LayerFadeOut( OWN_IMPACT& rImp ) const;

			private:
				////先知或同类技能小球的当前数量, 在InitFromData中初始化为表格中的小球总数量
				////此数量记录在template表格中
				//UINT m_uBallCount;
				////当前的小球个数
				//UINT m_uCurrentBallCount;
				StdImpact059_Ball	m_aBallUniqueID[OBJLIST::MAX_OBJ_LIST_SIZE];
				INT					m_iAccuracy;

		} DI_SelfIntervalOverSkill_T;
	};
};

#endif //StdImpact059_h__24_11_2007_14_43
