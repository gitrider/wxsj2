
/**	GIUISystem.h
 */

#pragma once


#include "GIBasicType.h"
#include "GINode.h"
#include "GIMath.h"
#include "GITransferItemSystem.h"



/// ����ģ������������
enum
{
	TALK_TEMPLATE_NOBODY = 0,
	TALK_TEMPLATE_OTHER,
	TALK_TEMPLATE_MYSELF,

	TALK_TEMPLATE_MAX
};

// ��������
enum FILTER_TYPE
{
	FT_NONE = 0,	// ������
	FT_CHAT,		// ����
	FT_ROLE,		// ��������
	FT_NAME,		// ����
	FT_STALL,		// ��̯

	FT_MAX,
};

// ���˵ȼ�
enum FILTER_RANK
{
	FR_NO_MATCH = 0,	// ����
	FR_PART_MATCH,		// ����ƥ��
	FR_FULL_MATCH,		// ��ȫƥ��

	FR_MAX,
};

#define TALK_HELPSTRING_MAX		200
#define TALK_HELPSTRING_KEY		"help"


/** ��ɫͷ���ϵ���Ϣ��
 */
class KYLIN_API tCreatureBoard
{
public:

	/** ɾ������ */
	virtual VOID			Destroy(VOID)	= 0;
	/** ��������Ļ�ϵ�����(����) */
	virtual VOID			SetPosition(FLOAT nX, FLOAT nY) = 0;
	/** ��ʾ/���� */
	virtual VOID			Show(BOOL bShow) = 0;
	/** ��ʾ/����������ԣ�Ѫ���͵ȼ��� */
	virtual VOID			ShowCharAttr(BOOL bShow) = 0;
	/** ��ʾ/������ҳƺ� */
	virtual VOID			ShowCharTitle(BOOL bShow, INT nType) = 0;

	/** �������� */
	virtual VOID			SetElement_Name(LPCTSTR szName)				= 0;
	/** ��������ֵ */
	virtual VOID			SetElement_HPProgress( INT hp, INT maxHp )	= 0;
	virtual	VOID			SetElement_HPProgress( FLOAT hpPercent )	= 0;
	/** ���õȼ� */
	virtual VOID			SetElement_Level( INT nLevel, INT nType )			= 0;
	/** ���óƺ� */
	virtual VOID			SetElement_Title(LPCTSTR szName, INT nType = 0)		= 0;
	/** ���ù��ұ�� */
	virtual VOID			SetElement_CountryFlag(INT nCountry)	= 0;
	/** ���öӳ���� */
	virtual VOID			SetElement_LeaderFlag( BOOL bShowj, INT nType )		= 0;
	/** ���� pk��� | ������ʾ */
	virtual VOID			SetElement_PKFlag( INT nType, INT nState, BOOL bShow ) = 0;
	/** ���ð�̯�ϵ����� */
	virtual VOID			SetElement_SaleText(LPCTSTR szSaleText) = 0;
	/** �����Ƿ���ʾ��̯��Ϣ */
	virtual VOID			SetElement_SaleSign(BOOL bShow)			= 0;
	/** �����߼�����ID */
	virtual VOID			SetElement_ObjId(INT nObjId)			= 0;
	// ������ҵ�״̬��������������
	virtual VOID			SetInfoState( int nState )				= 0;
	// ������Ϣ�����ǵ�λ��
	virtual void			SetInfoDistance( float nDistance )		= 0;
	// �������ͷ��������������
	virtual VOID			SetElement_PaoPaoText( LPCTSTR szPaoPao ) = 0;
	// �õ���Ļλ��
	virtual fVector2		GetPosition(void) const = 0;
	// �Ƿ���ʾ
	virtual BOOL			isShow() = 0;

	// ��ʾ��̯״̬����Ϣ��		20100527 BLL
	virtual VOID				ShowIsInStall( BOOL bIsMyself ) = 0;

};


/** UIϵͳ��ӿ�
 */
class KYLIN_API tUISystem : public tNode
{
public:

	// ���ڴ�С�����ı�
	virtual VOID			OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)	= 0;
	// ��������,�������TRUE�����ʾui�ػ�������
	virtual VOID			InjectInput(VOID) = 0;
	// ����������Ϣ
	virtual BOOL			MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	//����Ƿ���ui�Ͽ�
	virtual BOOL			IsMouseHover(VOID) = 0;
	//����Ƿ���������Ϣ�Ͽ�
	virtual BOOL			IsMouseHover_ChatBoard(LONG x, LONG y) = 0;
	//���������Ϣ��ĳ����е���Ϣ
	virtual STRING			getChatBoard_HyplinkContex(LONG x, LONG y) = 0;
	//����һ���µĽ�ɫ��Ϣ��
	virtual tCreatureBoard*	CreateCreatureBoard(VOID) = 0;
	virtual VOID			AddNewBeHitBoard( bool bDouble, LPCTSTR szInfo, FLOAT nStartX, FLOAT nStartY, int nType = 0 ,int nMoveType = 0) = 0;
	//ĳһ��UI�Ƿ��ڴ���
	virtual BOOL			IsWindowShow(LPCTSTR szUIName, LPCTSTR szChildName = NULL) = 0;
	virtual BOOL			CloseAllWindow() = 0;
	//֪ͨUI����һ��UI��ʾģ��
	virtual VOID			FakeObj_Show(LPCTSTR szWinName, LPCTSTR szName) = 0;
	//֪ͨUI����һ��UI��ʾģ��
	virtual VOID			FakeObj_Close(LPCTSTR szWinName, LPCTSTR szName) = 0;

	//--- FOR DEBUG
	virtual VOID		Debug_PushChatString(LPCTSTR szName, LPCTSTR szMsg)	= 0;
	virtual VOID		Debug_SaveFontTexture(VOID) = 0;
	//--- FOR DEBUG

	virtual VOID			ChangeDragFlag(bool Flag) = 0;
	virtual BOOL			IsDragFlag(VOID) = 0;

	//�߼������ϵͳ֪ͨ��Ϣ
	virtual STRING			ParserString_VarParam(LPCTSTR strID, ... ) = 0;
	virtual VOID			ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl = FALSE) = 0;
	virtual STRING			ParserString_NoColor_VarParam(LPCTSTR strID, ... ) = 0;
	virtual STRING			ParserString_NoVarParam(LPCTSTR strID) = 0;
	virtual STRING			ParserString_NoColor_NoVarParam(LPCTSTR strID) = 0;

	//����ģ���ִ���ȡ��
	virtual STRING			getTalkTemplate(const STRING& strKey, UINT index) = 0;
	virtual STRING			ModifyChatTemplate(const STRING& strTemp, const STRING& strTalker, const STRING& strTarget) = 0;

	virtual STRING			getTalkRandHelpMsg() = 0;
	//�鿴��ǰ��ȡ�����EditBox
	virtual BOOL			IsIMEEditboxActive(LPCTSTR szWinName) = 0;
	//���뷨״̬
	virtual INT				GetCurrentIMEState() = 0;
	//��ȡEditBox������
	virtual STRING			getIMEEditboxString(LPCTSTR szWinName) =0;


	//��һ����Ʒ��Ϣ����
	virtual bool			InjectItemInfo(tTransferItemSystem::Element* pElement) = 0;

	virtual void			OnPushEcsape() = 0;
	virtual void			SetPaoPaoActive( bool bActive ) = 0;
	virtual bool			IsPaoPaoActive() = 0;

//	virtual UINT			OgreColor_CEGUIColor_Limit_Lumination( UINT OgreColor,FLOAT fLumination) = 0;

	virtual UINT			Lumination_OgreColor( UINT OgreColor, INT Lumination ) = 0;
	virtual INT				OgreColor_Lumination( UINT OgreColor ) = 0;

	//�����ַ����еķǷ������ַ�
	virtual BOOL	CheckStringFilter(const STRING& strIn, const FILTER_TYPE filterType = FT_NONE) = 0;
	virtual BOOL	CheckStringCode(const STRING& strIn, STRING& strOut) = 0;
	//��ȫƥ�����
	virtual BOOL	CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType = "all", BOOL bUseAllTable = TRUE) = 0;

	//��ȡ��ǰ���������Ĵ���
	virtual STRING	GetDebugString(VOID) = 0;
	

	//��ȡ��ǰ���������Ĵ���
	virtual void	OpenWindow(const STRING& winName) = 0;
	virtual void	CloseWindow(const STRING& winName) = 0;
	virtual void	ToggleWindow(const STRING& winName) = 0;

	virtual bool	HasInputFocus() = 0;

	//���������Ϣ�������ʾ������Ϣ��Ч�ʣ�
	virtual bool			addChatHistoryMessage(INT msgID,LPCTSTR szWinName, LPCTSTR szMessage, INT nOp = -1, UINT nDisappearTime = 0) =0;
	virtual VOID		ReplaceStringUseFilter(const STRING& strIn, STRING& strOut, FILTER_TYPE filterType = FT_CHAT) =0;
	virtual std::string		string_check_valid(const char* szString) = 0;

	virtual bool	ReloadWindowScript( const STRING& windowName ) =0;

	virtual VOID	UIRenderToggleShow(VOID) =0;
	
	KL_DECLARE_DYNAMIC(tUISystem);

};