
/**	GMInterface_Script_Country.h
 *
 *	功能：	国家功能脚本接口
 *
 *	修改记录：	
 *			080417	创建： 产生国王， 确定产生国王， 获取国家官员信息， 修改现有国家官员职位， 产生皇帝
 *			
 */

#include "Type.h"


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};



namespace SCRIPT_SANDBOX
{

	// 国家
	class Country
	{
	public:

		// 产生国王
		INT CreateCountryKing( LuaPlus::LuaState* state );

		// 确定产生国王
		INT CreateCountryKingConfirm( LuaPlus::LuaState* state );

		// 请求国家官员信息
		INT RequestCountryFunctionary( LuaPlus::LuaState* state );

		// 获取国家官员信息
		INT GetCountryFunctionary( LuaPlus::LuaState* state );

		// 修改现有国家官员职位
		INT	AdjustFunctionaryAuth( LuaPlus::LuaState* state );

		// 任命官员
		INT AppointFunctionary( LuaPlus::LuaState* state );
		// 罢免
		INT DismissFunctionary( LuaPlus::LuaState* state );

		// 产生皇帝
		INT CreateMonarch( LuaPlus::LuaState* state );


	private:

	public:

		Country();
		virtual ~Country();

		static Country s_Country;
		static LuaPlus::LuaObject* s_pMetaTable;

	};	// class

}	// namespace 

