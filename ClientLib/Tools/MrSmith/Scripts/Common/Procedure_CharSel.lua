--------------------------------------
-- 选择角色流程
--------------------------------------
local STATE_ASKCHARLIST 				= "ask_char_list";				--发送askcharlist
local STATE_WAIT_ASKCHARLIST 			= "wait_ask_char_list";			--等待AskCharList回复
local STATE_HAVE_ASKCHARLIST 			= "char_list";					--有信息了
local STATE_HAVENOT_ASKCHARLIST			= "char_list_err";				--角色信息错误
local STATE_ASKCHARLOGIN				= "ask_char_login";				--发送askcharlogin
local STATE_WAIT_ASKCHARLOGIN			= "wait_ask_char_login";		--等待askcharlogin回复
local STATE_ENTER_SCENE					= "enter_scene";				--一切OK，可以直接进入场景了
local STATE_ENTER_SCENE_ERROR			= "enter_scene_error";			--进入场景错误

local STATE_CURRENT; --状态

--设置状态
function Procedure_CharSel_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_CharSel_Init()
	
end

function Procedure_CharSel_Tick()

	if(STATE_CURRENT == STATE_ASKCHARLIST) then
		--发送请求角色信息
		Util:Sleep( 1000 );
		Loger:AddLog("发送Ask CharList...");
		NetManager:SendMessage("CLAskCharList");
		Procedure_CharSel_SetState(STATE_WAIT_ASKCHARLIST);
		
	elseif(STATE_CURRENT == STATE_WAIT_ASKCHARLIST) then
		--等待ask回复
		
	elseif(STATE_CURRENT == STATE_HAVE_ASKCHARLIST) then
		local countList = LoginCharacterList:GetCharacterCounts();

		if( countList < 1 ) then
			--进入角色创建流程
			Loger:AddLog("进入角色创建流程..." .. countList);
			Procedure_CharCreate_SetState("create_char");
			Procedure:SetCurrentProcedure("CharCreate");
		else
			Procedure_CharSel_SetState(STATE_ASKCHARLOGIN);
		end
	
	elseif(STATE_CURRENT == STATE_HAVENOT_ASKCHARLIST) then
		local errmsg = Variable:GetVariable("CharListErrorDesc");
		Loger:AddLog("角色信息错误("..errmsg..")，重新进入ask_char_list过程...");
		Procedure_CharSel_SetState(STATE_ASKCHARLIST);

	elseif(STATE_CURRENT == STATE_ASKCHARLOGIN) then
		Util:Sleep( 1000 );

--	停留在登录界面，等待登入游戏
--	Loger:AddLog("发送Ask CharLogin...");
--	NetManager:SendMessage("CLAskCharLogin");

		Procedure_CharSel_SetState(STATE_WAIT_ASKCHARLOGIN);

	elseif(STATE_CURRENT == STATE_WAIT_ASKCHARLOGIN) then
		--等待ask_login返回
		
	elseif(STATE_CURRENT == STATE_ENTER_SCENE) then
		--选择一个人物，进入场景
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");
	
	elseif(STATE_CURRENT == STATE_ENTER_SCENE_ERROR) then
		Util:ThrowException("场景登陆失败 : " .. Variable:GetVariable("CharLogin_ErrorDesc"));
	end

end

function Procedure_CharSel_Release()

end
