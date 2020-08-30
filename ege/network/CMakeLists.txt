set(SOURCES
	"Client.cpp"
	"Client.h"
	"ClientConnection.cpp"
 	"ClientConnection.h"
	"NetworkEndpoint.cpp"
	"NetworkEndpoint.h"
	"Packet.cpp"
	"Packet.h"
	"Server.cpp"
	"Server.h"
)

ege_add_module(network)
ege_depend_module(network loop)
ege_depend_module(network sfml)
