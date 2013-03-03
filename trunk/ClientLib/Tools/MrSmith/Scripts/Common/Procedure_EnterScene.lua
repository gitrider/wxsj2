
--------------------------------------
-- ���볡������
--------------------------------------
local STATE_READY 			= "ready";			--׼�����
local STATE_REQUESTING		= "requesting";		--���ͽ��볡��������,�ȴ��������Ļ�Ӧ...
local STATE_ENTER_OK		= "enter_ok";		--������������볡��
local STATE_ENTER_FAILED	= "enter_failed";	--�������ܾ����볡��
local STATE_CHANGE_SCENE	= "change_scene";	--�л�����
local STATE_CHANGE_OK		= "change_ok"; -- �л������ɹ�
local STATE_CHANGE_OK2		= "change_ok2"; -- �л���ͬ��Server
local STATE_CHANGE_ERR		= "change_err"; -- �л���������

local STATE_CURRENT;


--����״̬
function Procedure_EnterScene_SetState(stateName)
	STATE_CURRENT = stateName;
end

function Procedure_EnterScene_Init()
	
end

function Procedure_EnterScene_Tick()
	
	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_READY) then
		Util:Sleep( 10 );
		Loger:AddLog("���ͽ��볡��������...!");
		NetManager:SendMessage("CGEnterScene");
		Procedure_EnterScene_SetState(STATE_REQUESTING);
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_SCENE) then	
		Util:Sleep( 10 );
		Loger:AddLog("�����л�����������...!");
		NetManager:SendMessage("CGAskChangeScene");
		Procedure_EnterScene_SetState(STATE_REQUESTING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_OK) then
		Loger:AddLog("�л�����������ɹ�");
		Procedure_EnterScene_SetState(STATE_READY);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_OK2) then
		Loger:AddLog("�л�����������ɹ���!!  ��ȥ��ķ������� ^_^");
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_ERR) then
		Loger:AddLog("�л�����������ʧ����...  :(");
		Procedure_Main_SetState("rand_move");
		Procedure:SetCurrentProcedure("Main");
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_REQUESTING) then
		--Loger:AddLog(STATE_CURRENT);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ENTER_OK) then
		--���볡��
		Loger:AddLog("���볡��[" .. Variable:GetVariable("Scene_ID") .. "]!" );
		--WorldManager:EnterScene();

		Procedure_Main_SetState("request_scene");
		Procedure:SetCurrentProcedure("Main");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ENTER_FAILED) then
		--do something!!!
		--Loger:AddLog(STATE_CURRENT);
		
	else
	
	end
end

function Procedure_EnterScene_Release()

end

