#pragma once

#include <CEGUIScriptModule.h>

class tScriptSandbox;
namespace CEGUI
{
	
class CGameUIScript : public ScriptModule
{
public:
	//ִ��ĳ���ű��ļ�
	virtual	void	executeScriptFile(const String& filename, const String& resourceGroup = "");
	//ִ��ĳ��ȫ�ֽű�����
	virtual int		executeScriptGlobal(const String& function_name);
	//��ӦUI�¼�
	virtual	bool	executeScriptedEventHandler(const String& handler_name, const EventArgs& e);
	//ִ��ĳ���ű����
    virtual void	executeString(const String& str);
	//��ʼ�ű�ϵͳ��
    virtual void	createBindings(void);
	//�����ű���
    virtual void	destroyBindings(void);

public:
	CGameUIScript();
	virtual ~CGameUIScript();
};

}