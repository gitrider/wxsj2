
/* ����ѧϰ��ؽӿ�
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
	//LuaPlus������ؽӿ�
	public:
		// ѧϰһ���¼���
		INT	StudySkill( LuaPlus::LuaState* state );

		// �õ� �ü��ܵĿ�ѧϰ״̬, ���� 0 -- ����ѧ, 1 -- ��ѧ
		INT GetStudyState( LuaPlus::LuaState* state );

		// �õ� �ü��ܼ���
		INT GetSkillLevel( LuaPlus::LuaState* state );

		// �õ� ������󼶱�
		INT	GetSkillMaxLevel( LuaPlus::LuaState* state );

		// �õ� �Ѿ����䵽ĳһ��ϵ�еļ��ܵ���
		INT GetSeriesPoint( LuaPlus::LuaState* state );

		// �õ� ����˵��
		INT GetSkillExplain( LuaPlus::LuaState* state );

		// �õ� ������ID
		INT GetFatherSkillID( LuaPlus::LuaState* state );

		// �õ� ʣ�༼�ܵ���
		INT	GetSkillPoint( LuaPlus::LuaState* state );

		// 20100421 ModifyCodeBegin
		// laim
		// INT GetSkillType(LuaPlus::LuaState* state){return 0;};
		// �õ���������
		INT GetSkillType(LuaPlus::LuaState* state);
		// 20100421 ModifyCodeEnd

		// 20100421 AddCodeBegin
		// �õ���������
		INT GetSkillName(LuaPlus::LuaState* state);
		// 20100421 AddCodeEnd

		// 20100421 AddCodeBegin
		// �õ�������/�������
		INT GetSkillPassiveFlag(LuaPlus::LuaState* state);

		//������������Ƿ����ѧϰ�ü���
		INT RequestIsStudySkill( LuaPlus::LuaState* state );

		// 20100421 AddCodeEnd
	protected:
		// ��������������, ��������
		INT GetSkillData( const CHAR* szSkillData, INT nLevel );

		// �õ� �ü��ܵĿ�ѧϰ״̬, �ݹ鼼����
		BOOL ClimbStudyTree( INT nSkillID, INT nSkillLevel );

		// �õ� �Ѿ����䵽ĳһ��ϵ�еļ��ܵ���
		INT SeriesPoint( INT nSeriesID );

	public:
		SkillStudy(){};
		virtual ~SkillStudy(){};

		static SkillStudy s_SkillStudy;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
}