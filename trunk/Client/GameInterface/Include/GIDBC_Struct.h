
/** GIDBC_Struct.h
 *	
 *	功能：	基本数据库结构
 *
 *	修改记录：
 *			080304	增加 帮主令 在 common.tab 表中固定的ID
 *
 *	修改记录：
 *			08_03_07 修改坐骑表
 *
 *			080331	增加 DayTips.tab
 *			080410	增加 TopList.tab
 *			080410	增加 questlog.tab
 *			100517	增加 Mappic.tab
 *			100517	增加 Chathelp.tab
 *
 */



#pragma once



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								主角相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


// 帮主令， 固定的ID
#define EQUIP_CHIEFTAIN_TAG		11990001



// 装备上的特效
#define  EQUIP_EFFECT_MAX_NUM    16
struct EQUIP_EFFECT
{
	LPCTSTR szEffect;     // 特效id
	LPCTSTR	szBindPoint;  // 特效绑点
}; 

// 怪身上的特效
#define  MONSTER_EFFECT_MAX_NUM     4
struct MONSTER_EFFECT
{
	LPCTSTR	szEffect;
	LPCTSTR	szBind;

	MONSTER_EFFECT()
	{
		szEffect = NULL;
		szBind	 = NULL;
	};
};




/////////////////////////////////////////////////////////////////////
//主角种族							(CHAR_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_RACE			(101)		//主角种族(男，女)
struct _DBC_CHAR_RACE		// bc
{
	INT		nID;
//	INT		nIsPlayer;
//	INT		nGender;
	INT		nModelID;			// 模型
	INT		nDefHairGeo;		// 头
	INT		nDefFaceGeo;		// 脸
	INT		nDefShoulder;		// 肩
	INT		nDefBody;			// 身体
	INT		nDefArm;			// 手
	INT		nDefFoot;			// 脚
	INT		nIdleInterval;		// 休闲时间
};

/////////////////////////////////////////////////////////////////////
// 主角头发模型						(CHAR_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HAIR_GEO		(102)		//主角头发模型
struct _DBC_CHAR_HAIR_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
// 主角脸部模型						(CHAR_04)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HEAD_GEO		(104)		//主角脸部模型
struct _DBC_CHAR_HEAD_GEO		// bc
{
	INT		nID;
	LPCSTR	pMeshFile;
	INT		nRace;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
// 主角动作组						(CHAR_06)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_ACTION_SET	(106)
#define MAX_WEAPON_TYPE_NUMBER		(14)	// 武器类型数量  这个值等于,WEAPON_TYPE_NUMBERS
struct _DBC_CHARACTER_ACTION_SET
{
	INT		nID;
	LPCSTR	pWeapon_Set[MAX_WEAPON_TYPE_NUMBER];	// 对应武器类型 ENUM_WEAPON_TYPE
	BOOL	bHideWeapon;							// 隐藏武器
	INT		nAppointedWeaponID;						// 指定的武器ID
	LPCSTR	pszDesc;								// 描述
};

/////////////////////////////////////////////////////////////////////
// 主角特效							(CHAR_07)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EFFECT		(107)
struct _DBC_CHARACTER_EFFECT
{
	INT		nID;
	INT		nEffect1;
	INT		nEffect2;
	LPCSTR	pLocator;
	INT		nSoundID;
	LPCSTR	pWeaponLocator;
};

/////////////////////////////////////////////////////////////////////
// 主角升级经验值						(CHAR_08)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EXPLEVEL		(108)
struct _DBC_CHARACTER_EXPLEVEL
{
	INT		nID;
	INT		nEffectNeed;
};

/////////////////////////////////////////////////////////////////////
// 外形ID对应模型名称					(CHAR_09)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MODEL			(109)
#define MAX_MOUNT_NUMBER			(20)				// 
#define MAX_SOUND_NUMBER			(6)					// 此数值根据地面的类型得出
struct _DBC_CHARACTER_MODEL		// bc
{
	INT		m_nID;
	FLOAT	m_fFuseTime;								// 动作熔合时间
	LPCSTR	m_pszModelName;								// 模型文件
	INT		m_nSoundId[MAX_SOUND_NUMBER];				// 不同地面跑步的声音
	LPCSTR	m_pszActionSetName_None;					// 不使用坐骑的动作文件	
	LPCSTR	m_apszActionSetName_Mount[MAX_MOUNT_NUMBER];// 使用坐骑后的动作文件	
};

/////////////////////////////////////////////////////////////////////
// 主角头像
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_FACE		(110)		//主角头像
struct _DBC_CHAR_FACE		/// bc
{
	INT		nID;
	LPCSTR	pImageSetName;
	INT		nRace;
};

/////////////////////////////////////////////////////////////////////
// 坐骑表
/////////////////////////////////////////////////////////////////////
#define MOUNT_EFFECT_MAX_NUM 8
struct MOUNT_EFFECT
{
	const CHAR * pszEffectLocator;	// 特效帮定点
	const CHAR * pszEffectName;		// 特效名称
};

#define DBC_CHARACTER_MOUNT		(111)
struct _DBC_CHARACTER_MOUNT		// bc
{
	INT				m_nID;								// 骑乘ID
	FLOAT			m_fAddHeight;						// 马离地面的距离，比如 飞马。
	INT				m_nModelID;							// 模型ID
	INT				m_nCharActionIndex;					// 人物骑乘时的动作组,对应char_model.tab的mount0~19
	MOUNT_EFFECT	m_EffectInfo[MOUNT_EFFECT_MAX_NUM];	// 特效信息
};




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								生物相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// 生物定义总表						(CREATURE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_ATT			(201)
struct _DBC_CREATURE_ATT
{
	INT		nID;						// 怪物编号
	LPCSTR	pName;						// 名称
	INT		nLevel;						// 等级
	INT		NOTCARE1[3];				//* 刷新周期
										// 最小伤血百分比(Hppercent)
										// 基础经验获得(EXP)
	INT		nAttackRate_Near;			// 近程攻击获得(short range att)
	INT		nAttackRate_Far;			// 远程攻击获得(long range att)
	INT		nAttackRate_Magic_Near;		// 近程外功攻击
	INT		nAttackRate_Magic_Far;		// 远程外功攻击
	INT		nDefence_Near;				// 近程防御(short range recovery)
	INT		nDefence_Far;				// 远程防御(long range recovery)
	INT		nDefence_Magic_Near;		// 近程外功防御
	INT		nDefence_Magic_Far;			// 远程外功防御
	INT		nMaxHP;						// HP上限(hp limit)
	INT		nMaxMP;						// MP上限(mp limit)
	INT		nRestoreHP;					// HP回复(hp reversion)
	INT		nRestoreMP;					// MP回复(mp reversion)
	INT		nHit;						// 命中(hit rate)
	INT		nMiss;						// 闪避（volt）
	INT		nCritrate;					// 会心率（huixin)
	INT     nToughness;					// 韧性
	INT		nAttackGold;				// 属性攻击１(shuxingatt1 )	金
	INT		nAttackWood;				// 属性攻击２(shuxingatt2 ) 木
	INT		nAttackWater;				// 属性攻击３(shuxingatt3) 水
	INT		nAttackFire;				// 属性攻击４(shuxingatt4) 火
	INT		nAttackSoil;				// 属性攻击5 (shuxingatt4) 土
	INT		nDefenceGold;				// 属性防御１(shuxingrecovery1)
	INT		nDefenceWood;				// 属性防御２(shuxingrecovery2)
	INT		nDefenceWater;				// 属性防御３(shuxingrecovery3)
	INT		nDefenceFire;				// 属性防御４(shuxingrecovery4)
	INT		nDefenceSoil;				// 属性防御４(shuxingrecovery4)
	INT		NOTCARE2;					//* 调用脚本ID
	INT		nModelID;					// 外型ID
	LPCSTR	szIconName;					// 人物头像
	INT		nIsCanSwerve;				// 是否随玩家转向
	INT		nIsDisplayerName;			// 是否显示头顶信息板
	FLOAT	fBoardHeight;				// 名字板高度
	FLOAT	fProjTexRange;				// 选中环大小
	FLOAT	fShadowRange;				// 阴影大小
	INT		NOTCARE3[2];				// 攻击动作时间
										// 攻击冷却时间
	INT		nIdleInterval;				// 休闲间隔时间
	INT		nSpeed;						// 移动速度
	INT		nMonsterWalkSpeed;			// 步行速度
	INT		nAttackSpeed;				// 攻击速度
	LPCSTR	nMArmID;					// 武器调用ID
	INT		NOTCARE4[4];				//* 马调用ID
										// 基础AI
										// 扩展AI
										// 阵营
	INT		nCannotBeAttack;			// 不可被攻击
	INT		nCanHandle;					// 是否可以交互
	INT		NOTCARE5;					// 友好值
	INT		nMonsterBossFlag;			// BOSS标记
	INT		nMonsterBossSnapImmID;		// 瞬时效果免疫ID
	INT		nMonsterBossDurationImmID;	// 持续效果免疫ID
	INT		NOTCARE6[4];				// 掉落分配半径(米)
										// 掉落最大有效组
										// 掉落物品组
										// 掉落概率
	MONSTER_EFFECT	sMonsterEffect[MONSTER_EFFECT_MAX_NUM];		// 绑在怪身上的特效

	LPCSTR	pHorseDropID;				// 马掉落ID
	INT		nDropID;					// 掉落ID
	INT		nNPCType;					// NPC类型
	LPCSTR	pNPCTypeName;				// NPC类型名
	LPCSTR	pFilterName;				// NPC名称拼音索引
	INT			nCriticalHitDamage;	//暴击伤害 ,不知道用途,表格里有
	INT			nRage;							//怒气 ,不知道用途,表格里有
	INT	nIsPlayAnimation;			//是否有搁挡或者闪避动作
};



/////////////////////////////////////////////////////////////////////
//宠物附加属性						(DBC_PET_EX_ATT)
/////////////////////////////////////////////////////////////////////
#define DBC_PET_EX_ATT				(202)
struct _DBC_PET_EX_ATT		// bc
{
	INT		m_Type;						// 宠物编号
	INT		m_TakeLevel;				// 可携带等级
	LPTSTR	m_Name;						// 名称
	INT		m_Camp;						// 阵营
	INT		m_Reserve1;					// 保留
	INT		m_Reserve2;					// 保留
	INT		m_FoodType;					// 食物类
	INT		m_SkillLearnNum;			// 所能学的技能数
	INT		m_PositiveSkill;			// 主动技能
	INT		m_PassiveSkill1;			// 被动技能1
	INT		m_PassiveSkill2;			// 被动技能2
	INT		m_PassiveSkill3;			// 被动技能3
	INT		m_PassiveSkill4;			// 被动技能4
	INT		m_Life;						// 标准寿命
	INT		m_StrPerception;			// 标准力量资质
	INT		m_ConPerception;			// 标准体质资质
	INT		m_DexPerception;			// 标准灵气资质
//	INT		m_SprPerception;			// 标准身法资质
	INT		m_IntPerception;			// 标准定力资质
	INT		m_GrowRate0;				// 成长率1
	INT		m_GrowRate1;				// 成长率2
	INT		m_GrowRate2;				// 成长率3
	INT		m_GrowRate3;				// 成长率4
	INT		m_GrowRate4;				// 成长率5
	INT		m_CowardiceRate;			// 胆小几率
	INT		m_WarinessRate;				// 谨慎几率	
	INT		m_LoyalismRate;				// 忠诚几率
	INT		m_CanninessRate;			// 精明几率
	INT		m_ValourRate;				// 勇猛几率
};

/////////////////////////////////////////////////////////////////////
//生物声音定义表					(CREATURE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_SOUND			(203)
struct _DBC_CREATURE_SOUND	// bc
{
	INT		nID;
	INT		nUnderAttack;
	INT		nAttack;
	INT		nDeath;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								物品相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//装备类物品显示定义				(ITEM_01)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_CHAR		(301)
struct _DBC_ITEM_VISUAL_CHAR	// bc
{
	#define CHAR_RACE_NUM	(4)

	INT		nID;
//	LPCSTR	pVisualEntity[ 6 * 2 * CHAR_RACE_NUM ];
	INT		pVisualEntityIndex[38];//8个门派*2个方向*2个性别 +无门派*2 + 高模*2
};

#define  EQUIP_LOC_EFFECT_MAX_NUM    8
#define	DBC_EQUIP_LOC		(303)
struct _DBC_EQUIP_LOC
{
	INT		nID;		// 
	LPCSTR	szDesc;		// 
	LPCSTR	pMeshFile;	// 
	LPCSTR	pMatFile;	// 
	EQUIP_EFFECT	szREffect[EQUIP_LOC_EFFECT_MAX_NUM];	// 绑点和特效
};


/////////////////////////////////////////////////////////////////////
// 挂接类物品显示定义				(ITEM_02)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_LOCATOR				(302)
struct _DBC_ITEM_VISUAL_LOCATOR
{
	INT		nID;					// 

	LPCSTR	pObjFile_Right;			// 右手武器名
	LPCSTR	pMatName_Right;			// 右手武器材质名

	LPCSTR	pObjFile_Left;			// 左手武器名
	LPCSTR	pMatName_Left;			// 左手武器材质名

	LPCSTR	pObjShieldFile;			// 左手盾名
	LPCSTR	pMatName_Shield;		// 左手盾材质名

	INT		pWeaponType;			// 武器类型
	INT		nWeaponActor;			// 表示该武器是否有动作
	LPCSTR	pActorFile;				// 武器动作文件

	LPCSTR	pEffectName[30];		// 增加30列各种宝石的特效
};

// 怪武器
#define	DBC_ITEM_MONSTER_VISUAL_LOCATOR		(320)
struct _DBC_ITEM_MONSTER_VISUAL_LOCATOR
{
	INT				nID;								// id

	LPCSTR			pObjFile_Right;						// 右手武器名
	LPCSTR			pMatName_Right;						// 右手武器材质名
	EQUIP_EFFECT	szREffect[MONSTER_EFFECT_MAX_NUM];	// 右手武器特效

	LPCSTR			pObjFile_Left;						// 左手武器名
	LPCSTR			pMatName_Left;						// 左手武器材质名
	EQUIP_EFFECT	szLEffect[MONSTER_EFFECT_MAX_NUM];	// 左手武器特效

	LPCSTR			pObjShieldFile;						// 左手盾名
	LPCSTR			pMatName_Shield;					// 左手盾材质名
	EQUIP_EFFECT	szLSEffect[MONSTER_EFFECT_MAX_NUM];	// 左手盾材特效

	INT		pWeaponType;								// 武器类型
	INT		nWeaponActor;								// 表示该武器是否有动作
	LPCSTR	pActorFile;									// 武器动作文件
};





#define  EQUIP_EFFECT_ATTR_MAXNUM      16

// 装备对人物属性的影响
struct   EQUIP_EFFECT_ATTR
{
	INT nAttrID;     // 影响属性Id
	INT nEffectNum;  // 影响属性数值
};

//附加技能最大个数
#define  EQUIP_ADDEFFECT_SKILL_MAXNUM     4  
//影响技能最大个数
#define  EQUIP_EFFECT_SKILL_MAXNUM     4 


struct   EQUIP_EFFECT_SKILL
{

	INT nSkillID;     // 影响技能id
	INT nEffectLevel; // 影响技能等级
}; 

/////////////////////////////////////////////////////////////////////
//物品定义表_装备										
/////////////////////////////////////////////////////////////////////

#define  DBC_ITEM_EQUIP         (318)
struct  _DBC_ITEM_EQUIP
{
	INT		nID;			// 装备序列号
	INT		nClass;			// 基本类型class
	INT		nType;			// 类别 type
	INT		nIndex;			// 物品号
	INT		nEquipPoint;	// 装备点     
	INT		nRule;          // 适应规则
	INT		nSetID;         // 装备套装编号
	LPCSTR	nLevelRequire;  // 等级需求
	INT		nBasePrice;     // 基本价格
	INT		nBMaxDur;       // max耐久
	INT     nStarLeveL;     // 星级
	INT     nNatureHoleNum;	// 天然孔数目 
	INT     nMaxHoleNum;    // 最大孔数目
	INT     nProfessionalReq;// 职业要求
	INT		nPaixing;	    	// 装备牌型
	INT		nEquipScore;	 // 装备分数
	INT     nBaseAttackNear; // 基础近程攻击
	INT     nBaseAttackFar;  // 基础远程攻击
	INT     nBaseAttackMagic;// 基础魔法攻击
	INT     nBaseDEefNear ;   // 基础近程防御
    INT     nBaseDefFar;	 // 基础远程防御
    INT     nBaseDefMagic;   // 基础魔法防御

	// 装备对人物属性的影响   
	EQUIP_EFFECT_ATTR stEffectAttr[EQUIP_EFFECT_ATTR_MAXNUM];

	// 装备对技能的影响
	EQUIP_EFFECT_SKILL   stADDEffectSkill[EQUIP_ADDEFFECT_SKILL_MAXNUM];// 附加技能
	EQUIP_EFFECT_SKILL   stEffectSkill[EQUIP_EFFECT_SKILL_MAXNUM];  // 影响技能

	/*

		// 装备对人物属性的影响   
		INT   stEffectAttr[ EQUIP_EFFECT_ATTR_MAXNUM * 2] ;
		// 装备对技能的影响
		INT		ADDEffectSkill[EQUIP_ADDEFFECT_SKILL_MAXNUM * 2];  // 附加技能
		INT		EffectSkill[EQUIP_EFFECT_SKILL_MAXNUM * 2];     // 影响技能
	*/

};

/////////////////////////////////////////////////////////////////////
// 物品定义表_药瓶				(ITEM_06)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MEDIC			(306)
struct _DBC_ITEM_MEDIC		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //类型
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //名字
	LPCSTR	szDesc;   //描述
	INT		nLevel; //等级
	INT		nBasePrice;	//基本价格
	INT     nSalePrice;     //出售价格
	INT		nRule;		//适应规则
	INT		nPileCount; //叠放数量
	INT		nScriptID;	//脚本编号
	INT		nSkillID;	//技能编号
	INT		NOTCARE[2];	//需求技能等级
	INT		nMaxHold;	//最大持有数量
	INT		nTargetType;	// 针对对象类型 ENUM_ITEM_TARGET_TYPE
	INT     nBroadcast ;//是否广播
	LPCTSTR szTypeDesc;		// 物品的类型描述
	LPCTSTR     nDropModelId;         //掉落时的外形
	LPCTSTR		szDropEffect;			//掉落模型附属特效
	INT			szDropSound;            //掉落声音
	LPCTSTR		szDropVisColor;         //显示名称颜色

	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
// 物品定义表 		任务物品表		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_TASK			(323)
struct _DBC_ITEM_TASK		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //类型
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //名字
	LPCSTR	szDesc;   //描述
	INT		nLevelRequire; //等级
	INT		nBasePrice;	//基本价格
	INT     nSalePrice;     //出售价格
	INT		nRule;		//适应规则
	INT		nPileCount; //叠放数量
	INT		nScriptID;	//脚本编号
	INT		nSkillID;	//技能编号
	INT		NOTCARE[2];	//需求技能等级
	INT		nMaxHold;	//最大持有数量
	INT		nTargetType;	// 针对对象类型 ENUM_ITEM_TARGET_TYPE
	INT     nBroadcast ;//是否广播
	LPCTSTR szTypeDesc;		// 物品的类型描述

	//... NOT CARE
};


/////////////////////////////////////////////////////////////////////
// 物品定义表 		扩展背包物品表		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EXTRABAG		(324)
struct _DBC_ITEM_EXTRABAG		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //类型
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //名字
	LPCSTR	szDesc;   //描述
	INT		nRule;		//适应规则
	INT		nBasePrice;	//基本价格
	INT		nSalePrice;     //出售价格
	INT		nEffectTime;	//有效时间
	INT		nEffectGridNum;		//有效格子数
	LPCTSTR szTypeDesc;		// 物品的类型描述
	LPCTSTR     nDropModelId;         //掉落时的外形
	LPCTSTR		szDropEffect;			//掉落模型附属特效
	INT			szDropSound;            //掉落声音
	LPCTSTR		szDropVisColor;         //显示名称颜色

	//... NOT CARE
};


/////////////////////////////////////////////////////////////////////
// 物品定义表 		魂珠物品表		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SOULBEAD		(325)
struct _DBC_ITEM_SOULBEAD		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //类型
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //名字
	LPCSTR	szDesc;   //描述
	INT		nRule;		//适应规则
	INT		nEffectTime;	//有效时间
	INT		nImpactId;		//使用效果编号
	INT		nIsRadio;		//是否广播
	LPCTSTR szTypeDesc;		// 物品的类型描述
	INT		nSelectObjType;		//选择对象类型
	LPCTSTR     nDropModelId;         //掉落时的外形
	LPCTSTR		szDropEffect;			//掉落模型附属特效
	INT			szDropSound;            //掉落声音
	LPCTSTR		szDropVisColor;         //显示名称颜色

	//... NOT CARE
};



//宝石属性最大个数
#define  EQUIP_GEMATTR_MAXNUM     4  
//需求宝石最大数量
#define  EQUIP_GEMNEED_MAXNUM    3  

struct   EQUIP_GEM_ATTR
{

	INT nAttr;     // 强化属性
	INT nNum;    // 强化数值
}; 

struct   EQUIP_GEM_NEED
{

	INT nGem;     // 需求宝石
	INT nGemNum;    // 需求宝石NUM
}; 


/////////////////////////////////////////////////////////////////////
// 物品定义表_宝石				(ITEM_07)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_GEM			(307)
struct _DBC_ITEM_GEM	// ef
{
	INT		nID;
	INT		nClass; //物品类型
	INT		nType;   //物品种类
	INT		nGemIndex ;//物品编号
	LPCTSTR	szIcon;   //图标
	INT		nRule;  //适应规则
	LPCTSTR	szWhere; //镶嵌位置
	LPCTSTR	szName;
	LPCTSTR szDesc;
	INT		nPrice;			// 宝石基本的价格
	INT		nSalePrice;		// 宝石出售的价格
	INT     nBroadcast;		//是否广播
//	INT		nAttr[66];		// 宝石的附加属性	// 59

	EQUIP_GEM_ATTR  stGemAttr[EQUIP_GEMATTR_MAXNUM];// 宝石的附加属性
	EQUIP_GEM_NEED  stGemNeed[EQUIP_GEMNEED_MAXNUM]; //需求其他宝石的信息

	LPCTSTR	szColor;		// 附加属性的颜色
	LPCTSTR szTypeDesc;		// 类型描述
	LPCTSTR szEffectColor;		//特效颜色值
	INT		nEffectIndex;	//特效索引
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
// 物品定义表_地图				(ITEM_08)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_STOREMAP			(308)
struct _DBC_ITEM_STOREMAP		// ef
{
	INT		nID;
	INT		nClass;
//	INT		nQuality;//remove
	INT		nType;
	LPCSTR	szIcon;
	INT		nIndex;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nRule;
	//.......................
	//... NOT CARE
};
/////////////////////////////////////////////////////////////////////
// 套装组合后附加属性			(ITEM_15)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SETATT			(315)
struct _DBC_ITEM_SETATT
{
	INT		nID;
	INT		nAtt[66];	// 59
};

/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
/*
	/////////////////////////////////////////////////////////////////////
	// 普通装备外形表
	/////////////////////////////////////////////////////////////////////
	#define DBC_ITEM_COMMON_EQUIP_VISUAL    (316)
	struct _DBC_ITEM_COMMON_EQUIP_VISUAL
	{ 
		INT			nIndex;          // 编号 区分同一id的装备
		INT			nID;             // 例如 10010000   普通装备末4位为0000
		INT         level;           // client不使用
		INT			nModelID;        // 模型id，可以在模型表里找到对应的具体模型
		LPCTSTR		szName;          // 装备名称 例如 xx刀
		LPCTSTR		szDes;           // 说明 描述信息
		LPCTSTR		szIcon;          // 图标 ui显示用
		LPCTSTR		szType;          // 刀, 类型描述


		LPCTSTR         nDropModelId;         //掉落时的外形
	//	LPCTSTR		szDropEffect;			//掉落模型附属特效
		INT			szDropSound;            //掉落声音
		LPCTSTR		szDropVisColor;         //显示名称颜色


		//LPCTSTR     szEffect[EQUIP_EFFECT_MAX_NUM*2];//特效
		EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//特效

	};
*/
	#define DBC_ITEM__EQUIP_VISUAl_MAXNUM    99
	#define DBC_ITEM__EQUIP_GEM_MAXNUM    7
/*

	/////////////////////////////////////////////////////////////////////
	//高级装备外形表					
	/////////////////////////////////////////////////////////////////////

	#define DBC_ITEM_ADVANCED_EQUIP_VISUAL    (317)
	struct _DBC_ITEM_ADVANCED_EQUIP_VISUAL
	{ 
		INT			nIndex;          // 编号 对高级装备来说 没用的列
		INT			nID;             // 例如 10010001
		INT			nModelID;        // 模型id，可以在模型表里找到对应的具体模型
		LPCTSTR		szName;          // 装备名称 例如 xx刀
		LPCTSTR		szDes;           // 说明 描述信息
		LPCTSTR		szIcon;          // 图标 ui显示用
		LPCTSTR		szType;          // 刀

		LPCTSTR      nDropModelId;         //掉落时的外形
	//	LPCTSTR		szDropEffect;			//掉落模型附属特效
		INT			szDropSound;            //掉落声音
		LPCTSTR		szDropVisColor;         //显示名称颜色

		//	LPCTSTR     szEffect[EQUIP_EFFECT_MAX_NUM*2]; //特效
		EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//特效

	};


*/

/////////////////////////////////////////////////////////////////////
//高级装备外形表					
/////////////////////////////////////////////////////////////////////

#define DBC_ITEM_EQUIP_VISUAL    (316)
struct _DBC_ITEM_EQUIP_VISUAL
{ 

	INT			nID;             // 例如 10010001
	INT			nModelID;        // 模型id，可以在模型表里找到对应的具体模型
	LPCTSTR		szName;          // 装备名称 例如 xx刀
	LPCTSTR		szDes;           // 说明 描述信息
	LPCTSTR		szIcon;          // 图标 ui显示用
	LPCTSTR		szType;          // 刀

	LPCTSTR      nDropModelId;         //掉落时的外形
	LPCTSTR		szDropEffect;			//掉落模型附属特效
	INT			szDropSound;            //掉落声音
	LPCTSTR		szDropVisColor;         //显示名称颜色

	EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//特效

};

/////////////////////////////////////////////////////////////////////
//装备属性组表	〈 普通装备才会用到〉对应的装备表的普通装备的属性会填充-1	
/////////////////////////////////////////////////////////////////////
#define  DBC_ITEM_ATTR_GROUP		322    
struct  _DBC_ITEM_ATTR_GROUP
{	
	INT			nGroupId;		// 属性组Id
	LPCTSTR		szGroupName;    // 属性组名字
	INT			nProbability;   // 属性组几率

	INT         nAttrId[EQUIP_EFFECT_ATTR_MAXNUM]; // 影响属性id 共16项 对应到装备表的16项
};



#define    EQUIP_LEVEL_MAXNUM               100
/////////////////////////////////////////////////////////////////////
//装备属性等级表〈 普通装备才会用到〉					
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_ATTR_LEVEL			321
struct _DBC_ITEM_ATTR_LEVEL
{	
	INT			nAttrID; //属性id
	LPCSTR      szName; //属性名  

	INT         nLevel[EQUIP_LEVEL_MAXNUM]; //装备影响属性
	 
};


//装备玩法
////////////////////////////////////////////////////////////////////////////


//装备打孔
//Equip_HoleReqInfo.tab
#define  DBC_EQUIP_HOLE_INFO             330

struct	_DBC_EQUIP_HOLE_INFO
{
	
	INT  nHoleNum; //孔的数目
	INT	 nItemNeed; //需要的打孔道具
    INT  nItemNeedNum;//需要的道具数量
	INT  nMoneyNeed; //需要的金钱数量
	INT  nSuccessRatio;//成功率
//	LPCTSTR szName;//道具名称

};


//装备绑定
//Equip_bind.tab
#define  DBC_EQUIP_BINDING_INFO             331

struct	_DBC_EQUIP_BINDING_INFO
{
	INT nBindingLevel; //绑定级别
    INT nGemNeed;     //需求宝石
    INT nIntensifyInfo; //强化比例
	 
};


// 装备打星
//Equip_levelup.tab
#define  DBC_EQUIP_ADD_START             332

struct	_DBC_EQUIP_ADD_START
{			
	INT  nStarLevel;							//星级	
	INT	m_LowLevelGemID;
	INT	m_HighLevelGemID;
	INT	m_OtherLevelGemID;
	INT  nProOneGem;							//1颗宝石成功几率	
	INT  nProTwoGem;							//2颗宝石成功几率	
	INT  nProThrGem;							//3颗宝石成功几率	
	INT  nProFourGem;							//4颗宝石成功几率	
	INT  nPunishment;							//失败惩罚	
	INT  nNeedMoney;							//所需金钱	
	INT nIntensifyInfo;  						//强化比例

};


//装备特修表
//Equip_SpecialRepair.tab
#define  DBC_EQUIP_SP_REPAIR             333

struct	_DBC_EQUIP_SP_REPAIR
{			
	INT  nNeedItem;							//需求物品
	INT  nIntensifyInfo;  						//恢复耐久点

};

#define  MAX_EQUIP_PROMATL_NUM         4
struct	 _DBC_EQUIP_PRO_MAT
{


	INT		nMaterial;				//材料类
	INT		nMaterialNum;			//材料数量
	INT		nMatLowestLv;		//材料最低等1	
	INT		nMaterialLoss;			//拆解损耗比例4

};

#define  MAX_EQUIP_RWMATL_NUM        3       //改造材料数量

struct	 _DBC_EQUIP_REWORK_MAT
{
	INT		nReWorkMaterial;		//改造1材料类
	INT		nReWorkMatNum;			//改造1材料数量
	INT		nReWorkMatLowestLv;	    //改造1材料最低等级

};

#define  MAX_EQUIP_SPIRIT_RWMATL_NUM        3       //灵魂改造材料数量

#define   DBC_EQUIP_PRODUCT_INFO			334

struct	 _DBC_EQUIP_PRODUCT_INFO	 
{
	INT		nFormulaId;				//配方号
	INT		nProductId;				//产品ID					 	
	LPCSTR	szFormulaName;			//配方名称	
	INT		nProductNum;			//产品数量

	//INT		nMaterial1;				//材料类1
	//INT		nMaterial1Num;			//材料数量1
	//INT		nMat1LowestLv;		//材料最低等级1	
	//INT		nMaterial1Loss;			//拆解损耗比例1	

	//INT		nMaterial2;				//材料类2	
	//INT		nMaterial2Num;			//材料数量2	
	//INT		nMat2LowestLv;		//材料最低等级2	
	//INT		nMaterial2Loss;			//拆解损耗比例2
	//INT		nMaterial3;				//材料类3
	//INT		nMaterial3Num;			//材料数量3
	//INT		nMat3LowestLv;		//材料最低等级3
	//INT		nMaterial3Loss;			//拆解损耗比例3
	//INT		nMaterial4;				//材料类4	
	//INT		nMaterial4Num;			//材料数量4	
	//INT		nMat4LowestLv;		//材料最低等级4
	//INT		nMaterial4Loss;			//拆解损耗比例4
	_DBC_EQUIP_PRO_MAT  szMat[MAX_EQUIP_PROMATL_NUM];



	//INT		nReWorkMaterial1;		//改造1材料类
	//INT		nReWorkMat1Num;			//改造1材料数量
	//INT		nReWorkMat1LowestLv;	//改造1材料最低等级
	//INT		nReWorkMaterial2;		//改造2材料类
	//INT		nReWorkMat2Num;			//改造2材料数量
	//INT		nReWorkMat2LowestLv;	//改造2材料最低等级
	//INT		nReWorkMaterial3;		//改造3材料类
	//INT		nReWorkMat3Num;			//改造3材料数量
	//INT		nReWorkMat3LowestLv;	//改造3材料最低等级

   _DBC_EQUIP_REWORK_MAT szRwMat[MAX_EQUIP_RWMATL_NUM];

	INT		nOperateTime;			//操作时间
	INT		nScriptId;				//脚本ID

};

//MaterialLevel_info.tab


#define  DBC_EQUIP_MATERIAL_INFO			335

struct	 _DBC_EQUIP_MATERIAL_INFO	 
{

	INT		nMaterialGrade;	//材料等级	
	INT		nMaterialValues;//材料分值

};

///pai_info.tab
// 每张牌
#define  DBC_EQUIP_CARD_INFO			336

struct	 _DBC_EQUIP_CARD_INFO	 
{

	INT			nCardId;	//牌id	 对应于枚举  例如 EQUIPPAI_ANY 任意牌
	LPCTSTR		szCardName; //牌名
	INT			nAttrId;	//属性id
	INT			nAttrNum;   //属性值
	INT			nRata;		//概率

};

//chain_info.tab
 
//3张牌组

#define  DBC_EQUIP_CARD_GROUP_INFO			337

struct	 _DBC_EQUIP_CARD_GROUP_INFO	 
{

	INT			nGroupId;		//组id
	LPCTSTR		sznGroupName;	//组合名字
	INT			nNeedCardId1;  //需求牌型id
	INT			nNeedCardId2;  //需求牌型id
	INT			nNeedCardId3;  //需求牌型id
	INT			nAttrId;	   //强化属性
	INT			nAttrNum;	   //强化数值
	LPCTSTR		szEffect;	   //特效	
};

//soul_info.tab


//整套牌 用于激活

#define  EQUIP_CARD_ACTIVE_ATTR_NUM		3

struct   EQUIP_CARD_ATTR
{
	INT			nAttrId;	   //强化属性
	INT			nAttrNum;	   //强化数值

};


#define  DBC_EQUIP_CARD_ALL		338

struct	 _DBC_EQUIP_CARD_ALL	 
{

	INT			nGroupId;		//组id
	LPCTSTR		sznGroupName;	//组合名字
	INT			nNeedGropId1;  //需求组合id
	INT			nNeedGropId2;  //需求组合id
	INT			nNeedGropId3;  //需求组合id
	INT			nNeedGropId4;  //需求组合id
	INT			nNeedGropId5;  //需求组合id

	EQUIP_CARD_ATTR stAttr[EQUIP_CARD_ACTIVE_ATTR_NUM];//强化属性
	LPCTSTR		szEffect;	   //特效	
};



//换牌表格
 //ChangePai_info.tab
#define  DBC_EQUIP_CARD_TARGET    339

struct  _DBC_EQUIP_CARD_TARGET

{
	INT   nId;     //道具i
	LPCTSTR szName; //说明
	INT   nTargetId; //目标id


};



//equipset_attr.tab


//装备套装表

#define  DBC_EQUIP_SET_INFO			340
#define		EQUIP_SET_ATTR_NUM		4

struct   EQUIP_SET_ATTR

{
   INT   nAttrId;  //属性id
   INT   nAttrNum; //属性数值
   INT	 nAttrActiveNum; //激活属性需要的套装数目

};

struct  _DBC_EQUIP_SET_INFO

{
	INT   nSetId;     //套装id
	LPCTSTR szSetName; //套装名字
	INT   nSetNum;    //套装件数
	LPCTSTR  szId;    //套装包含的系列id
	EQUIP_SET_ATTR  stAttr[EQUIP_SET_ATTR_NUM]; //套装属性

};

////////////////////////////////////////////////////////////////////////////////




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								声音相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// 声音文件定义						(SOUND_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SOUND_INFO				(401)
struct _DBC_SOUND_INFO
{
	INT		nID;
	LPCSTR	pSoundFile;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								特效相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// 特效定义							(EFFECT_01)
/////////////////////////////////////////////////////////////////////
#define DBC_EFFECT_DEFINE			(501)
struct _DBC_EFFECT_DEFINE		// bc
{
	INT		nID;
	LPCSTR	pEffectType;
	LPCSTR	pParam1;
	LPCSTR	pParam2;
	LPCSTR	pParam3;
	LPCSTR	pParam4;
	LPCSTR	pParam5;
	LPCSTR	pParam6;
};



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								技能相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// 心法定义表						(SKILL_01)
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_XINFA				(601)
//struct _DBC_SKILL_XINFA		// bc
//{
//	INT		nID;
//	LPCSTR	pszName;
//	INT		nMenpaiID;
//	LPCSTR	pszDesc;
//	LPCSTR	pszIconName;
//};

/////////////////////////////////////////////////////////////////////
// 附加效果							(SKILL_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_ADDIMPACT			(602)
struct _DBC_SKILL_ADDIMPACT		// bc
{
	INT		nID;
	INT		nParamNum;
	LPCSTR	pDesc;
	INT		nAddAttributeNum;
	LPCSTR	pAddAttribute1;
	LPCSTR	pAddAttribute2;
	LPCSTR	pAddAttribute3;
	LPCSTR	pAddAttribute4;
	INT		nSpliceID;
	LPCSTR	pIcon;
};


//
///////////////////////////////////////////////////////////////////////
// 子弹效果定义						(SKILL_03)
///////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_BULLET			(603)
//struct _DBC_SKILL_BULLET
//{
//	INT		nID;							//子弹类型
//	INT		nType;							/*
//											|	0 - 瞬间到达
//											|	1 - 移动到达
//											*/
//	INT		nEffect;						//特效ID
//	FLOAT	fSpeed;							//移动速度	(m/s)
//	INT		nHitEffect;						//击中特效
//	INT		nHitEffectLiveTime;				//击中特效维持时间
//};

/////////////////////////////////////////////////////////////////////
// BUFF附加效果						(BUFF_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_BUFF_IMPACT					(604)
//_DBC_BUFF_IMPACT

/////////////////////////////////////////////////////////////////////
// DIRECTLY附加效果						(DIRECTLY_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_DIRECTLY_IMPACT				(605)
//_DBC_DIRECTLY_IMPACT

/////////////////////////////////////////////////////////////////////
// 子弹						(BULLET)
/////////////////////////////////////////////////////////////////////
#define DBC_BULLET_DATA					(606)
//_DBC_BULLET_DATA

/////////////////////////////////////////////////////////////////////
// 技能						(SKILL)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DATA					(607)
//_DBC_SKILL_DATA

/////////////////////////////////////////////////////////////////////
// 技能						(SKILL_DEPLETE)
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_DEPLETE					(608)
//struct _DBC_SKILL_DEPLETE	// bc
//{
//	INT			m_nID;
//	INT			m_nMP;
//	INT			m_nHP;
//	INT			m_nSP;
//	INT			m_nStrikePoint;
//	INT			m_nItemID;
//};
// $$
/////////////////////////////////////////////////////////////////////
// 任务						(MISSION)
/////////////////////////////////////////////////////////////////////
//#define DBC_MISSION_DATA				(609)
//_DBC_MISSION_DATA

/////////////////////////////////////////////////////////////////////
// 法术OBJ数据						(SPECIAL_OBJ_DATA)
/////////////////////////////////////////////////////////////////////
#define DBC_SPECIAL_OBJ_DATA			(610)
struct _DBC_SPECIAL_OBJ_DATA		// cd
{
	INT			m_nID;					// ID
	LPCSTR		m_lpszName;				// 名称
	INT			m_nType;				// 类别（服务器专用）
	LPCSTR		m_lpszToolTips;			// ToolTips
	INT			m_nLogicID;				// 逻辑ID（服务器专用）
	INT			m_nStealthLevel;		// 隐形级别（服务器专用，陷阱专用）
	INT			m_nTrapFlags;			// 陷阱标记位集合（服务器专用，陷阱专用）
	INT			m_nActiveTimes;			// 可以激发的次数（服务器专用）
	LPCSTR		m_lpszEffect_Normal;	// 生存期持续特效
	LPCSTR		m_lpszSound_Normal;		// 生存期持续音效
	LPCSTR		m_lpszEffect_Active;	// 激发特效
	LPCSTR		m_lpszSound_Active;		// 激发音效
	LPCSTR		m_lpszEffect_Die;		// 死亡特效
	LPCSTR		m_lpszSound_Die;		// 死亡音效
	INT			m_nReserve1;			// 预留
	INT			m_nReserve2;			// 预留
	INT			m_nReserve3;			// 预留
	INT			m_nBulletID;			// 子弹ID
	INT			m_nDuration;			// 持续时间（服务器专用）
	INT			m_nInterval;			// 激活或触发时间间隔（服务器专用）
	FLOAT		m_nTriggerRadius;		// 触发半径（服务器专用，陷阱专用）
	FLOAT		m_nEffectRadius;		// 影响半径（服务器专用，陷阱专用）
	INT			m_nEffectUnitNum;		// 影响个体数目（服务器专用，陷阱专用）
	INT			m_nReserve4;			// 预留
	INT			m_nReserve5;			// 预留
	INT			m_nReserve6;			// 预留
	INT			m_nReserve7;			// 预留
	INT			m_nReserve8;			// 预留
	LPCSTR		m_lpszParamDesc0;		// 参数说明（服务器专用）
	INT			m_nParam0;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc1;		// 参数说明（服务器专用）
	INT			m_nParam1;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc2;		// 参数说明（服务器专用）
	INT			m_nParam2;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc3;		// 参数说明（服务器专用）
	INT			m_nParam3;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc4;		// 参数说明（服务器专用）
	INT			m_nParam4;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc5;		// 参数说明（服务器专用）
	INT			m_nParam5;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc6;		// 参数说明（服务器专用）
	INT			m_nParam6;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc7;		// 参数说明（服务器专用）
	INT			m_nParam7;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc8;		// 参数说明（服务器专用）
	INT			m_nParam8;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc9;		// 参数说明（服务器专用）
	INT			m_nParam9;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc10;		// 参数说明（服务器专用）
	INT			m_nParam10;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc11;		// 参数说明（服务器专用）
	INT			m_nParam11;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc12;		// 参数说明（服务器专用）
	INT			m_nParam12;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc13;		// 参数说明（服务器专用）
	INT			m_nParam13;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc14;		// 参数说明（服务器专用）
	INT			m_nParam14;				// 参数值（服务器专用）
	LPCSTR		m_lpszParamDesc15;		// 参数说明（服务器专用）
	INT			m_nParam15;				// 参数值（服务器专用）
	LPCSTR		m_lpszInfo;				// 详细说明(内部使用)
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								场景相关相关							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// 场景定义							(SCENE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_DEFINE			(701)
struct _DBC_SCENE_DEFINE	// cd
{
	static const int SCENE_SERVER_ID_COLUMN = 1;

	INT		nLocalID;
	INT		nServerID;
	LPCSTR	szName;
	INT		nCityLevel;
	INT		nXSize;
	INT		nZSize;
	LPCSTR	szSceneName;
	LPCSTR	szPOVFile;
	LPCSTR	szRegionFile;
	LPCSTR  szCollisionfile;				// 建筑物行走面文件。
	LPCSTR	szMiniMap;
	INT		nBackSound;
	INT		SilentTime;						//
	LPCSTR  szSceneMap;

	INT		nWroldMapPosX;
	INT		nWroldMapPosY;					// 场景图标所在得位置
	INT		nNameWroldMapPosX;
	INT		nNameWroldMapPosY;				// 场景名字所在得位置
	LPCSTR	szSceneType;					// 场景得图标等级
	LPCSTR	szCityNameNormalImageSet;		// 场景正常名字所用得图片资源
	LPCSTR	szCityNameNormalImage;			// 场景正常名字所用得图片资源
	LPCSTR	szCityNameHoverImageSet;		// 场景高亮名字所用得图片资源
	LPCSTR	szCityNameHoverImage;			// 场景高亮名字所用得图片资源
	LPCSTR	SceneInfo;						//		
	LPCSTR	Blur_And_Shine_Amount;			//

	FLOAT	nCameraAngle;					// 场景摄像机偏移量 

};

/////////////////////////////////////////////////////////////////////
// 场景建筑物定义					(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_BUILDING_DEINFE			(702)
struct _DBC_BUILDING_DEFINE		// bc
{
	static const int MAX_LEVEL = 5;

	INT		nID;				// 资源id
	LPCSTR	szResType;			// 资源类型
	LPCSTR	szLevel[MAX_LEVEL];	// 等级1-5资源
};

/////////////////////////////////////////////////////////////////////
// 城市建筑物定义					(SCENE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CITY_BUILDING			(703)
struct _DBC_CITY_BUILDING		// de
{
	INT		nID;					// id
	INT		nCityID;				// 城市id(DBC_CITY_DEINFE)
	INT		nBuildingID;			// 建筑物id(DBC_BUILDING_DEINFE)
	LPCSTR	szGfxPosition;			// 位置(gfx)
	INT		nInitLevel;				// 初始等级
	LPCSTR	szGfxOrientation;		// 旋转四元数(gfx)
};

/////////////////////////////////////////////////////////////////////
// 可跳转的地图信息定义							(SCENE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_WORLDPOS_DEFINE			(704)
struct _DBC_WORLDPOS_DEFINE
{
	INT		nID;				// id
	INT		nSceneID;			// 场景的id
	LPCSTR	szSceneName;		// 场景的名字
	INT		nXPos;				// 传送点的位置
	INT		nYPos;				// 传送点的位置
	INT		nSceneType;			// 场景类型
	INT		nMapLevel;			// 地图等级
	INT		nEnterLevel;		// 进入等级
	LPCSTR	szPinyinIndex;		// 拼音索引
};

/////////////////////////////////////////////////////////////////////
// 场景地图上的任务NPC信息定义							(SCENE_05)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENENPC_DENFINE		(705)
struct _DBC_SCENENPC_DENFINE
{
	INT		nID;				// id
	INT		nSceneID;			// 场景的id
	LPCSTR	szNpcName;			// NPC的名字
	LPCSTR	szNpcUIName;		// NPC头顶显示的名字
	INT		nXPos;				// X坐标
	INT		nYPos;				// Y坐标
	INT		nIsTalkNpc;			// 是否讲话
	INT		nNpcType;			// NPC类型
	LPCSTR	szNpcTypeName;		// NPC类型名
	LPCSTR	szPinyinIndex;		// 拼音索引
};


/////////////////////////////////////////////////////////////////////
// 场景传送点定义							(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_POS_DEFINE			(917)
struct _DBC_SCENE_POS_DEFINE
{
	INT		nID;				// id
	INT		nSceneID;			// 场景的id
	LPCSTR	szSceneName;		// 场景的名字
	INT		nXPos;				// 传送点的位置
	INT		nZPos;				// 传送点的位置
	INT		nDestSceneID;			// 传送到哪个场景
	LPCSTR	szDestSceneName;			// 传送到哪个场景
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								UI相关									//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// UI组定义						(UI_01)
/////////////////////////////////////////////////////////////////////
#define DBC_UI_LAYOUTDEFINE		(801)
struct _DBC_UI_LAYOUTDEFINE	// de
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szLayout;
	INT		nDemise;
	LPCSTR	szScript;
	INT		nOpenSoundID;
	INT		nCloseSoundID;
	LPCSTR szDefaultPosition;
	LPCSTR	szHelp;
	LPCSTR	szDes;
};



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								生活技能相关							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// 资源生长点							(LIFE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_GROWPOINT		(901)
struct _DBC_LIFEABILITY_GROWPOINT	// cd
{
	INT		nID;
	LPCSTR	szName;
	INT		nLifeAbility;
	LPCSTR	szMeshFile;
	LPCSTR	szTips;
	INT		nTimeNeed;
	LPCSTR	szNotcare[4];
	INT		nAnimOpen;
};

/////////////////////////////////////////////////////////////////////
// 生活技能								(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_DEFINE	(902)
struct _DBC_LIFEABILITY_DEFINE		// bc
{
	INT		nID;						//ID
	LPCSTR	szName;				//名称
	INT		nLevelNeed;			//所需等级
	INT		nLevelMax;			//最高等级
	LPCSTR	szLevelupTable;		//升级需求和消耗表
	INT		nTimeOperation;	//操作时间基数
	INT		nToolNeed;			//工具ID
	FLOAT	fPlatformDist;			//辅助平台的有效距离
	INT		nPlatformID;			//辅助平台ID
	INT		nAnimOperation;	//操作动作ID
	LPCSTR	szIconName;			//图标
	INT		nItemVisualLocator; // 动作挂接
	LPCSTR	szExplain;			// 详细解释
	INT		nPopup;				// 是否弹出下级界面
	LPCSTR	szProgressbarName;	// 进度条名称
	INT		nconsume;				//消耗活力还是精力
	INT		nBeginSound;		//开始时播放的声音
	INT		nIsCycle;				//是否循环
	INT		nSuccedSound;		//成功时播放的声音
	INT		nFailedSound;		//失败时播放的声音
};


/////////////////////////////////////////////////////////////////////
// 生活技能	升级信息							(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CAIKUANG_LEVELINFO		(811)			//采矿
#define DBC_SHOUGE_LEVELINFO		(812)			//收割
#define DBC_BUZHUO_LEVELINFO		(813)			//捕捉
#define DBC_ZHUZAO_LEVELINFO		(814)			//铸造
#define DBC_ZHIYI_LEVELINFO		(815)			//制衣
#define DBC_GONGJIANG_LEVELINFO		(816)			//工匠
#define DBC_PENGREN_LEVELINFO		(817)			//烹饪
struct _DBC_LIFEABILITY_LEVELINFO
{
	INT		nLevel;					//级别
	LPCSTR	szName;				//名称
	INT		nMoneyNeed;		//需求金钱
	INT		nExpNeed;				//需求经验
	INT		nSkillPro;				//技能熟练度限制
	INT		nSkillProClient;		//客户端显示技能熟练度限制
	INT		nSkillMaxProLevel;	//本级熟练度上限
	INT		nLevelNeed;			//等级限制
	LPCSTR	szNPCName;			//NPC名称
	INT		nNpcMoneyNeed;	//NPC需求金钱
	INT		nNpcExpNeed;		//NPC需求经验
};


/////////////////////////////////////////////////////////////////////
// 生活技能	 物品合成							(LIFE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_ITEMCOMPOSE		(903)
struct _DBC_LIFEABILITY_ITEMCOMPOSE		// bc
{
	INT		nID;							//配方ID
	LPCSTR	szName;					//名称
	INT		nResultID;					//产品ID
	INT		nResultNum;				//产品数量	
	INT		nLifeAbility;				//所需生活技能ID
	INT		nLifeAbility_Level;		//所需生活技能等级
	INT		nStuff1ID;					//所需材料1ID
	INT		nStuff1num;				//所需材料1数量
	INT		nStuff2ID;					
	INT		nStuff2num;
	INT		nStuff3ID;
	INT		nStuff3num;
	INT		nStuff4ID;
	INT		nStuff4num;
	INT		nStuff5ID;
	INT		nStuff5num;
	INT		nAttribute_ID1;		//活力需求
	INT		nAttribute_Level1;	//精力需求
	INT		nAttribute_ID2;		//门派贡献度需求
	INT		nAttribute_Level2;	//
	INT		nCoolDown_Time;	//冷却时间
	INT		nFacility;				// 需要的工具	
	INT		nCoolDoon_Troop;		// 冷却组
	INT		nProficiencyRequire;	// 熟练度需求
	INT		nProficiencyIncrease;	// 熟练度增加
	INT		NOTCARE;				// 不关心的1列
	INT		nProficiencyTime;		// 配方时间
	INT		nScriptID;				// 脚本ID
	INT		nFailItem;				// 失败后的替代品
	LPCSTR	szExplain;				// 详细描述


	// 功能分组

	//NOT CARE...
};

/////////////////////////////////////////////////////////////////////
// 生活技能 操作平台								(LIFE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_PLATFORM		(904)
struct _DBC_LIFEABILITY_PLATFORM	// bc
{
	INT		nID;
	LPCSTR	szName;
	INT		nType;
	LPCSTR	szMeshFile;
	INT		nClickType;
};
/////////////////////////////////////////////////////////////////////
// 表情和颜色转换字符
/////////////////////////////////////////////////////////////////////
#define DBC_CODE_CONVERT			( 905 )
struct _DBC_CODE_CONVERT	// bc
{
	INT		nID;		// ID
	LPCSTR  szCode;		// 字符串形式的十六进制数字
	LPCSTR  szName;		// 介绍
};

/////////////////////////////////////////////////////////////////////
// 心法技能								(XINFA_02)
/////////////////////////////////////////////////////////////////////
//#define DBC_XINFA_XINFADEFINE	(902)
//struct _DBC_XINFA_XINFADEFINE
//{
//	INT		nID;			// 心法ID
//	INT		nIDMenpai;		// 门派ID
//	LPCSTR	szName;			// 心法名称
//	LPCSTR	szTips;			// 心法描述
//	LPCSTR	szIconName;		// 图标名称
//};
/////////////////////////////////////////////////////////////////////
// 心法升级花费						(XINFA_02)
/////////////////////////////////////////////////////////////////////
struct Spend
{
	INT		dwSpendMoney;			// 需要花费的金钱数
	INT		dwSpendExperience;		// 需要花费的经验值
};
//#define DBC_XINFA_STUDYSPEND	(907)
//struct _DBC_XINFA_STUDYSPEND
//{
//	INT		nStudyLevel;			// 要学习的等级
//	Spend	StudySpend[54];
//};

/////////////////////////////////////////////////////////////////////
// 称号列表						(TITLE)
/////////////////////////////////////////////////////////////////////
/*
#define DBC_TITLE_NAME			(908)
struct _DBC_TITLE_NAME		// bc
{
	INT		nTitleIndex;			// 称号的索引
	LPCSTR	szTitle;				// 称号的名字
	UINT	nReserve;				// 类型保留
	LPCSTR	szTitleDesc;			// 称号的描述
};
*/

#define DBC_TITLE				(908)		// title_info.tab
struct _DBC_TITLE
{
	INT			nId;				// 称号ID
	INT			nType;				// TYPE
	LPCSTR		szColor;			// 显示颜色
	INT			bShow;				// 默认是否显示
	LPCSTR		szManTitle;			// 男性称号
	LPCSTR		szWomanTitle;		// 女性称号
	LPCSTR		szTitleDesc;		// 称号描述
	INT			nTimeLimit;			// 称号时限
	LPCSTR		szAcquireDesc;		// 获取途径
	INT			nEffectId;			// 效果ID
	LPCSTR		szReclaimCause;		// 回收原因
	INT			nCombinationId;		// 组合称号ID
};


/////////////////////////////////////////////////////////////////////
// 阵营数据	
/////////////////////////////////////////////////////////////////////
#define DBC_CAMP_DATA	(909)

/////////////////////////////////////////////////////////////////////
// 技能	skillData_v1.txt(读取表)					
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILLDATA_V1_DEPLETE					(910)
//struct _DBC_SKILLDATA_V1_DATA_PAIR		
//{
//	INT		nData;
//	LPCSTR  szDesc;
//	
//};
//struct _DBC_SKILLDATA_V1_DEPLETE	// cd
//{
//
//	INT		nId;				//  ID	  
//	INT		nCeHuaId;			//	策划专用ID	
//	INT		nSkillLogicid;		//	技能逻辑ID
//	LPCSTR	szEffectDesc;		//	效果的简要逻辑说明	
//	INT		nCooldownTime;		//	冷却时间	
//	INT		nJuqiTime;			//	聚气时间(ms)	
//	INT		nDirectTime;		//	引导时间(ms)	
//	LPCSTR  szCondition1Desc;	//	条件或消耗参数说明
//	INT		nCondition11;		//	条件或消耗参数值	
//	INT	    nCondition12;		//	条件或消耗参数值	
//	LPCSTR  szCondition2Desc;	//	条件或消耗参数说明
//	INT		nCondition21;		//	条件或消耗参数值	
//	INT		nCondition22;		//	条件或消耗参数值	
//	LPCSTR  szCondition3Desc;	//	条件或消耗参数说明
//	INT		nCondition31;		//	条件或消耗参数值	
//	INT		nCondition32;		//	条件或消耗参数值	
//	LPCSTR	szCondition4Desc;	//	条件或消耗参数说明
//	INT		nCondition41;		//	条件或消耗参数值	
//	INT		nCondition42;		//	条件或消耗参数值	
//	LPCSTR  szCondition5Desc;	//	条件或消耗参数说明
//	INT		nCondition51;		//	条件或消耗参数值	
//	INT		nCondition52;		//	条件或消耗参数值	
//	LPCSTR  szCondition6Desc;	//	参数说明
//	    
//	// 参数对
//	_DBC_SKILLDATA_V1_DATA_PAIR	paraPair[12];
//
//	LPCSTR Skill_Desc_Interface;
//
//};//$$

/////////////////////////////////////////////////////////////////////
// 任务种类						(MissionKind)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_KIND			(911)
struct _DBC_MISSION_KIND
{
	INT		nIndex;					// 任务种类的索引
	LPCSTR	szKindName;				// 任务种类的名字
};

// 任务NPC信息
#define DBC_MISSION_INFO		(930)
struct _DBC_MISSION_INFO
{
	INT		nIndex;
	LPCSTR	nNpcName;
	LPCSTR	szDefault;
	INT		nScript;
	LPCSTR	szEventList;
	LPCSTR	szEventList1;
	LPCSTR	szEventList2;
	LPCSTR	szEventList3;
	LPCSTR	szEventList4;
};


/////////////////////////////////////////////////////////////////////
// 表情和颜色转换字符
/////////////////////////////////////////////////////////////////////
#define DBC_STRING_DICT			( 912 )
struct _DBC_STRING_DICT		// bc
{
	INT		nID;			// ID
	LPCSTR  szString;		// 字符串
	LPCSTR  szKey;			// 关键字
};


/////////////////////////////////////////////////////////////////////
// 环境音效列表
/////////////////////////////////////////////////////////////////////
#define DBC_ENV_SOUND			( 913 )
struct _DBC_ENV_SOUND
{
	INT		nID;			// ID
	UINT	nSoundID;		// 音效ID
	UINT	nXPos;			// 声源的X坐标
	UINT	nZPos;			// 声源的Z坐标
	UINT	nDistance;		// 能听到声音的最大距离
	INT		n1;				// 连播次数
	INT		n2;				// 间隔时间(ms)
	INT		n3;				// 下一次连播间隔时间(ms)
};

/////////////////////////////////////////////////////////////////////
// 物品规则表
/////////////////////////////////////////////////////////////////////
#define DBC_ITEM_RULE			( 914 )
struct _DBC_ITEM_RULE		// bc
{
	INT		nID;			// ID
	LPCSTR  szName;         //规则名字 程序不适用 策划用
	INT		bCanOverLay;	// 是否可重叠
	INT		bCanDrop;		// 是否可丢弃
	INT		bCanQuick;		// 是否可放入快捷栏
	INT		bCanSale;		// 是否可以出售给NPC商店
	INT		bCanExchange;	// 是否可以交易
	INT		bCanUse;		// 是否可以使用
	INT		bPickBind;		// 是否是拾取邦定
	INT		bEquipBind;		// 是否是装备邦定
	INT		bUnique;		// 是否是唯一 
	INT     bCheck;			//是否需要鉴定
	INT     bUnRealItem;    //是否是虚拟道具
	INT     bAccessInBack;	//是否可存入银行
	INT     bWearOut;		//是否消耗	     		
};

/////////////////////////////////////////////////////////////////////
// 过滤词词汇表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_FILTER			( 915 )
struct _DBC_TALK_FILTER
{
	INT		nID;				// ID
	LPCSTR	szString;			// 过滤词汇（即：不能说的词汇）

	// 过滤规则
	// 0表示忽略，1表示含有就过滤，2表示完全匹配过滤
	INT		chatFilter;			// 聊天时规则
	INT		roleFilter;			// 创建人物时规则
	INT		nameFilter;			// 命名时规则
	INT		stallFilter;		// 摆摊时规则
};

/////////////////////////////////////////////////////////////////////
// 等级金钱对应表
/////////////////////////////////////////////////////////////////////
#define DBC_LV_MAXMONEY			( 916 )
struct _DBC_LV_MAXMONEY		// bc
{
	INT		nID;			// ID
	INT		nMaxMoney;		// 最大金钱
	INT		nLv;			// 等级
};

/////////////////////////////////////////////////////////////////////
// 聊天动作命令表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_ACT			( 918 )
struct _DBC_TALK_ACT	// ef
{
	INT		nID;			// ID
	LPCSTR	szCmd;			// 命令
	LPCSTR	szNobody;		// 没有目标时的字串
	LPCSTR	szMyself;		// 目标自己时的字串
	INT		n0;				// 维持状态标
	LPCSTR	szOther;		// 目标是其他玩家时的字串
	LPCSTR	szAct;			// （起始）收到聊天信息时需要做的动作命令串
	LPCSTR	szActPersist;	// （持续）
	LPCSTR	szActEnd;		// （结束）

	LPCSTR	szIconName;		//图标
	LPCSTR	szToolTip;		//提示信息
};

/////////////////////////////////////////////////////////////////////
// 聊天限制配置表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_CONFIG			( 919 )
struct _DBC_TALK_CONFIG		// cd
{
	INT		nID;				// ID 依据ENUM_CHAT_TYPE
	LPCSTR	szChannelName;		// 频道名称
	INT		nTimeSlice;			// 时间间隔（单位：秒）
	LPCSTR	szChannelHeader;	// 频道前导字符
	INT		nNeedType1;			// 消耗类型1 依据CHAT_NEED_TYPE
	INT		nNeedValue1;		// 消耗数值1
	INT		nNeedType2;			// 消耗类型2 依据CHAT_NEED_TYPE
	INT		nNeedValue2;		// 消耗数值2
	INT		nNeedType3;			// 消耗类型3 依据CHAT_NEED_TYPE
	INT		nNeedValue3;		// 消耗数值3
	INT		nNeedLv;			// 等级限制，这里标称的是最低多少级才能在此频道发言
};

/////////////////////////////////////////////////////////////////////
// 发型改变消耗表
/////////////////////////////////////////////////////////////////////
#define DBC_HAIR_STYLE			( 920 )
struct _DBC_HAIR_STYLE	// bc
{
	INT		nID;			// ID索引
	INT		nItemTableID;	// 消耗物品ID
	INT		nRaceID;		// 性别区分
	INT		nItemCount;		// 消耗物品数量
};

/////////////////////////////////////////////////////////////////////
// 怪物头顶泡泡表
/////////////////////////////////////////////////////////////////////
#define DBC_MONSTER_PAOPAO		( 921 )
struct _DBC_MONSTER_PAOPAO
{
	INT		nID;			// ID索引
	LPCSTR	szPaoPaoTxt;	// 怪物要说的头顶文字
};


#define DBC_CAMP_AND_STAND		(922)	//阵营


/////////////////////////////////////////////////////////////////////
// 字体信息颜色表
/////////////////////////////////////////////////////////////////////
#define DBC_SYSTEM_COLOR		(923)
struct _DBC_SYSTEM_COLOR	// bc
{
	const static INT NAME_PLAYER_MYSELF		 = 1;	// 名字_主角
	const static INT NAME_PLAYER_OTHER		 = 2;	// 名字_其他玩家
	const static INT NAME_NPC_PEACE			 = 3;	// 名字_和平NPC
	const static INT NAME_NPC_ATTACK_ACTIVE	 = 4;	// 名字_主动攻击NPC
	const static INT NAME_NPC_ATTACK_PASS	 = 5;	// 名字_不主动攻击NPC
	const static INT NAME_NPC_CANNT_ATTACK	 = 6;	// 不可攻击的NPC
	const static INT NAME_PET				 = 7;	// 宠物名字
	const static INT NANE_SELF_PET			 = 8;	// 自己的宠物

	const static INT TITLE_NORMAL			 = 100;	// 称号_普通
	const static INT TITLE_BANGPAI			 = 101;	// 称号_帮派职称
	const static INT TITLE_MOOD				 = 102;	// 称号_玩家自定义心情
	const static INT TITLE_PLAYER_SHOP		 = 103;	// 称号_玩家商店

	const static INT UI_DEBUGLISTBOX		 = 1000; // debuglistbox的颜色

	// 20100513 AddCodeBegin
	// 根据与主动攻击怪物级别差，显示的名字颜色
	// 级别差=对方级别-玩家级别
	const static INT NAME_DIFF_1			 = 201;	// 级别差>=20    (深红)
	const static INT NAME_DIFF_2			 = 202;	// 级别差[5,20)  (红)
	const static INT NAME_DIFF_3			 = 203;	// 级别差(-5,5)  (黄)
	const static INT NAME_DIFF_4			 = 204;	// 级别差(-20,-5](绿)
	const static INT NAME_DIFF_5			 = 205;	// 级别差<= -20  (灰)
	// 20100513 AddCodeEnd

	INT		m_nID;
	LPCSTR	strFontColor;
	LPCSTR  strDesc;
	LPCSTR	strExtenColor;
};

/////////////////////////////////////////////////////////////////////
// 完全匹配过滤表，用在创建帮会、起名字之类的地方，避免使用系统词组
/////////////////////////////////////////////////////////////////////
#define DBC_FULLCMP_FILTER		( 924 )
struct _DBC_FULLCMP_FILTER	// bc
{
	INT		nID;			// ID索引
	LPCSTR	szFullTxt;		// 完全屏蔽词
	LPCSTR	szFilterType;	// 用在什么地方，例如：all,guild,team,.....表示用在所有地方，仅帮会，仅组队等等。
};

/////////////////////////////////////////////////////////////////////
// 宠物升级需要的经验表
/////////////////////////////////////////////////////////////////////
#define DBC_PET_LEVELUP			( 925 )
struct _DBC_PET_LEVELUP
{
	INT		nID;			// ID索引
	INT		nExp;			// 所需要经验
};

/////////////////////////////////////////////////////////////////////
// 技能学习条件表
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_STUDY_DATA	(926)

/////////////////////////////////////////////////////////////////////
// 骑宠物品表
/////////////////////////////////////////////////////////////////////
#define DBC_MOUNT_ITEM_DATA		(927)
struct _DBC_MOUNT_ITEM_DATA
{
	UINT			nItemID;		// 物品ID
	const CHAR	*	szItemDec;		// 物品说明,策划用
	INT				nType;			// 物品类型
	INT				nRate;			// 属性比例
	INT				nNum;			// 属性值
	INT				nSkillID;		// 技能ID
	INT				nSucceedRate;	// 成功率

};

/////////////////////////////////////////////////////////////////////
// 坐骑基础表
/////////////////////////////////////////////////////////////////////
#define DBC_MOUNT_BASE_DATA		(928)
struct _DBC_MOUNT_BASE_DATA
{
	INT				nMountID;			// 骑乘的ID
	const CHAR	*	szMountName;		// 骑乘名称
	INT				nLevel;				// 骑乘等级
	INT				nRequirementLevel;	// 骑乘携带等级
	const CHAR	*	szChildID;			// 还童ID
	const CHAR	*	szMatingID;			// 交配ID
	INT				bMating;			// 是否可以交配
	INT				bAberrance;			// 是否变异
	INT				nDaiShu;			// 繁殖代数
	INT				nAttach;			// 骑乘的归属
	INT				nSurvivalTime;		// 无主骑乘存活时间
	INT				nMinStr;			// 初始最小力量
	INT				nMaxStr;			// 初始最大力量
	INT				nMinDex;			// 初始最小敏捷
	INT				nMaxDex;			// 初始最大敏捷
	INT				nMinInt;			// 初始最小智力
	INT				nMaxInt;			// 初始最大智力
	INT				nMinCon;			// 初始最小体质
	INT				nMaxCon;			// 初始最大体质
	INT				nMinStrApt;			// 初始最小力量资质
	INT				nMaxStrApt;			// 初始最大力量资质
	INT				nMinDexApt;			// 初始最小敏捷资质
	INT				nMaxDexApt;			// 初始最大敏捷资质
	INT				nMinIntApt;			// 初始最小智力资质
	INT				nMaxIntApt;			// 初始最大智力资质
	INT				nMinConApt;			// 初始最小体质资质
	INT				nMaxConApt;			// 初始最大体质资质
	INT				nMinSavvy;			// 初始最小悟性
	INT				nMaxSavvy;			// 初始最大悟性
	INT				nMinBasic;			// 初始最小根骨
	INT				nMaxBasic;			// 初始最大根骨
	INT				nMinGrowRate;		// 初始最小成长率
	INT				nMaxGrowRate;		// 初始最大成长率
	INT				nMoveSpeed;			// 移动速度
	INT				nModelID;			// 对应表现ＩＤ
	INT				nModelID_A;			// 对应表现ID
	INT				nSkillNum;			// 骑乘技能数
	const CHAR	*	szSkill_1;			// 骑乘技能1
	const CHAR	*	szSkill_2;			// 骑乘技能2
	const CHAR	*	szSkill_3;			// 骑乘技能3
	const CHAR	*	szSkill_4;			// 骑乘技能4
	const CHAR	*	szSkill_5;			// 骑乘技能5
	const CHAR	*	szSkill_6;			// 骑乘技能6
	INT				nSavvySklll;		// 骑乘领悟技能ＩＤ

};

/////////////////////////////////////////////////////////////////////
// 每日提醒相关
/////////////////////////////////////////////////////////////////////
#define MAX_NPC_DIALOG_LEN 70
#define MAX_SCENE_LEN 10
#define MAX_MIS_NAME_LEN 24
#define MAX_MIS_TEXT_LEN 24			//任务描述
#define MAX_COUNTRY_NUM 4
#define GUILD_FAMILY_SIZE 99  
#define NPC_NAME_LEN 32

const INT MAX_LEVEL_NUM		= 100;
const INT MAX_DAY_TIPS_NUM	= 256;
const INT MONTH_WEEK_NUM	= 7;	//每月最多6周
const INT WEEK_DAY_NUM		= 7;	//每周最多7天

//任务的有效时间片
struct VALID_TIME_SLICE
{
	tm		m_Begin;		//开始时间
	tm		m_End;			//结束时间
};


/////////////////////////////////////////////////////////////////////
// 每日提醒功能属性
/////////////////////////////////////////////////////////////////////
#define DBC_DAY_TIPS		(929)
struct _DBC_DAY_TIPS
{
	INT		m_Index;					//index
	BOOL	m_Used;						//是否已初始化
	INT		m_DayNominate;				//日推荐
	INT		m_MonthNominate;			//月推荐
	INT     m_ShowAll;					//是否全部可见	0为否，1为所有人可见

	//可见等级	读表中0表示所有等级段。
	//代码中遇0时，会将数组所有元素置1,代码中就没有0了.
	//此后0表示不可见，1表示可见。
	INT     m_ShowLevel[MAX_LEVEL_NUM];	

	//可见国家,	0表示所有国家。
	//其它同m_ShowLevel
	INT		m_ShowCountry[MAX_COUNTRY_NUM];	

	//可见帮会		0表示所有帮会
	//其它同m_ShowLevel
	INT		m_ShowGuild[GUILD_FAMILY_SIZE];	

	//可见家族		0表示有家族		
	INT		m_ShowFamily[GUILD_FAMILY_SIZE];		

	INT		m_MisType;					//任务类型		0表示循环任务，1表示定时玩法。
	CHAR	m_MisName[MAX_MIS_NAME_LEN];//任务名称

	//任务有效月份		0表示不限月，否则表示可做该任务的月份
	INT		m_MisValidMonth[13];		//为了让数组下标表示月份，m_MisValidMonth[0]废弃, m_MisValidMonth[1]~[12]表示月份
	INT		m_WeekOrDay;				//任务以星期或日期来做，0表示星期

	//m_WeekOrDay=1时的任务有效日期,0表示不限时。要注意每个月不是都在31天
	INT    m_MisValidDate[32]; // m_MisValidDate[1]~[31] 有效

	//任务以周为单位来做
	//第一个字段表示第几周，为0表示不限周
	//第二个字段表示周几，为0表示不限
	INT		m_MisValidWeek[MONTH_WEEK_NUM][WEEK_DAY_NUM];	

	//任务开始到结束的时间,为0表示不限时
	VALID_TIME_SLICE	m_VailidTimeSlice;

	INT		m_MisPos;					//表示读取此任务次数的相关变量及脚本位置,越看越是一头雾水
	INT		m_MisCount;					//每日可做次数
	CHAR	m_MisText[MAX_MIS_TEXT_LEN];//任务描述
	CHAR	m_NpcName[NPC_NAME_LEN];	//怪物名
	INT		m_ScendId;					//OBJ所在场景ID
	CHAR	m_SceneName[MAX_SCENE_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	// 活动参数保存在任务参数数组的序号，对应ScriptGlobal.lua里MissionData中各个数据的定义
	INT		m_DoneCountIndex;	    // 活动完成次数下标 
	INT		m_DoneCircleIndex;  	// 活动完成环数下标
	INT		m_LastDoneTimeIndex;    // 活动最后一次完成时间下标
	INT		m_LastAbandonTimeIndex; // 活动最后一次放弃时间下标
};


/////////////////////////////////////////////////////////////////////
// 排行榜表
/////////////////////////////////////////////////////////////////////
#define DBC_TOP_LIST		(931)
struct _DBC_TOP_LIST
{
	UINT			nID;		// ID
	const CHAR*		szName;		// 排行榜名
	const CHAR*		szType;		// 所属类(分主类和子类)
	FLOAT			fRefresh;	// 刷新间隔(小时)
	INT				nListNum;	// 排名数量
};

/////////////////////////////////////////////////////////////////////
// 任务管理表
/////////////////////////////////////////////////////////////////////
#define DBC_QUEST_LOG		(932)
struct _DBC_QUEST_LOG
{
	UINT			nScriptID;		// 脚本 ScriptID
	INT				nMissionID;		// 任务 MissionID
	const CHAR*		szName;			// 任务名称
	INT				nSceneID;		// 接任务地图ID
	const CHAR*		szNPCName;		// 接任务NPC名称
	const CHAR*		szNPCCommit;	// 交任务NPC名称 // 20100511 ModifyCode
	INT				nNPC_X;			// 接任务NPC_X坐标
	INT				nNPC_Z;			// 接任务NPC_Z坐标
	INT				nMinLevel;		// 接任务最低等级
	INT				nMaxLevel;		// 接任务最高等级

	// 前置任务 MissionID
	INT				nPrevMission1;	// 前置任务1 
	INT				nPrevMission2;	// 前置任务2 
	INT				nPrevMission3;	// 前置任务3 
	INT				nPrevMission4;	// 前置任务4 
	INT				nPrevMission5;	// 前置任务5 
};

/////////////////////////////////////////////////
//模型配置表格
/////////////////////////////////////////////////
#define DBC_MODEL_PARAMETER (933)
struct _DBC_MODEL_PARAMETER
{
	INT		nID;				// Index
	LPCSTR	szModelName;		// Model Name
	FLOAT	nHeight;			// Height
	FLOAT	nDistance;			// Distance
};

/////////////////////////////////////////////////////////////////////
// 搜侠录相关数据表
/////////////////////////////////////////////////////////////////////
#define DBC_SOUXIA_DATA			(934)
struct	_DBC_SOUXIA_DATA
{
	UINT			nID;			//序号
	const CHAR*		szName;	//名称
	INT				nNeedLevel;		//使用等级
	INT				nType;				//道具类型(	0.搜侠录道具， 1.搜侠录记忆棒，	2.拆卸道具， 3.炼化道具， 4.缝合道具		)

	//搜侠录道具数据
	INT				nSouxiaLevel;	//搜侠录道具的等级品质(0,1,2,3,4分别代表入门级，初级，中级，高级，特殊搜侠录)
	INT				nSouxiaParam0_AllPageNum;		//搜侠录的总页数上限
	INT				nSouxiaParam1_SkillPageNum;		//搜侠录技能页数的上限
	INT				nSouxiaParam2_ArmPageNum;	//搜侠录武器配方页数的上限
	INT				nSouxiaParam3_PetPageNum;		//搜侠录神兽页数上限
	INT				nSouxiaParam4_RidePageNum;	//搜侠录坐骑页数上限

	//搜侠录记忆棒数据
	INT				nSouxiaUsbType;		//搜侠录记忆棒道具的类型(1,2,3,4 分别表示技能，武器配方，神兽召唤，坐骑召唤)
	const CHAR*		szSouxiaUsbParam0_SkillID;		//存储的技能标号
	const CHAR*		szSouxiaUsbParam1_ArmID;		//存储的武器配方的ID
	const CHAR*		szSouxiaUsbParam2_PetID;		//存储的神兽召唤技能的ID
	const CHAR*		szSouxiaUsbParam3_RideID;		//存储的坐骑召唤技能的ID

	//搜侠录拆卸道具数据
	INT				nSouxiaStripLevel;	//搜侠录拆卸道具的品质
	INT				nSouxiaStripParam0_Skill;				//拆卸功能，表示保留几个技能页
	INT				nSouxiaStripParam1_SkillOdds;		//拆卸技能页的成功概率
	INT				nSouxiaStripParam2_Arm;			//拆卸功能，表示保留几个武器配方页
	INT				nSouxiaStripParam3_ArmOdds;	//拆卸武器配方页的成功概率
	INT				nSouxiaStripParam4_Pet;				//拆卸功能，表示保留几个神兽召唤技能页
	INT				nSouxiaStripParam5_PetOdds;		//拆卸神兽召唤技能页的成功概率
	INT				nSouxiaStripParam6_Ride;			//拆卸功能，表示保留几个坐骑召唤技能页
	INT				nSouxiaStripParam7_RideOdds;	//拆卸坐骑召唤技能页的成功概率

	//搜侠录炼化道具数据
	INT				nSouxiaSmeltLevel;	//搜侠录炼化道具的品质（0-4，分别表示低级，初级，中级，高级，特殊炼化道具）
	INT				nSouxiaSmeltParam0_Skill;			//炼化功能，表示保留几个技能页
	INT				nSouxiaSmeltParam1_SkillOdds;	//炼化技能页的成功概率
	INT				nSouxiaSmeltParam2_Arm;			//炼化功能，表示保留几个武器配方页
	INT				nSouxiaSmeltParam3_ArmOdds;	//炼化武器配方页的成功概率
	INT				nSouxiaSmeltParam4_Pet;			//炼化功能，表示保留几个神兽召唤技能页
	INT				nSouxiaSmeltParam5_PetOdds;	//炼化神兽召唤页的成功概率
	INT				nSouxiaSmeltParam6_Ride;			//炼化功能，表示保留几个坐骑召唤技能页
	INT				nSouxiaSmeltParam7_RideOdds;	//炼化坐骑召唤技能页的成功概率

	INT				nSutrueOdds;		//搜侠录缝合道具的成功概率
	INT				nConjureNum;		//表示对应（神兽/坐骑）召唤技能使用的次数
};


/////////////////////////////////////////////////////////////////////
// 场景跳转图片数据表 20100517 AddCode
/////////////////////////////////////////////////////////////////////
#define DBC_MAPPIC_DATA			(935)
struct	_DBC_MAPPIC_DATA
{
	static const int MAPPIC_SCENEID_COLUMN = 1;	// 按列查找时，默认查找场景ID列

	INT		nID;				// Index
	INT		nSceneID;			// 场景ID
	LPCTSTR	szPicName;			// 跳转图片名称
};

/////////////////////////////////////////////////////////////////////
// 场景跳转文字提示数据表 20100517 AddCode
/////////////////////////////////////////////////////////////////////
#define DBC_CHATHELP_DATA			(936)
struct	_DBC_CHATHELP_DATA
{
	static const int CHATHELP_ID_COLUMN = 0;

	INT		nID;				// Index
	LPCTSTR	szTip;				// 提示内容
};


//#define DBC_LOGINSERVER_DATA			(937)
struct	_DBC_LOGINSERVER_DATA
{
	INT	nID;
	const CHAR* szAreaName;		//所属大区
	const CHAR* szServerName;		//服务器名
	const CHAR* szTip;			//Tip说明
	INT	nProvider;					//0网通，1电信
	INT	nServerAreaID;
	INT	nServerStatus;			//服务器荷载：0为爆满，1为繁忙，2为良好，3为极佳，4为维护，10为备用服务器
	INT nTuiJian;						//是否为推荐服务器
	INT	nIsNewServer;			//是否为新开服务器
	const CHAR* szServerIP;	//LoginServer IP及Port
};

/** 搜侠召唤技能所要转换的召唤兽的ID */
#define DBC_IMPACT_SOUXIAMODEL		(938)
struct	_DBC_IMPACT_SOUXIAMODEL
{
	INT nID;				//效果ID
	INT nModelID;		//召唤兽ID
};

#define DBC_BUS_DATA		(939)

#define DBC_IB_SHOP			(940)
struct	_DBC_IB_SHOP
{
	UINT			nID;		//商城序号
	UINT			nItemID;	//道具ID
	const CHAR*		szDesc;		//描述
	UINT			nPrice;		//价格
	INT				bIsNewSell; //是否属于新品
	INT				bIsHotSell; //是否属于热卖商品
	INT				bIsCutPrice;//是否属于打折促销
	INT				CutPriceRate;//打折促销比例
	INT				bIsPetShop;		//是否属于宠物商铺
	INT				bIsEquipShop;	//是否属于装备商铺
	INT				bIsMedicShop;	//是否属于药品商铺
	INT				bIsSpecialShop;	//是否属于异术商铺
	INT				bIsColligateShop;//是否属于综合商铺
};

/////////////////////////////////////////////////////////////////////
// 坐骑属性表    [7/8/2010 陈军龙] 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MOUNT_ATT		(941)
struct _DBC_ITEM_MOUNT_ATT		// ef
{
	INT		         nID;                 //id
	LPCSTR	         szName;              //名字
	LPCSTR	         szDesc;              //描述
	LPCSTR	         szIcon;              //icon
	INT		         nRule;		          //适应规则
	INT              nNeedLevel;          //需要达到的等级
	INT		         nAddtionSpeed;       //增加的速度
	INT              nLife;               //存在时限（单位：天）
	INT              nBindType;           //绑定类型 MOUNT_BIND_TYPE
	INT              nPrice;              //所需金钱
    INT              nCharMountID;        //外形ID

	//... NOT CARE
};
