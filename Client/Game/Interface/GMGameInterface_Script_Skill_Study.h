
/* 技能学习相关接口
*/

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

namespace SCRIPT_SANDBOX
{
	class SkillStudy
	{
	//LuaPlus界面相关接口
	public:
		// 学习一个新技能
		INT	StudySkill( LuaPlus::LuaState* state );

		// 得到 该技能的可学习状态, 返回 0 -- 不可学, 1 -- 可学
		INT GetStudyState( LuaPlus::LuaState* state );

		// 得到 该技能级别
		INT GetSkillLevel( LuaPlus::LuaState* state );

		// 得到 技能最大级别
		INT	GetSkillMaxLevel( LuaPlus::LuaState* state );

		// 得到 已经分配到某一个系列的技能点数
		INT GetSeriesPoint( LuaPlus::LuaState* state );

		// 得到 技能说明
		INT GetSkillExplain( LuaPlus::LuaState* state );

		// 得到 父技能ID
		INT GetFatherSkillID( LuaPlus::LuaState* state );

		// 得到 剩余技能点数
		INT	GetSkillPoint( LuaPlus::LuaState* state );

		// 20100421 ModifyCodeBegin
		// laim
		// INT GetSkillType(LuaPlus::LuaState* state){return 0;};
		// 得到技能类型
		INT GetSkillType(LuaPlus::LuaState* state);
		// 20100421 ModifyCodeEnd

		// 20100421 AddCodeBegin
		// 得到技能名称
		INT GetSkillName(LuaPlus::LuaState* state);
		// 20100421 AddCodeEnd

		// 20100421 AddCodeBegin
		// 得到技能主/被动标记
		INT GetSkillPassiveFlag(LuaPlus::LuaState* state);

		//向服务器请求是否可以学习该技能
		INT RequestIsStudySkill( LuaPlus::LuaState* state );

		// 20100421 AddCodeEnd
	protected:
		// 解析技能数据项, 返回整数
		INT GetSkillData( const CHAR* szSkillData, INT nLevel );

		// 得到 该技能的可学习状态, 递归技能树
		BOOL ClimbStudyTree( INT nSkillID, INT nSkillLevel );

		// 得到 已经分配到某一个系列的技能点数
		INT SeriesPoint( INT nSeriesID );

	public:
		SkillStudy(){};
		virtual ~SkillStudy(){};

		static SkillStudy s_SkillStudy;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
}