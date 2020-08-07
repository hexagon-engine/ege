### 0001 EGE/loop: load failed
	>> GameLoop::run() return 0x0001
### 0002 EGE/loop: finish failed
	>> GameLoop::run() return 0x0002
### 0003 EGE/main: ASSERTION FAILED: ...
	>> exit(1234)
### 0004 EGE/main: debug: ...
### 0005 EGE/resources: could not load resource: TEXTURE %s
	>> ResourceManager::loaTexturefromFile() return false
### 0006 EGE/resources: could not load resource: FONT %s
	>> ResourceManager::loadFontfromFile() return false
### 0007 EGE/resources: could not open resource directory: %s
	>> ResourceManager::setResourcePath() return false
### 0008 EGE/resources: invalid TEXTURE requested: %s, falling back to unknown texture
	>> ResourceManager::getTexture() return m_unknownTexture
### 0009 EGE/resources: invalid FONT requested: %s
	>> ResourceManager::getFont() return nullptr
### 000A EGE/gui: no ResourceManager set, setting to default GUIResourceManager

