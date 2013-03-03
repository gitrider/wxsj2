--------------------------------------
-- Main流程
--------------------------------------
local	STATE_REQ_SCENE		= "request_scene";	--请求场景
local	STATE_LOOP				= "loop";

local STATE_CURRENT;
local	g_nTick						= 0;								--心跳

--设置状态
function Procedure_Main_SetState( stateName )
	STATE_CURRENT	= stateName;	
end

--------------------------------------
-- 游戏主流程
--------------------------------------
function Procedure_Main_Init()
	Loger:AddLog( "====进入Main流程====" );
end

function Procedure_Main_Tick()

	Util:Sleep( 10000 );
	if( STATE_CURRENT == STATE_REQ_SCENE ) then
		--请求环境
		Util:Sleep( 10 );
		NetManager:SendMessage( "CGEnvRequest" );
		Procedure_Main_SetState( STATE_LOOP );

	elseif( STATE_CURRENT == STATE_LOOP ) then
		Loger:AddLog( "Tick "..g_nTick );
		g_nTick			= g_nTick + 1;

	end

end

function Procedure_Main_Release()
end
