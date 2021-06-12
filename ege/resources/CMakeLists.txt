set(SOURCES
	"AtlasTexture.cpp"
	"AtlasTexture.h"
	"ResourceManager.cpp"
	"ResourceManager.h"
	"SFMLTexture.cpp"
	"SFMLTexture.h"
	"Texture.cpp"
	"Texture.h"
)

ege_add_module(resources)
ege_depend_module(resources debug)
ege_depend_module(resources util)
target_link_libraries(ege-resources PUBLIC sfml-graphics)
