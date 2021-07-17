set(SOURCES
	"SFMLPacket.cpp"
	"SFMLPacket.h"
	"TcpClient.cpp"
	"TcpClient.h"
	"TcpClientConnection.cpp"
 	"TcpClientConnection.h"
	"TcpEndpoint.cpp"
	"TcpEndpoint.h"
	"TcpPacket.cpp"
	"TcpPacket.h"
	"TcpServer.cpp"
	"TcpServer.h"
)

ege_add_module(network)
ege_depend_module(network debug)
ege_depend_module(network core)
target_link_libraries(ege-network PUBLIC sfml-network)

