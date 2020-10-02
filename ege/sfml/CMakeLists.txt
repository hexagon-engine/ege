set(SOURCES
	"SFML.cpp"
)

ege_add_module(sfml)
ege_link_sfml(ege-sfml)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  target_link_libraries(ege-sfml PUBLIC dl)
endif()
