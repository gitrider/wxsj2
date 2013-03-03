CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (Billing)

ADD_DEFINITIONS(-D_FOX_BILLING)
ADD_DEFINITIONS(-D_FOX_LOGIN)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D__linux)
  ADD_DEFINITIONS(-D_REENTRANT)
  # ADD_DEFINITIONS(-DDONT_TD_VOID)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

##update_begin


INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\Base)
INCLUDE_DIRECTORIES(Main)
INCLUDE_DIRECTORIES(Packets)
INCLUDE_DIRECTORIES(..\\..\\Common\\DataBase)
INCLUDE_DIRECTORIES(..\\..\\Common\\Packets)
INCLUDE_DIRECTORIES(..\\..\\Common)


SET (BASE_LIST
	../../Server/Server/Base/Config.cpp
	../../Server/Server/Base/Config.h
	../../Server/Server/Base/File.cpp
	../../Server/Server/Base/File.h
	../../Server/Server/Base/Ini.cpp
	../../Server/Server/Base/Ini.h
	../../Server/Server/Base/Log.cpp
	../../Server/Server/Base/Log.h
	../../Server/Server/Base/StrTable.cpp
	../../Server/Server/Base/StrTable.h
	../../Server/Server/Base/Thread.cpp
	../../Server/Server/Base/Thread.h
	../../Server/Server/Base/TimeManager.cpp
	../../Server/Server/Base/TimeManager.h
)

SET (PACKETS_LIST
	Packets/BLRetAuthHandler.cpp
	Packets/LBAskAuthHandler.cpp
	Packets/SSConnectHandler.cpp
	Packets/SSScenePlayerCountHandler.cpp
)

SET (MAIN_LIST
	Main/AccountTable.cpp
	Main/AccountTable.h
	Main/Billing.cpp
	Main/Billing.h
	Main/BillingPlayer.cpp
	Main/BillingPlayer.h
	Main/Player.cpp
	Main/Player.h
	Main/PlayerManager.cpp
	Main/PlayerManager.h
	Main/PlayerPool.cpp
	Main/PlayerPool.h
	Main/ServerManager.cpp
	Main/ServerManager.h
	Main/ServerPlayer.cpp
	Main/ServerPlayer.h
	stdafx.cpp
	stdafx.h
)

SET (COMMON_PACKETS_LIST
	../../Common/Packets/BLRetAccCheck.cpp
	../../Common/Packets/BLRetAccCheck.h
	../../Common/Packets/BLRetAskBuy.cpp
	../../Common/Packets/BLRetAskBuy.h
	../../Common/Packets/BLRetAskPoint.cpp
	../../Common/Packets/BLRetAskPoint.h
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
	../../Common/Packets/BLRetPrize.cpp
	../../Common/Packets/BLRetPrize.h
	../../Common/Packets/BLRetRegPassPort.cpp
	../../Common/Packets/BLRetRegPassPort.h
	../../Common/Packets/LBAccCheck.cpp
	../../Common/Packets/LBAccCheck.h
	../../Common/Packets/LBAskAuth.cpp
	../../Common/Packets/LBAskAuth.h
	../../Common/Packets/LBAskBuy.cpp
	../../Common/Packets/LBAskBuy.h
	../../Common/Packets/LBAskPoint.cpp
	../../Common/Packets/LBAskPoint.h
	../../Common/Packets/LBAskRegAuth.cpp
	../../Common/Packets/LBAskRegAuth.h
	../../Common/Packets/LBAskRegLicense.cpp
	../../Common/Packets/LBAskRegLicense.h
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
	../../Common/Packets/LBLAskMig.cpp
	../../Common/Packets/LBLAskMig.h
	../../Common/Packets/LBLAskNewUserCard.cpp
	../../Common/Packets/LBLAskNewUserCard.h
	../../Common/Packets/LBLCostLog.cpp
	../../Common/Packets/LBLCostLog.h
	../../Common/Packets/LBLNewCheckPrize.cpp
	../../Common/Packets/LBLNewCheckPrize.h
	../../Common/Packets/LBLNewPrize.cpp
	../../Common/Packets/LBLNewPrize.h
	../../Common/Packets/LBLRegNewLicense.cpp
	../../Common/Packets/LBLRegNewLicense.h
	../../Common/Packets/LBPrize.cpp
	../../Common/Packets/LBPrize.h
	../../Common/Packets/LBRegPassPort.cpp
	../../Common/Packets/LBRegPassPort.h
	../../Common/Packets/SSConnect.cpp
	../../Common/Packets/SSConnect.h
	../../Common/Packets/SSScenePlayerCount.cpp
	../../Common/Packets/SSScenePlayerCount.h
)

SET (COMMON_DATABASE_LIST
	../../Common/DataBase/WXSJ_DBC.cpp
	../../Common/DataBase/WXSJ_DBC.h
)

SET (COMMON_LIST
	../../Common/Assertx.cpp
	../../Common/Assertx.h
	../../Common/BaseType.h
	../../Common/Chain.cpp
	../../Common/Chain.h
	../../Common/DB_Struct.cpp
	../../Common/DB_Struct.h
	../../Common/FileAPI.h
	../../Server/Server/Base/FoxWin32.h
	../../Common/GameDefine.h
	../../Common/GameDefine2.h
	../../Common/GameDefine_Attr.h
	../../Common/GameDefine_Guild.h
	../../Common/GameDefine_Item.h
	../../Common/GameDefine_Result.cpp
	../../Common/GameDefine_Result.h
	../../Common/GameDefine_Scene.h
	../../Common/GameStruct.cpp
	../../Common/GameStruct.h
	../../Common/GameStruct_Guild.h
	../../Common/GameStruct_Item.h
	../../Common/GameStruct_Relation.cpp
	../../Common/GameStruct_Relation.h
	../../Common/GameStruct_Skill.h
	../../Common/GameUtil.cpp
	../../Common/GameUtil.h
	../../Common/Minidump.cpp
	../../Common/Minidump.h
	../../Common/OResultDef.h
	../../Common/Packet.h
	../../Common/PacketDefine.h
	../../Common/PacketFactory.h
	../../Common/PacketFactoryManager.cpp
	../../Common/PacketFactoryManager.h
	../../Common/PlayerStatus.h
	../../Common/Rand.cpp
	../../Common/Rand.h
	../../Common/ScriptCommandStruct.cpp
	../../Common/ScriptCommandStruct.h
	../../Common/ServerSocket.h
	../../Common/Socket.h
	../../Common/SocketAPI.h
	../../Common/SocketInputStream.h
	../../Common/SocketOutputStream.h
	../../Common/Type.h
)



if (WIN32)
	source_group(Base FILES ${BASE_LIST})
	source_group(Packets FILES ${PACKETS_LIST})
	source_group(Main FILES ${MAIN_LIST})
	source_group(Common\\Packets FILES ${COMMON_PACKETS_LIST})
	source_group(Common\\DataBase FILES ${COMMON_DATABASE_LIST})
	source_group(Common FILES ${COMMON_LIST})
endif()


ADD_EXECUTABLE(Billing
	${BASE_LIST}
	${PACKETS_LIST}
	${MAIN_LIST}
	${COMMON_PACKETS_LIST}
	${COMMON_DATABASE_LIST}
	${COMMON_LIST}
)


##update_end

if (WIN32)
TARGET_LINK_LIBRARIES(Billing FoxNet)
else()
TARGET_LINK_LIBRARIES(Billing FoxNet pthread)
endif()

