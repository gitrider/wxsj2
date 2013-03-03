
--------------------------------------
-- ��¼Login����
--------------------------------------
local STATE_DISCONNECT_L				= "disconnect_L";			--����״̬������Login������
local STATE_DISCONNECT_G				= "disconnect_G";			--����״̬��ֱ��������Ϸ������
local STATE_CONNECTING 					= "connecting";				--��������Login������
local STATE_CONNECTED_SUCCESS 	= "conn_success";			--�ɹ�����Login
local STATE_CONNECTED_FAILED 		= "conn_failed";			--����Loginʧ��
local STATE_CONNECT_VALIDATING	= "conn_validating"		--�Ѿ�����CLConnect
local STATE_CONNECT_VAL_SUCCESS	= "conn_val_success"	--������֤�ɹ�
local STATE_CONNECT_VAL_FAILED	= "conn_val_failed"		--������֤ʧ��
local STATE_ACCOUNT_VALIDATING	= "account_validating"--�ʺ���֤��
local STATE_ACCOUNT_VAL_SUCCESS	= "accnt_val_success"	--�ʺ���֤�ɹ�
local STATE_ACCOUNT_VAL_FAILED	= "accnt_val_failed"	--�ʺ���֤ʧ��

local STATE_CURRENT;

--����״̬
function Procedure_Login_SetState(stateName)
	STATE_CURRENT = stateName;	
end

--��ʼ��
function Procedure_Login_Init()

end

--Tick
function Procedure_Login_Tick()

	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_DISCONNECT_L) then
		--����Login������
		local ip	= Variable:GetVariable("LoginServer_Address");
		local port= Variable:GetVariable("LoginServer_Port");
		local	name= Variable:GetVariable("AccountName");
		Loger:AddLog("Connect to loginserver..." .. ip .. "," .. port);
		
		--��������
		local	connErrCode	= -2;
		local	log	= 0;
		while log < 1000 do
			log							= log + 1;
			connErrCode			= NetManager:ConnectToServer(ip, tonumber(port), "LoginServer");
			if connErrCode >= 0 then
				break
			end
		end

		if(connErrCode == 0) then
			Loger:AddLog("LoginServer SOCKET ���ӳɹ�..." .. name );
			Procedure_Login_SetState(STATE_CONNECTED_SUCCESS);
		else
			Procedure_Login_SetState(STATE_CONNECTED_FAILED);
		end

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_DISCONNECT_G) then
		--ֱ��������Ϸ������
		local	ip	= Variable:GetVariable("LoginServer_Address");
		local	port= Variable:GetVariable("LoginServer_Port");
		local	name= Variable:GetVariable("AccountName");
		Variable:SetVariable("GameServer_Address",ip);
		Variable:SetVariable("GameServer_Port",port);
		Variable:SetVariable("User_NAME",name);
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTING) then
		--Loger:AddLog(STATE_CURRENT);
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_SUCCESS) then
		Util:Sleep( 1000 );
		NetManager:SendMessage("CLConnect");
		Procedure_Login_SetState(STATE_CONNECT_VALIDATING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_FAILED) then
		Util:ThrowException("LoginServer SOCKET ����ʧ��...");
				
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VALIDATING) then
		--Loger:AddLog(STATE_CURRENT);
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_SUCCESS) then
		--�����ʺ�����
		Util:Sleep( 800 );
		NetManager:SendMessage("CLAskLogin");
		Procedure_Login_SetState(STATE_ACCOUNT_VALIDATING);
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_FAILED) then
		Util:ThrowException("LoginServer ��֤ʧ��!");
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VALIDATING) then
		--�ȴ��ʺ���֤
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VAL_SUCCESS) then
		--�����ɫ�б�
		Loger:AddLog("�ʺ���֤ͨ���������ɫѡ������...");
		Procedure_CharSel_SetState("ask_char_list"); --����askcharlist
		Procedure:SetCurrentProcedure("CharSel");
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VAL_FAILED) then
		Util:ThrowException("LoginServer �ʺŵ�½ʧ�� " .. Variable:GetVariable("LoginErrorDesc"));
	else
	
	end
end

--�����˳�
function Procedure_Login_Release()

end

