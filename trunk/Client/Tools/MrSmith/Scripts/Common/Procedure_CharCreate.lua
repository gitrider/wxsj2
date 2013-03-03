--------------------------------------
-- ������ɫ����
--------------------------------------
local STATE_CREATECHAR 					= "create_char";					--������ɫ
local STATE_WAIT_CREATECHAR 		= "wait_create_char";			--�ȴ�������ɫ
local STATE_CREATECHAR_OK				= "create_char_success";	--������ɫ�ɹ�
local STATE_CREATECHAR_REPEAT		= "create_char_repeat";		--������ɫʧ�ܣ�������æ������
local STATE_CREATECHAR_HIGHFRE	= "create_char_too_fast";	--������ɫʧ�ܣ���������Ƶ��
local STATE_CREATECHAR_FULL			= "create_char_full";			--������ɫʧ�ܣ���ɫ�Ѿ�����
local STATE_CREATECHAR_PROC			= "create_char_proc";			--������ɫʧ�ܣ������������
local STATE_CREATECHAR_IN				= "create_char_inner";		--������ɫʧ�ܣ��ڲ�����
local STATE_CREATECHAR_NAME			= "create_char_name";			--������ɫʧ�ܣ������ظ�
local STATE_CREATECHAR_UNKNOW		= "create_char_unknow";		--������ɫʧ�ܣ�δ֪����
local STATE_DELETECHAR 					= "delete_char";					--ɾ����ɫ

local STATE_CURRENT; --״̬

--����״̬
function Procedure_CharCreate_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_CharCreate_Init()

end

function Procedure_CharCreate_Tick()

	if( STATE_CURRENT == STATE_CREATECHAR ) then
		Util:Sleep( 1000 );
		Loger:AddLog("������ɫ..." );
		NetManager:SendMessage("CLAskCreateChar");
		--Procedure_CharCreate_SetState("wait_create_char");
		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");
	
	elseif( STATE_CURRENT == STATE_WAIT_CREATECHAR ) then
		Loger:AddLog("�ȴ�������ɫ������ѡ���û���������");
		--Procedure_CharSel_SetState("wait_ask_char_list");
		--Procedure:SetCurrentProcedure("CharSel");
		--Loger:AddLog(STATE_CURRENT);
		
	elseif( STATE_CURRENT == STATE_CREATECHAR_OK ) then
		--Loger:AddLog("������ɫ�ɹ�������ʲô������");

		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");
		--Loger:AddLog(STATE_CURRENT);
		
	elseif( STATE_CURRENT == STATE_CREATECHAR_REPEAT ) then
		Loger:AddLog("������ɫʧ�ܣ�������æ������...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_HIGHFRE ) then
		Loger:AddLog("������ɫʧ�ܣ���������Ƶ��������...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_FULL ) then
		Loger:AddLog("������ɫʧ�ܣ���ɫ�Ѿ�����...");
		Util:Sleep( 800 );
		Procedure_CharSel_SetState("enter_scene");
		Procedure:SetCurrentProcedure("CharSel");

	elseif( STATE_CURRENT == STATE_CREATECHAR_PROC ) then
		Loger:AddLog("������ɫʧ�ܣ�����������̣����µ�½...");
		Util:Sleep( 2000 );
		Procedure_Login_SetState("disconnect");
		Procedure:SetCurrentProcedure("Login");

	elseif( STATE_CURRENT == STATE_CREATECHAR_IN ) then
		Loger:AddLog("������ɫʧ�ܣ��ڲ���������...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_NAME ) then
		Loger:AddLog("������ɫʧ�ܣ������ظ�������...");
		--Procedure_CharCreate_SetState("create_char");
		Util:Sleep( 5000 );
		Procedure_Login_SetState("disconnect2");
		Procedure:SetCurrentProcedure("Login");

	elseif( STATE_CURRENT == STATE_CREATECHAR_UNKNOW ) then
		Loger:AddLog("������ɫʧ�ܣ�δ֪��������...");
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_DELETECHAR ) then
		Loger:AddLog("ɾ����ɫ..." );
		Util:Sleep( 1000 );
		NetManager:SendMessage("CLAskDeleteChar");
		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");

	end

end

function Procedure_CharCreate_Release()

end

