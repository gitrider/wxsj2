

local Media_Path_table = 
{
	"../Media/",
	"../Media/overlays",
	"../Media/EffectSystem",
	"../Media/ProjTex",
	"../Media/PostFilter",
	"../Media/Water",
	"../Media/Skybox",
	"*../Media/模型",
	"../Media/人物/水鬼",
	"../Media/人物/鬼面人",
	"../Media/人物/蜘蛛怪",
	"../Media/人物/藤甲贼",
	"../Media/人物/扬子鳄",
	"../Media/人物/断桥花妖",
	"../Media/人物/旱船",
	"../Media/人物/白堤柳鬼",
	"../Media/人物/石狮子",
	"../Media/人物/女主角",
	"../Media/人物/男主角",
	"../Media/人物/女主角装备",
	"../Media/人物/长剑",
	"../Media/人物/法杖",
	"../Media/人物/Npc",
	"../Media/人物/残破兵俑",
	"../Media/人物/稻草人",
	"../Media/人物/秦家寨探子",
	"../Media/人物/山蛛",
	"../Media/人物/蜀道黑猿",
	"../Media/人物/狼",
	"../Media/人物/莽盖",
	"../Media/人物/棋魂",
	"../Media/人物/神汉",
	"../Media/人物/石人",
	"../Media/人物/邪剑仙",
	"../Media/人物/熊",
	"../Media/人物/野猿",
	"../Media/人物/豹",
	"../Media/人物/冰蚕",
	"../Media/人物/鳄鱼怪",
	"../Media/人物/狼人",
	"../Media/人物/美人鱼",
	"../Media/人物/木人",
	"../Media/人物/铜人",
	"../Media/人物/修罗",
	"../Media/人物/雪人",
	"../Media/人物/药篓",
	"../Media/人物/山鬼",
	"../Media/人物/马匪",
	"../Media/人物/黑蜂",
	"../Media/人物/珍兽",
	"../Media/人物/骑乘",
};

local Sound_path_table = 
{	
	"*../Sound",
};

local Interface_path_table =
{
	"*../Interface",
};

local PublicLua_path_table = 
{
	"*../Config",
	"*../../Public/Config",
	"*../../Public/Data/Script",
	"../../Public/Data",
};

local Brushes_group_table = 
{
	"*../Brushes"
};

local Scene_path_table =
{
	"../../Public/Scene",
	"../../Public/Scene/Schema"
};

function Main()

	print( "");
	print( "1:Model" );
	print( "2:Material" );
	print( "3:Effect" );
	print( "4:Interface" );
	print( "5:Sound" );
	print( "6:Scene" );
	print( "7:Brushes" );
	print( "8:Config" );
	print( "a:All" );
	print( "");
	io.write( "输入需要打包的编号:");
	local sel = io.read();
	sel = "a";
	
	local groupSel = {0, 0, 0, 0, 0, 0, 0, 0};
	
	if(sel == "a") then
		groupSel[1] = 1;
		groupSel[2] = 1;
		groupSel[3] = 1;
		groupSel[4] = 1;
		groupSel[5] = 1;
		groupSel[6] = 1;
		groupSel[7] = 1;
		groupSel[8] = 1;
	elseif(tonumber(sel)~=nil and (tonumber(sel) >= 1 and tonumber(sel) <= 8)) then
		groupSel[tonumber(sel)] = 1;
	end

	-----------------------
	--		Model		---
	-----------------------
	if(	groupSel[1] == 1) then
		PakMaker:moveSkeleton("../Media/模型");
		pakPath("Model", {Media_Path_table}, "mesh;model;skeleton;obj;frame", "stand");
	end
	
	-----------------------
	--		Material	---
	-----------------------
	if(	groupSel[2] == 1) then
		pakPath("Material", {Media_Path_table, Interface_path_table}, 
			"material;cg;asm;program;overlay;bmp;dib;cut;dcx;dds;gif;hdr;ico;cur;jpg;jpe;jpeg;lif;mdl;mng;jng;pcx;pic;pix;png;pbm;pgm;pnm;ppm;psd;pdd;psp;pxr;sgi;bw;rgb;rgba;tga;vda;icb;vst;tif;tiff;wal;xpm;raw", 
			"stand");
	end
	
	-----------------------
	--		Effect		---
	-----------------------
	if(	groupSel[3] == 1) then
		pakPath("Effect", {Media_Path_table}, "particle;effect;skill", "stand");
	end
	
	-----------------------
	--	  Interface		---
	-----------------------
	if(	groupSel[4] == 1) then
		pakPath("Interface", {Interface_path_table}, "xml;lua;ttf;font;shema;bin;imageset", "stand");
	end
	
	-----------------------
	--	   Sound		---
	-----------------------
	if(	groupSel[5] == 1) then
		pakPath("Sound", {Sound_path_table}, "wav;mp2;mp3;ogg;raw;mod;s3m;xm;it;mid", "stand");
	end
	
	-----------------------
	--	    Scene		---
	-----------------------
	if(	groupSel[6] == 1) then
		pakPath("Scene", {Scene_path_table}, 
			"jpg;dds;esd;gridinfo;heightmap;png;region;scene;preload;terrain;wcollision;xsd;xml;material;cg;program", "stand");
	end
	
	-----------------------
	--	    Brushes		---
	-----------------------
	if(	groupSel[7] == 1) then
		pakPath("Brushes", {Brushes_group_table}, 
			"bmp;dib;cut;dcx;dds;gif;hdr;ico;cur;jpg;jpe;jpeg;lif;mdl;mng;jng;pcx;pic;pix;png;pbm;pgm;pnm;ppm;psd;pdd;psp;pxr;sgi;bw;rgb;rgba;tga;vda;icb;vst;tif;tiff;wal;xpm;raw"
			, "stand");
	end
	
	-----------------------
	--	    Config		---
	-----------------------
	if(	groupSel[8] == 1) then
		pakPath("Config", {PublicLua_path_table}, "dat;txt;lua", "config");
	end
	
end

---------------------------------------------------
--pakNormal path
function pakPath(pakName, pathTable, extFilter, callBack)
	PakMaker:clear();

	local tblPathGroup;
	for _, tblPathGroup in ipairs(pathTable) do
		local strPath="";
		for _, strPath in ipairs(tblPathGroup) do
			local bRecursive = string.sub(strPath, 1, 1) == "*";
			if bRecursive then strPath = string.sub(strPath, 2, -1) end
			
			if( not PakMaker:addDiskFold(strPath, "", extFilter, bRecursive)) then
				OnError();
				return;
			end
		end
	end
		
	if( not PakMaker:savePakFile(pakName .. ".axp", callBack)) then
		OnError();
		return;
	end
end

function OnError()
	local errId, errDesc = getLastError();
	local strErr = "\nErr: " .. tostring(errId) .. "\nErrDesc: " .. errDesc;

	error(strErr);
end