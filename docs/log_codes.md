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
### 000B EGE/resources: invalid FONT requested to be default: %s
	>> ResourceManager::setDefaultFont() return false
### 000C EGE/gpo: std::exception caught in GameplayObjectRegistry::%s: %s
	>> GameplayObjectRegistry::%s return RegistryError::Unknown
### 000D EGE/gpo: unknown exception caught in GameplayObjectRegistry::%s: %s
	>> GameplayObjectRegistry::%s return RegistryError::Unknown
### 000E EGE/syswindow: window already removed, close skipped
### 000F EGE/scene: Scene2D: no camera set, defaulting to {[0,0] 0x 0dg} transform.
### 0010 EGE/network: Server listening on %d
### 0011 EGE/network: Failed to start server on %d
### 0012 EGE/network: Closing server
### 0013 EGE/network: Client connected (ip=%s, port=%d)
### 0014 EGE/network: Event ClientConnect failed (rejected by EventHandler)
### 0015 EGE/network: Event Receive failed (rejected by EventHandler)
### 0016 EGE/network: Socket Receive failed (system error)
### 0017 EGE/network: Socket Send failed (system error)
### 0018 EGE/network: Kicking client %s:%s due to explicit disconnect
