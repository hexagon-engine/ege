## Full module/feature list
* **controller** - API used for synchronizing scenes over the network
* **core** - Component and basic utilities
    * Hierarchy system based on Components
    * Event loop
    * Timers & clocks
    * Async tasks + `Progress` class for splash screens
    * Data Manager for managing game settings
* **debug** - Debug utility
    * EGE custom logger
    * Configurable hex dump
    * `util`'s Object printing
    * Object inspector (allows to see when objects are created/removed)
    * Profiler
* **egeNetwork** - Protocol for network games
    * `scene` synchronizing
    * Login system (not encrypted for now)
    * API for controlling any object
* **event** - System (window) event handlers
    * SFML system window wrapper with Component compatible API
    * Default system event handling (e.g close app on Close)
    * Keybind Manager, which allows easier management of controls
* **game** - Game class with Gameplay Object Manager and Game Loop included
* **geometry** - Geometry utility (computing intersections etc.)
    * It's very WIP for now
* **gfx** - Graphics renderer
    * Basic shape rendering (rectangles, texts, points etc.)
    * Theme renderer
* **gpo** - "Gameplay Object" Manager
    * *Gameplay Objects* - Objects that can be used in game (e.g entity types) with specified **base** (usually string) and **numeric** ID.
    * Mainly *GameplayObjectRegistry* - a structure that manages and automatically assigns numeric IDs to registered objects.
* **gui** - User interface utility
    * GUI animations with many easing functions
    * Basic widgets (Button, CheckBox, Frame, Label, RadioButton, ScrollBar, TextBox, Slider, ProgressBar, ListBox) and modal dialogs
    * Simple layout calculation
    * Splash screens
* **main** - Engine-global functionality & configuration (ASSERT etc.)
* **network** - Low-level network library (opening sockets etc.)
    * TCP sockets and listeners (SFML Packet compatible)
* **resources** - Resource management
    * ResourceManager for loading textures, fonts, shaders etc.
    * Simple texture atlas generation
* **scene** - Library for managing scenes and adding objects to it.
    * Scene and SceneObjects (in 2D) with `gui` integration and camera system
    * Basic texture renderer
    * Particle system
    * Tilemap renderer
    * Scene saving and loading from file (TODO: map editor)
* **sfml** - Dummy module for linking SFML
* **sound** - Basic sound utilities
* **tilemap** - Tilemaps
    * Tilemaps: abstract, fixed sized, dynamic sized (chunked)
* **util** - Common utility
    * Object system - used for serialization
    * Simple JSON parser and generator
    * System-specific stuff (filesystem, time)
    * Basic math (equations, vector operations, radians / degrees convertion)
    * Random number generators: LCG
