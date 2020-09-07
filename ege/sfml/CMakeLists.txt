set(SOURCES
	"SFML.cpp"
)

ege_add_module(sfml)
ege_link_sfml(ege-sfml)
target_link_libraries(ege-sfml PUBLIC dl)
