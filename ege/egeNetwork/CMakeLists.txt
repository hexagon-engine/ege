set(SOURCES
	"EGEClient.cpp"
	"EGEClient.h"
	"EGEClientConnection.cpp"
 	"EGEClientConnection.h"
	"EGEPacket.cpp"
	"EGEPacket.h"
	"EGEPacketConverter.cpp"
	"EGEPacketConverter.h"
	"EGEPacketGenerators.cpp"
	"EGEServer.cpp"
	"EGEServer.h"
)

ege_add_module(egeNetwork)
ege_depend_module(egeNetwork gui)
ege_depend_module(egeNetwork network)
ege_depend_module(egeNetwork scene)
ege_depend_module(egeNetwork util)
