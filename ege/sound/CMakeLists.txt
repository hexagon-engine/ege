set(SOURCES
    "Sound.cpp"
    "Sound.h"
    "SoundManager.cpp"
    "SoundManager.h"
)

ege_add_module(sound)
ege_depend_module(sound core)
ege_depend_module(sound sfml)
