--------------------------------------
-- ѡ���ɫ����
--------------------------------------
local STATE_ASKCHARLIST 				= "ask_char_list";				--����askcharlist
local STATE_WAIT_ASKCHARLIST 			= "wait_ask_char_list";			--�ȴ�AskCharList�ظ�
local STATE_HAVE_ASKCHARLIST 			= "char_list";					--����Ϣ��
local STATE_HAVENOT_ASKCHARLIST			= "char_list_err";				--��ɫ��Ϣ����
local STATE_ASKCHARLOGIN				= "ask_char_login";				--����askcharlogin
local STATE_WAIT_ASKCHARLOGIN			= "wait_ask_char_login";		--�ȴ�askcharlogin�ظ�
local STATE_ENTER_SCENE					= "enter_scene";				--һ��OK������ֱ�ӽ��볡����
local STATE_ENTER_SCENE_ERROR			= "enter_scene_error";			--���볡������

local STATE_CURRENT; --״̬

--����״̬
function Procedure_CharSel_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_CharSel_Init()
	
end

function Procedure_CharSel_Tick()

	if(STATE_CURRENT == STATE_ASKCHARLIST) then
		--���������ɫ��Ϣ
		Util:Sleep( 1000 );
		Loger:AddLog("����Ask CharList...");
		NetManager:SendMessage("CLAskCharList");
		Procedure_CharSel_SetState(STATE_WAIT_ASKCHARLIST);
		
	elseif(STATE_CURRENT == STATE_WAIT_ASKCHARLIST) then
		--�ȴ�ask�ظ�
		
	elseif(STATE_CURRENT == STATE_HAVE_ASKCHARLIST) then
		local countList = LoginCharacterList:GetCharacterCounts();

		if( countList < 1 ) then
			--�����ɫ��������
			Loger:AddLog("�����ɫ��������..." .. countList);
			Procedure_CharCreate_SetState("create_char");
			Procedure:SetCurrentProcedure("CharCreate");
		else
			Procedure_CharSel_SetState(STATE_ASKCHARLOGIN);
		end
	
	elseif(STATE_CURRENT == STATE_HAVENOT_ASKCHARLIST) then
		local errmsg = Variable:GetVariable("CharListErrorDesc");
		Loger:AddLog("��ɫ��Ϣ����("..errmsg..")�����½���ask_char_list����...");
		Procedure_CharSel_SetState(STATE_ASKCHARLIST);

	elseif(STATE_CURRENT == STATE_ASKCHARLOGIN) then
		Util:Sleep( 1000 );

--	ͣ���ڵ�¼���棬�ȴ�������Ϸ
--	Loger:AddLog("����Ask CharLogin...");
--	NetManager:SendMessage("CLAskCharLogin");

		Procedure_CharSel_SetState(STATE_WAIT_ASKCHARLOGIN);

	elseif(STATE_CURRENT == STATE_WAIT_ASKCHARLOGIN) then
		--�ȴ�ask_login����
		
	elseif(STATE_CURRENT == STATE_ENTER_SCENE) then
		--ѡ��һ��������볡��
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");
	
	elseif(STATE_CURRENT == STATE_ENTER_SCENE_ERROR) then
		Util:ThrowException("������½ʧ�� : " .. Variable:GetVariable("CharLogin_ErrorDesc"));
	end

end

function Procedure_CharSel_Release()

end
