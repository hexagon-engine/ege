set(SOURCES
	"ClientNetworkController.cpp"
	"ClientNetworkController.h"
	"EGEClient.cpp"
	"EGEClient.h"
	"EGEClientConnection.cpp"
 	"EGEClientConnection.h"
	"EGEGame.cpp"
	"EGEGame.h"
	"EGEPacket.cpp"
	"EGEPacket.h"
	"EGEPacketConverter.cpp"
	"EGEPacketConverter.h"
	"EGEPacketGenerators.cpp"
	"EGEServer.cpp"
	"EGEServer.h"
	"ServerNetworkController.cpp"
	"ServerNetworkController.h"
)

ege_add_module(egeNetwork)
ege_depend_module(egeNetwork controller)
ege_depend_module(egeNetwork debug)
ege_depend_module(egeNetwork gpo)
ege_depend_module(egeNetwork gui)
ege_depend_module(egeNetwork network)
ege_depend_module(egeNetwork scene)
ege_depend_module(egeNetwork util)
