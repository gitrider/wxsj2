local path = "D:\\tlbb_resource";


function Main()

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Brushes.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Brushes");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Config.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Config");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Effect.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Effect");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Interface.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Interface");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\LaunchSkin.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\LaunchSkin");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Material.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Material");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Model.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Model");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Scene.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Scene");

PakFile:clear();
PakFile:openPakFile("D:\\tlbb_resource\\Sound.axp",true);
PakFile:extractAllToPath("D:\\tlbb_resource\\Sound");

end