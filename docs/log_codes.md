### 0001 EGE/loop: load failed
	>> GameLoop::run() return 0x0001
### 0002 EGE/loop: finish failed
	>> GameLoop::run() return 0x0002
### 0003 EGE/main: ASSERTION FAILED: %s
	>> exit(1234)
### 0004 EGE/main: debug: %s
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
### 0013 EGE/network: Client connected (%s:%d)
### 0014 EGE/network: Event ClientConnect failed (rejected by EventHandler)
### 0015 EGE/network: Event Receive failed (rejected by EventHandler)
### 0016 EGE/network: Socket Receive failed (system error)
### 0017 EGE/network: Socket Send failed (system error)
### 0018 EGE/network: Kicking client %s:%d due to explicit disconnect
### 0019 EGE/network: Client: Connection failed to (%s:%s)
### 001A EGE/network: Disconnecting network endpoint (%d)
### 001B EGE/network: Kicking client (%s:%d)
### 001C EGE/asyncLoop: AsyncTask[%s] worker finished with non-zero (%d) status!
### 001D EGE/egeNetwork: Packet parsing error: %s
### 001E EGE/egeNetwork: Starting server
### 001F EGE/egeNetwork: Closing server
### 0020 EGE/egeNetwork: Server PROTOCOL_VERSION doesn't match client! (required %d, got %d)
### 0021 EGE/egeNetwork: Client PROTOCOL_VERSION doesn't match server! (required %d, got %d)
