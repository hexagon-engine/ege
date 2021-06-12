set(SOURCES
    "Sound.cpp"
    "Sound.h"
    "SoundManager.cpp"
    "SoundManager.h"
)

ege_add_module(sound)
ege_depend_module(sound core)
target_link_libraries(ege-sound PUBLIC sfml-audio)
