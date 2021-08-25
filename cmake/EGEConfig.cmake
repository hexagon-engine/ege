get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/EGEUtils.cmake)

# TODO: Find out a way to don't use relative paths here!
set(SFML_CONFIG_ROOT "${SELF_DIR}/../../SFML/build/root/lib/cmake/SFML")
ege_message("INFO" "SFML_CONFIG_ROOT = ${SFML_CONFIG_ROOT}")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SFML_CONFIG_ROOT})
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 COMPONENTS network audio graphics window system REQUIRED)

#https://www.foonathan.net/2016/03/cmake-install/
include(${SELF_DIR}/ege.cmake)
