
/** �������Ѱ�ӿ�
 */


#include "Type.h"
#include "../DataPool/GMDP_Struct_Pet.h"



namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

struct _PET_DETAIL_ATTRIB;
struct SDATA_PET;
struct _PET_PLACARD_ITEM;
class CObject_PlayerNPC;

namespace SCRIPT_SANDBOX
{
	class PetInviteFriend
	{
	public:
		struct PET_FRIEND
		{
			SDATA_PET			m_Pet;
			CObject_PlayerNPC*	m_Avatar;

			PET_FRIEND()
			{
				m_Avatar = NULL;
			}

			virtual ~PET_FRIEND()
			{
				m_Pet.CleanUp();
				m_Avatar = NULL;
			}
		};

	//LuaPlus������ؽӿ�
	public:
		//��ÿͻ��˵�������Ϣ������
		INT GetInviteNum(LuaPlus::LuaState* state);

		//������ѳ�������ϸ��Ϣ�����ơ��ȼ�֮�ࣩ
		INT GetHumanINFO(LuaPlus::LuaState* state);

		//��ó����Գ���Ľ�����Ϣ
		INT GetInviteMsg(LuaPlus::LuaState* state);

		//���ý�������ʾ��ģ��
		INT	SetPetModel(LuaPlus::LuaState* state);

		//��ʾ��Ӧ�������ϸ��Ϣ����Ҫ����TargetPet��������ʾ
		INT	ShowTargetPet(LuaPlus::LuaState* state);

		//��ó����������Ϣ
		INT	GetPetINFO(LuaPlus::LuaState* state);

	//���紦����ؽӿ�
	public:
		VOID							ConvertPlaceCard2PetFriend();

	//���ú����ӿ�
	public:
		VOID							PET_DETAIL_ATTRIB2SDATA_PAT(const _PET_DETAIL_ATTRIB* pIn, SDATA_PET* pOut);
		VOID							InitPetList();
		VOID							CleanUpPetList();
		VOID							DestoryPetList();
		VOID							SetPetListFakeObj(INT idx, INT rdx);	//idx ����λ�ã� rdx ģ�ͱ��
		SDATA_PET*						GetPetListData(INT idx);
		const _PET_PLACARD_ITEM*		GetPlaceCardItem(INT idx);

	//�����������Դ��ת������������
	protected:
		std::vector< PET_FRIEND* >		m_PetList;

	public:
		PetInviteFriend();
		virtual ~PetInviteFriend();

		static PetInviteFriend s_PetInviteFriend;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
};