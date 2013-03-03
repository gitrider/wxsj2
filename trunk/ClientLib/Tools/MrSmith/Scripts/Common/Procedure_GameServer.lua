--------------------------------------
-- ������Ϸ����������
--------------------------------------
local STATE_DISCONNECT 			= "disconnect";			--����״̬
local STATE_CONNECTING 			= "connecting";			--��������GameServer
local STATE_CONNECTED_SUCCESS 	= "conn_success";		--�ɹ�����GameServer
local STATE_CONNECTED_FAILED 	= "conn_failed";		--����GameServerʧ��
local STATE_CONNECT_VALIDATING	= "conn_validating"		--�Ѿ�����CGConnect
local STATE_CONNECT_VAL_SUCCESS	= "conn_val_success"	--������֤�ɹ�
local STATE_CONNECT_VAL_FAILED	= "conn_val_failed"		--������֤ʧ��

local STATE_CURRENT;

local	g_IsEnterLogin						= 1;									--�Ƿ��¼Login

--����״̬
function Procedure_GameServer_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_GameServer_Init()

end

function Procedure_GameServer_Tick()

	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_DISCONNECT) then
		--����Game������
		local ip = Variable:GetVariable("GameServer_Address");
		local port = Variable:GetVariable("GameServer_Port");
		Loger:AddLog("Connect to gameserver..." .. ip .. "," .. port);
		local connErrCode = NetManager:ConnectToServer(ip, tonumber(port), "GameServer");

		if(connErrCode == 0) then
			Loger:AddLog("GameServer SOCKET ���ӳɹ�...");
			Procedure_GameServer_SetState(STATE_CONNECTED_SUCCESS);
		else
			Procedure_GameServer_SetState(STATE_CONNECTED_FAILED);
		end
				
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTING) then

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_SUCCESS) then
	
		local	name	= Variable:GetVariable("AccountName");
		local	hash	= Util:HashString2CRC(name);
		local	sex		= Variable:GetVariable("AccountGender");
		if sex == "" then
			sex				= "0"
		end

		--ֱ��������Ϸ������
		if g_IsEnterLogin ~= 1 then
			Variable:SetVariable("User_NAME", name);
			Variable:SetVariable("User_GUID", hash);
			Variable:SetVariable("User_GENDER", sex);
			Variable:SetVariable("GameServer_Key", "-1");
		end

		NetManager:SendMessage("CGConnect");

		local	str		= string.format( "�ȴ�������֤��...(NAME:%s,GUID:%X", name, hash )
		Loger:AddLog(str);
		Procedure_GameServer_SetState(STATE_CONNECT_VALIDATING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_FAILED) then
		Util:ThrowException("GameServer SOCKET ����ʧ��...");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VALIDATING) then

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_SUCCESS) then
		--ת����볡������
		Loger:AddLog("������֤�ɹ�!");
	
		Procedure_EnterScene_SetState("ready");
		Variable:SetVariable("Scene_Entertype", "0");
		Procedure:SetCurrentProcedure("EnterScene");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_FAILED) then
		--do something!!!!

	else

	end
end

function Procedure_GameServer_Release()

end
