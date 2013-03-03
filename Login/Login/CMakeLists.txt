CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (Login)

ADD_DEFINITIONS(-D_FOX_LOGIN)
ADD_DEFINITIONS(-D_FOX_BILLING)
ADD_DEFINITIONS(-D_FOX_CLIENT)
ADD_DEFINITIONS(-D_FOX_WORLD)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ADD_DEFINITIONS(-D_USE_32BIT_TIME_T)


IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D_REENTRANT)
  ADD_DEFINITIONS(-DDONT_TD_VOID)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

##update_begin


INCLUDE_DIRECTORIES(..\\..\\Common\\Packets)
INCLUDE_DIRECTORIES(DB)
INCLUDE_DIRECTORIES(..\\..\\Common)
INCLUDE_DIRECTORIES(Player)
INCLUDE_DIRECTORIES(..\\..\\Common\\DataBase)
INCLUDE_DIRECTORIES(Process)
INCLUDE_DIRECTORIES(Packets)
INCLUDE_DIRECTORIES(..\\..\\Common\\DBSystem\\DataBase)
INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\Base)
INCLUDE_DIRECTORIES(Main)


SET (MAIN_LIST
	Main/ConnectManager.cpp
	Main/ConnectManager.h
	Main/IpTable.cpp
	Main/IpTable.h
	Main/Login.cpp
	Main/Login.h
	Main/LoginMain.cpp
	Main/LoginMain.h
	Main/LoginTable.cpp
	Main/LoginTable.h
	Main/MiBao.cpp
	Main/MiBao.h
	Main/ReLogin.cpp
	Main/ReLogin.h
	Main/ServerManager.cpp
	Main/ServerManager.h
	Main/ServerThread.cpp
	Main/ServerThread.h
	stdafx.cpp
	stdafx.h
	Main/ThreadManager.cpp
	Main/ThreadManager.h
)

SET (BASE_LIST
	../../Server/Server/Base/Config.cpp
	../../Server/Server/Base/Config.h
	../../Server/Server/Base/FileDef.h
	../../Server/Server/Base/Ini.cpp
	../../Server/Server/Base/Ini.h
	../../Server/Server/Base/Log.cpp
	../../Server/Server/Base/Log.h
	../../Server/Server/Base/LogDefine.cpp
	../../Server/Server/Base/LogDefine.h
	../../Server/Server/Base/StrTable.cpp
	../../Server/Server/Base/StrTable.h
	../../Server/Server/Base/Thread.cpp
	../../Server/Server/Base/Thread.h
	../../Server/Server/Base/TimeManager.cpp
	../../Server/Server/Base/TimeManager.h
)

SET (COMMON_PACKETS_PRIZE_LIST
	../../Common/Packets/BLRetAskBuy.cpp
	../../Common/Packets/BLRetAskBuy.h
	../../Common/Packets/BLRetAskPoint.cpp
	../../Common/Packets/BLRetAskPoint.h
	../../Common/Packets/BLRetPrize.cpp
	../../Common/Packets/BLRetPrize.h
	../../Common/Packets/LBAskBuy.cpp
	../../Common/Packets/LBAskBuy.h
	../../Common/Packets/LBAskPoint.cpp
	../../Common/Packets/LBAskPoint.h
	../../Common/Packets/LBLAskNewUserCard.cpp
	../../Common/Packets/LBLAskNewUserCard.h
	../../Common/Packets/LBLNewCheckPrize.cpp
	../../Common/Packets/LBLNewCheckPrize.h
	../../Common/Packets/LBLNewPrize.cpp
	../../Common/Packets/LBLNewPrize.h
	../../Common/Packets/LBPrize.cpp
	../../Common/Packets/LBPrize.h
	../../Common/Packets/LWRetPrize.cpp
	../../Common/Packets/LWRetPrize.h
	../../Common/Packets/WLAskPrize.cpp
	../../Common/Packets/WLAskPrize.h
)

SET (COMMON_PACKETS_KICK_LIST
	../../Common/Packets/LWNotifyUser.cpp
	../../Common/Packets/LWNotifyUser.h
	../../Common/Packets/WLNotifyUser.cpp
	../../Common/Packets/WLNotifyUser.h
)

SET (COMMON_PACKETS_COSTLOG_LIST
	../../Common/Packets/GWLWGCostLog.cpp
	../../Common/Packets/GWLWGCostLog.h
	../../Common/Packets/LBLCostLog.cpp
	../../Common/Packets/LBLCostLog.h
)

SET (COMMON_PACKETS_MIG_LIST
	../../Common/Packets/LBLAskMig.cpp
	../../Common/Packets/LBLAskMig.h
)

SET (COMMON_PACKETS_CHANGENAME_LIST
	../../Common/Packets/LWChangeName.cpp
	../../Common/Packets/LWChangeName.h
	../../Common/Packets/WLChangeName.cpp
	../../Common/Packets/WLChangeName.h
)

SET (COMMON_PACKETS_CREATECODE_LIST
	../../Common/Packets/CLAskCreateCode.cpp
	../../Common/Packets/CLAskCreateCode.h
	../../Common/Packets/LCRetCreateCode.cpp
	../../Common/Packets/LCRetCreateCode.h
)

SET (COMMON_PACKETS_NEWLICENSE_LIST
	../../Common/Packets/CLRegNewLicense.cpp
	../../Common/Packets/CLRegNewLicense.h
	../../Common/Packets/LBAskRegLicense.cpp
	../../Common/Packets/LBAskRegLicense.h
	../../Common/Packets/LBLRegNewLicense.cpp
	../../Common/Packets/LBLRegNewLicense.h
)

SET (COMMON_PACKETS_SAFESIGN_LIST
	../../Common/Packets/CLRegSafeSign.cpp
	../../Common/Packets/CLRegSafeSign.h
)

SET (COMMON_PACKETS_LIST
	../../Common/Packets/BLRetAccCheck.cpp
	../../Common/Packets/BLRetAccCheck.h
	../../Common/Packets/BLRetAuth.cpp
	../../Common/Packets/BLRetAuth.h
	../../Common/Packets/BLRetBillingEnd.cpp
	../../Common/Packets/BLRetBillingEnd.h
	../../Common/Packets/BLRetBillingKeep.cpp
	../../Common/Packets/BLRetBillingKeep.h
	../../Common/Packets/BLRetBillingStart.cpp
	../../Common/Packets/BLRetBillingStart.h
	../../Common/Packets/BLRetConnect.cpp
	../../Common/Packets/BLRetConnect.h
	../../Common/Packets/BLRetKeepLive.cpp
	../../Common/Packets/BLRetKeepLive.h
	../../Common/Packets/BLRetKickAll.cpp
	../../Common/Packets/BLRetKickAll.h
	../../Common/Packets/BLRetRegPassPort.cpp
	../../Common/Packets/BLRetRegPassPort.h
	../../Common/Packets/CLAskCharList.cpp
	../../Common/Packets/CLAskCharList.h
	../../Common/Packets/CLAskCharLogin.cpp
	../../Common/Packets/CLAskCharLogin.h
	../../Common/Packets/CLAskCreateChar.cpp
	../../Common/Packets/CLAskCreateChar.h
	../../Common/Packets/CLAskDeleteChar.cpp
	../../Common/Packets/CLAskDeleteChar.h
	../../Common/Packets/CLAskLogin.cpp
	../../Common/Packets/CLAskLogin.h
	../../Common/Packets/CLAskLoginCode.cpp
	../../Common/Packets/CLAskLoginCode.h
	../../Common/Packets/CLAskRegPassPort.cpp
	../../Common/Packets/CLAskRegPassPort.h
	../../Common/Packets/CLConnect.cpp
	../../Common/Packets/CLConnect.h
	../../Common/Packets/KeyExchange.cpp
	../../Common/Packets/KeyExchange.h
	../../Common/Packets/LBAccCheck.cpp
	../../Common/Packets/LBAccCheck.h
	../../Common/Packets/LBAskAuth.cpp
	../../Common/Packets/LBAskAuth.h
	../../Common/Packets/LBAskRegAuth.cpp
	../../Common/Packets/LBAskRegAuth.h
	../../Common/Packets/LBBillingEnd.cpp
	../../Common/Packets/LBBillingEnd.h
	../../Common/Packets/LBBillingKeep.cpp
	../../Common/Packets/LBBillingKeep.h
	../../Common/Packets/LBBillingStart.cpp
	../../Common/Packets/LBBillingStart.h
	../../Common/Packets/LBConnect.cpp
	../../Common/Packets/LBConnect.h
	../../Common/Packets/LBKeepLive.cpp
	../../Common/Packets/LBKeepLive.h
	../../Common/Packets/LBKickAll.cpp
	../../Common/Packets/LBKickAll.h
	../../Common/Packets/LBRegPassPort.cpp
	../../Common/Packets/LBRegPassPort.h
	../../Common/Packets/LCRetCharList.cpp
	../../Common/Packets/LCRetCharList.h
	../../Common/Packets/LCRetCharLogin.cpp
	../../Common/Packets/LCRetCharLogin.h
	../../Common/Packets/LCRetConnect.cpp
	../../Common/Packets/LCRetConnect.h
	../../Common/Packets/LCRetCreateChar.cpp
	../../Common/Packets/LCRetCreateChar.h
	../../Common/Packets/LCRetDeleteChar.cpp
	../../Common/Packets/LCRetDeleteChar.h
	../../Common/Packets/LCRetLogin.cpp
	../../Common/Packets/LCRetLogin.h
	../../Common/Packets/LCRetLoginCode.cpp
	../../Common/Packets/LCRetLoginCode.h
	../../Common/Packets/LCStatus.cpp
	../../Common/Packets/LCStatus.h
	../../Common/Packets/LWAskCharLogin.cpp
	../../Common/Packets/LWAskCharLogin.h
	../../Common/Packets/LWRetBillingEnd.cpp
	../../Common/Packets/LWRetBillingEnd.h
	../../Common/Packets/LWRetBillingKeep.cpp
	../../Common/Packets/LWRetBillingKeep.h
	../../Common/Packets/LWRetBillingStart.cpp
	../../Common/Packets/LWRetBillingStart.h
	../../Common/Packets/SSConnect.cpp
	../../Common/Packets/SSConnect.h
	../../Common/Packets/SSScenePlayerCount.cpp
	../../Common/Packets/SSScenePlayerCount.h
	../../Common/Packets/WLBillingEnd.cpp
	../../Common/Packets/WLBillingEnd.h
	../../Common/Packets/WLBillingKeep.cpp
	../../Common/Packets/WLBillingKeep.h
	../../Common/Packets/WLBillingKick.cpp
	../../Common/Packets/WLBillingKick.h
	../../Common/Packets/WLBillingStart.cpp
	../../Common/Packets/WLBillingStart.h
	../../Common/Packets/WLRetCharLogin.cpp
	../../Common/Packets/WLRetCharLogin.h
)

SET (COMMON_DATABASE_LIST
	../../Common/DBSystem/DataBase/DBAbilityList.cpp
	../../Common/DBSystem/DataBase/DBAbilityList.h
	../../Common/DBSystem/DataBase/DBCharCreate.cpp
	../../Common/DBSystem/DataBase/DBCharCreate.h
	../../Common/DBSystem/DataBase/DBChardbVersion.cpp
	../../Common/DBSystem/DataBase/DBChardbVersion.h
	../../Common/DBSystem/DataBase/DBCharExtradbVersion.cpp
	../../Common/DBSystem/DataBase/DBCharExtradbVersion.h
	../../Common/DBSystem/DataBase/DBCharExtraInfo.cpp
	../../Common/DBSystem/DataBase/DBCharExtraInfo.h
	../../Common/DBSystem/DataBase/DBCharFullData.cpp
	../../Common/DBSystem/DataBase/DBCharFullData.h
	../../Common/DBSystem/DataBase/DBCharList.cpp
	../../Common/DBSystem/DataBase/DBCharList.h
	../../Common/DBSystem/DataBase/DBCharVisualGem.cpp
	../../Common/DBSystem/DataBase/DBCharVisualGem.h
	../../Common/DBSystem/DataBase/DBCheckCharName.cpp
	../../Common/DBSystem/DataBase/DBCheckCharName.h
	../../Common/DBSystem/DataBase/DBClearDupItemList.cpp
	../../Common/DBSystem/DataBase/DBClearDupItemList.h
	../../Common/DBSystem/DataBase/DBCoolDownInfo.cpp
	../../Common/DBSystem/DataBase/DBCoolDownInfo.h
	../../Common/DBSystem/DataBase/DBCrcInfo.cpp
	../../Common/DBSystem/DataBase/DBCrcInfo.h
	../../Common/DBSystem/DataBase/DBCreateCharOp.cpp
	../../Common/DBSystem/DataBase/DBCreateCharOp.h
	../../Common/DBSystem/DataBase/DBDeleteCharOp.cpp
	../../Common/DBSystem/DataBase/DBDeleteCharOp.h
	../../Common/DBSystem/DataBase/DBEquipBaseList.cpp
	../../Common/DBSystem/DataBase/DBEquipBaseList.h
	../../Common/DBSystem/DataBase/DBFatigueInfo.cpp
	../../Common/DBSystem/DataBase/DBFatigueInfo.h
	../../Common/DBSystem/DataBase/DBImmigration.cpp
	../../Common/DBSystem/DataBase/DBImmigration.h
	../../Common/DBSystem/DataBase/DBImpactList.cpp
	../../Common/DBSystem/DataBase/DBImpactList.h
	../../Common/DBSystem/DataBase/DBItemList.cpp
	../../Common/DBSystem/DataBase/DBItemList.h
	../../Common/DBSystem/DataBase/DBManager.cpp
	../../Common/DBSystem/DataBase/DBManager.h
	../../Common/DBSystem/DataBase/DBPetList.cpp
	../../Common/DBSystem/DataBase/DBPetList.h
	../../Common/DBSystem/DataBase/DBPrivateInfo.cpp
	../../Common/DBSystem/DataBase/DBPrivateInfo.h
	../../Common/DBSystem/DataBase/DBPvPInfo.cpp
	../../Common/DBSystem/DataBase/DBPvPInfo.h
	../../Common/DBSystem/DataBase/DBRelationList.cpp
	../../Common/DBSystem/DataBase/DBRelationList.h
	../../Common/DBSystem/DataBase/DBSkillList.cpp
	../../Common/DBSystem/DataBase/DBSkillList.h
	../../Common/DBSystem/DataBase/DBSouXiaList.cpp
	../../Common/DBSystem/DataBase/DBSouXiaList.h
	../../Common/DBSystem/DataBase/DBTaskList.cpp
	../../Common/DBSystem/DataBase/DBTaskList.h
	../../Common/DBSystem/DataBase/DBTitleInfo.cpp
	../../Common/DBSystem/DataBase/DBTitleInfo.h
	../../Common/DBSystem/DataBase/DBTypes.h
	../../Common/DBSystem/DataBase/DBXinFaList.cpp
	../../Common/DBSystem/DataBase/DBXinFaList.h
	../../Common/DBSystem/DataBase/ODBCBase.cpp
	../../Common/DBSystem/DataBase/ODBCBase.h
	../../Common/DBSystem/DataBase/ODBCInterface.cpp
	../../Common/DBSystem/DataBase/ODBCInterface.h
	../../Common/DBSystem/DataBase/SqlTemplate.cpp
	../../Common/DBSystem/DataBase/SqlTemplate.h
	../../Common/DataBase/WXSJ_DBC.cpp
	../../Common/DataBase/WXSJ_DBC.h
)

SET (COMMON_LIST
	../../Common/Assertx.cpp
	../../Common/Assertx.h
	../../Common/DB_Struct.cpp
	../../Common/DB_Struct.h
	../../Common/FoxEncrypt.cpp
	../../Common/FoxEncrypt.h
	../../Common/GameStruct.cpp
	../../Common/GameStruct.h
	../../Common/GameUtil.cpp
	../../Common/GameUtil.h
	../../Common/Minidump.cpp
	../../Common/Minidump.h
	../../Common/Packet.h
	../../Common/PacketDefine.h
	../../Common/PacketFactory.h
	../../Common/PacketFactoryManager.cpp
	../../Common/PacketFactoryManager.h
	../../Common/PacketStruct.cpp
	../../Common/PacketStruct.h
	../../Common/PlayerStatus.h
	../../Common/ResultDefine.h
	../../Common/ServerSocket.h
	../../Common/Socket.h
	../../Common/SocketAPI.h
	../../Common/SocketBillingInputStream.h
	../../Common/SocketBillingOutputStream.h
	../../Common/SocketInputStream.h
	../../Common/SocketOutputStream.h
	../../Common/Type.h
)

SET (PLAYER_LIST
	Player/BillingPlayer.cpp
	Player/BillingPlayer.h
	Player/LoginPlayer.cpp
	Player/LoginPlayer.h
	Player/LoginPlayerManager.cpp
	Player/LoginPlayerManager.h
	Player/Player.cpp
	Player/Player.h
	Player/PlayerManager.cpp
	Player/PlayerManager.h
	Player/PlayerPool.cpp
	Player/PlayerPool.h
	Player/ServerPlayer.cpp
	Player/ServerPlayer.h
	Player/TurnPlayerQueue.cpp
	Player/TurnPlayerQueue.h
)

SET (PACKETS_PRIZEHANDLER_LIST
	Packets/BLRetAskBuyHandler.cpp
	Packets/BLRetAskPointHandler.cpp
	Packets/BLRetPrizeHandler.cpp
	Packets/LBAskBuyHandler.cpp
	Packets/LBAskPointHandler.cpp
	Packets/LBLAskNewUserCardHandler.cpp
	Packets/LBLNewCheckPrizeHandler.cpp
	Packets/LBLNewPrizeHandler.cpp
	Packets/LBPrizeHandler.cpp
	Packets/LWRetPrizeHandler.cpp
	Packets/WLAskPrizeHandler.cpp
)

SET (PACKETS_COSTLOGHANDLER_LIST
	Packets/GWLWGCostLogHandler.cpp
	Packets/LBLCostLogHandler.cpp
)

SET (PACKETS_MIGHANDLER_LIST
	Packets/LBLAskMigHandler.cpp
)

SET (PACKETS_CHANGENAMEHANDLER_LIST
	Packets/LWChangeNameHandler.cpp
	Packets/WLChangeNameHandler.cpp
)

SET (PACKETS_REGLICENSE_LIST
	Packets/CLRegNewLicenseHandler.cpp
	Packets/LBAskRegLicenseHandler.cpp
	Packets/LBLRegNewLicenseHandler.cpp
)

SET (PACKETS_REGSAFESIGN_LIST
	Packets/CLRegSafeSignHandler.cpp
)

SET (PACKETS_LIST
	Packets/BLRetAccCheckHandler.cpp
	Packets/BLRetAuthHandler.cpp
	Packets/BLRetBillingEndHandler.cpp
	Packets/BLRetBillingKeepHandler.cpp
	Packets/BLRetBillingStartHandler.cpp
	Packets/BLRetConnectHandler.cpp
	Packets/BLRetKeepLiveHandler.cpp
	Packets/BLRetKickAllHandler.cpp
	Packets/BLRetRegPassPortHandler.cpp
	Packets/CLAskCharListHandler.cpp
	Packets/CLAskCharLoginHandler.cpp
	Packets/CLAskCreateCharHander.cpp
	Packets/CLAskCreateCodeHandler.cpp
	Packets/CLAskDeleteCharHandler.cpp
	Packets/CLAskLoginCodeHandler.cpp
	Packets/CLAskLoginHandler.cpp
	Packets/CLAskRegPassPortHandler.cpp
	Packets/CLConnectHandler.cpp
	Packets/KeyExchangeHandler.cpp
	Packets/LBAccCheckHandler.cpp
	Packets/LBAskAuthHandler.cpp
	Packets/LBAskRegAuthHandler.cpp
	Packets/LBBillingEndHandler.cpp
	Packets/LBBillingKeepHandler.cpp
	Packets/LBBilLingStartHandler.cpp
	Packets/LBConnectHandler.cpp
	Packets/LBKeepLiveHandler.cpp
	Packets/LBKickAllHandler.cpp
	Packets/LBRegPassPortHandler.cpp
	Packets/LCRetCharListHandler.cpp
	Packets/LCRetCharLoginHandler.cpp
	Packets/LCRetConnectHandler.cpp
	Packets/LCRetCreateCharHandler.cpp
	Packets/LCRetCreateCodeHandler.cpp
	Packets/LCRetDeleteCharHandler.cpp
	Packets/LCRetLoginCodeHandler.cpp
	Packets/LCRetLoginHandler.cpp
	Packets/LCStatusHandler.cpp
	Packets/LWAskCharLoginHandler.cpp
	Packets/LWNotifyUserHandler.cpp
	Packets/LWRetBillingEndHandler.cpp
	Packets/LWRetBillingKeepHandler.cpp
	Packets/LWRetBillingStartHandler.cpp
	Packets/SSConnectHandler.cpp
	Packets/SSScenePlayerCountHandler.cpp
	Packets/WLBillingEndHandler.cpp
	Packets/WLBillingKeepHandler.cpp
	Packets/WLBillingKickHandler.cpp
	Packets/WLBillingStartHandler.cpp
	Packets/WLNotifyUserHandler.cpp
	Packets/WLRetCharLoginHandler.cpp
)

SET (DATABASE_LIST
	DB/CharConfig.cpp
	DB/CharConfig.h
	DB/DBLogicManager.cpp
	DB/DBLogicManager.h
	DB/DBThread.cpp
	DB/DBThread.h
	DB/DBThreadManager.cpp
	DB/DBThreadManager.h
	DB/LoginDBManager.cpp
	DB/LoginDBManager.h
)

SET (PROCESS_LIST
	Process/ProcessManager.cpp
	Process/ProcessManager.h
	Process/ProcessPlayerManager.cpp
	Process/ProcessPlayerManager.h
)



if (WIN32)
	source_group(Main FILES ${MAIN_LIST})
	source_group(Base FILES ${BASE_LIST})
	source_group(Common\\Packets\\Prize FILES ${COMMON_PACKETS_PRIZE_LIST})
	source_group(Common\\Packets\\Kick FILES ${COMMON_PACKETS_KICK_LIST})
	source_group(Common\\Packets\\CostLog FILES ${COMMON_PACKETS_COSTLOG_LIST})
	source_group(Common\\Packets\\Mig FILES ${COMMON_PACKETS_MIG_LIST})
	source_group(Common\\Packets\\ChangeName FILES ${COMMON_PACKETS_CHANGENAME_LIST})
	source_group(Common\\Packets\\CreateCode FILES ${COMMON_PACKETS_CREATECODE_LIST})
	source_group(Common\\Packets\\NewLicense FILES ${COMMON_PACKETS_NEWLICENSE_LIST})
	source_group(Common\\Packets\\SafeSign FILES ${COMMON_PACKETS_SAFESIGN_LIST})
	source_group(Common\\Packets FILES ${COMMON_PACKETS_LIST})
	source_group(Common\\DataBase FILES ${COMMON_DATABASE_LIST})
	source_group(Common FILES ${COMMON_LIST})
	source_group(Player FILES ${PLAYER_LIST})
	source_group(Packets\\PrizeHandler FILES ${PACKETS_PRIZEHANDLER_LIST})
	source_group(Packets\\CostLogHandler FILES ${PACKETS_COSTLOGHANDLER_LIST})
	source_group(Packets\\MigHandler FILES ${PACKETS_MIGHANDLER_LIST})
	source_group(Packets\\ChangeNameHandler FILES ${PACKETS_CHANGENAMEHANDLER_LIST})
	source_group(Packets\\RegLicense FILES ${PACKETS_REGLICENSE_LIST})
	source_group(Packets\\RegSafeSign FILES ${PACKETS_REGSAFESIGN_LIST})
	source_group(Packets FILES ${PACKETS_LIST})
	source_group(Database FILES ${DATABASE_LIST})
	source_group(Process FILES ${PROCESS_LIST})
endif()


ADD_EXECUTABLE(Login
	${MAIN_LIST}
	${BASE_LIST}
	${COMMON_PACKETS_PRIZE_LIST}
	${COMMON_PACKETS_KICK_LIST}
	${COMMON_PACKETS_COSTLOG_LIST}
	${COMMON_PACKETS_MIG_LIST}
	${COMMON_PACKETS_CHANGENAME_LIST}
	${COMMON_PACKETS_CREATECODE_LIST}
	${COMMON_PACKETS_NEWLICENSE_LIST}
	${COMMON_PACKETS_SAFESIGN_LIST}
	${COMMON_PACKETS_LIST}
	${COMMON_DATABASE_LIST}
	${COMMON_LIST}
	${PLAYER_LIST}
	${PACKETS_PRIZEHANDLER_LIST}
	${PACKETS_COSTLOGHANDLER_LIST}
	${PACKETS_MIGHANDLER_LIST}
	${PACKETS_CHANGENAMEHANDLER_LIST}
	${PACKETS_REGLICENSE_LIST}
	${PACKETS_REGSAFESIGN_LIST}
	${PACKETS_LIST}
	${DATABASE_LIST}
	${PROCESS_LIST}
)


##update_end

if (WIN32)
TARGET_LINK_LIBRARIES(Login FoxNet ws2_32.lib odbc32.lib odbccp32.lib)
else()
TARGET_LINK_LIBRARIES(Login FoxNet pthread odbc)
endif()
