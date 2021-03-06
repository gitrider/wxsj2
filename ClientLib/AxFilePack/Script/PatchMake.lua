local g_newVersionPath  = "D:\\Test\\New";
local g_newVersion		= ""

local g_oldVersionPath	= "D:\\Test\\Old";
local g_oldVersion 		= ""


function Main()
	PatchMaker:clear();
	local temp;
	
	-----------------输入新版本目录
	io.write("输入新版本目录[" .. g_newVersionPath .. "]:");
	temp = io.read(); 
	if(temp ~= "") then
		g_newVersionPath = temp;
	end

	------------------预分析
	if(PatchMaker:analysePath(g_newVersionPath, "")) then
		g_newVersion = PatchMaker:getPathVersion(g_newVersionPath);
	else
		g_newVersion = "";
	end

	------------------输入新版本号
	io.write("输入新版本号[" .. g_newVersion .. "]:");
	temp = io.read();
	if(temp ~= "" and temp ~= g_newVersion) then
		g_newVersion = temp;
		if(not PatchMaker:analysePath(g_newVersionPath, g_newVersion)) then
			OnError();
			return;
		end
	end

	-----------------输入旧版本目录
	io.write("输入旧版本目录[" .. g_oldVersionPath .. "]:");
	temp = io.read(); 
	if(temp ~= "") then
		g_oldVersionPath = temp;
	end

	------------------预分析
	if(PatchMaker:analysePath(g_oldVersionPath, "")) then
		g_oldVersion = PatchMaker:getPathVersion(g_oldVersionPath);
	else
		g_oldVersion = "";
	end

	------------------输入旧版本号
	io.write("输入旧版本号[" .. g_oldVersion .. "]:");
	temp = io.read();
	if(temp ~= "" and temp ~= g_newVersion) then
		g_oldVersion = temp;
		if(not PatchMaker:analysePath(g_oldVersionPath, g_oldVersion)) then
			OnError();
			return;
		end
	end
	
	--生成补丁文件
	if(not PatchMaker:generatePatch(g_oldVersionPath, g_newVersionPath, "d:\\test.zip")) then
		OnError();
		return;
	end
	
	

end


function OnError()
	local errId, errDesc = getLastError();
	local strErr = "\nErr: " .. tostring(errId) .. "\nErrDesc: " .. errDesc;

	error(strErr);
end
