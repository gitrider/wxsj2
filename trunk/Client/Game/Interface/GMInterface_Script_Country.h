
/**	GMInterface_Script_Country.h
 *
 *	���ܣ�	���ҹ��ܽű��ӿ�
 *
 *	�޸ļ�¼��	
 *			080417	������ ���������� ȷ������������ ��ȡ���ҹ�Ա��Ϣ�� �޸����й��ҹ�Աְλ�� �����ʵ�
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

	// ����
	class Country
	{
	public:

		// ��������
		INT CreateCountryKing( LuaPlus::LuaState* state );

		// ȷ����������
		INT CreateCountryKingConfirm( LuaPlus::LuaState* state );

		// ������ҹ�Ա��Ϣ
		INT RequestCountryFunctionary( LuaPlus::LuaState* state );

		// ��ȡ���ҹ�Ա��Ϣ
		INT GetCountryFunctionary( LuaPlus::LuaState* state );

		// �޸����й��ҹ�Աְλ
		INT	AdjustFunctionaryAuth( LuaPlus::LuaState* state );

		// ������Ա
		INT AppointFunctionary( LuaPlus::LuaState* state );
		// ����
		INT DismissFunctionary( LuaPlus::LuaState* state );

		// �����ʵ�
		INT CreateMonarch( LuaPlus::LuaState* state );


	private:

	public:

		Country();
		virtual ~Country();

		static Country s_Country;
		static LuaPlus::LuaObject* s_pMetaTable;

	};	// class

}	// namespace 

