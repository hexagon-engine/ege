set(SOURCES
	"SFML.cpp"
)

ege_add_module(sfml)
ege_link_sfml(sfml)
target_link_libraries(sfml PUBLIC dl)
