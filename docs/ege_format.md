This document describes the EGE game/scene serialization and sync formats.

# Overview

## Definitions
* **Endpoint** - a program / host which participates in network transmission. There are 2 types of endpoints:
    * **Client**/**agent** - connects to server.
    * **Server** - listens on specified port for client.
    * **Local** endpoint has direct access to data in implementation. Client and server are local endpoints.
    * **Remote** endpoint need to make requests to access data. Client has 1 remote endpoint (server), server has a remote endpoint for each connected client.
* **Developer** - The user (a human) of API that implements software using this format.

## Notation
* `0x##` - Hexadecimal (base-16) notation (0x6A)
* `0b##` - Binary (base-2) notation (0b1001)
* `0##` - Octal (base-8) notation (0547)
* `'#'` - ANSI character ('a')
* `"..."` - ANSI string ("abc")
* `U32"..."` - UTF-32 string (U32"↓„ęœ")

## Layers
The EGE format is divided into 4 layers:
> TODO: think about moving data layer into OSI presentation layer
* Data layer 
    * SFML layer - implemented by SFML, processes raw TCP stream into packets (for network transmission). See [SFML docs](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Packet.php).
    * BinaryStream layer - currently not implemented, will be used for binary save format.
    * File or another plaintext stream.
* Object layer
    * EGE binary format (described in **Object layer** section)
    * JSON (described in [this standard](https://www.ecma-international.org/publications-and-standards/standards/ecma-404/)) or another JSON-like format.
* Serialization layer
* Processing layer
    * `egeNetwork` protocol client/server
    * Entity serialization (for saves)

### Data layer
The data layer processes raw stream (TCP/file/...) to individual packets. It 

### Object layer
The object layer processes packets to EGE objects.

### Serialization layer
The serialization layer loads EGE entities (scenes, SceneObjects, saves,...) from EGE objects where appropriate.

### Processing layer
The processing layer performs specific actions basing on data from object/serialization layer.

### Summary

```
            [Processing]
             ^       ^
             |       |
         serialized  |
            args     |
             |       |
     [Serialization] |
             ^       |
             |       |
            args   packet ID
             |       |
           [   Object   ]
                 ^
                 |
               packet
                 |
               [Data]
                 ^
                 |
              raw data
                 |
                 *
        
```

## Network protocol
This document describes a network protocol (`_ProtocolVersion.value` == `0x1`) which uses EGE formats.

### TCP/IP and OSI layer
This protocol runs in 7. (application) layer of OSI model or in 4. (application) layer of TCP/IP model. It uses **TCP** as transmission protocol. The server listener port number can be chosen by developer and *may* be any of valid TCP ports (1 - 65535). The client connection port number *may* be greater than 1024 and *should* be in TCP dynamic port range (49152 - 65535).

### Packet types (directions)
* **Bidirectional** packet *may* be sent by both types of endpoint
* **Client-bound** packet *may* be sent by server to client.
* **Server-bound** packet *may* be sent by client to server.

# Data structure

## Types
Data types used in EGE format:

### Primitives
The format is defined by **Data layer**.
* `null` (0 bytes) - No data
* `uint8` (1 byte) - Unsigned 8-bit big-endian integer
* `uint16` (2 bytes) - Unsigned 16-bit big-endian integer
* `uint32` (4 bytes) - Unsigned 32-bit big-endian integer
* `uint64` (8 bytes) - Unsigned 64-bit big-endian integer
* `int8` (1 byte) - Signed 8-bit big-endian integer
* `int16` (2 bytes) - Signed 16-bit big-endian integer
* `int32` (4 bytes) - Signed 32-bit big-endian integer
* `int64` (8 bytes) - Signed 64-bit big-endian integer
* `float` (4 bytes) - IEEE-754 32-bit floating-point number
* `double` (8 bytes) - IEEE-754 64-bit floating-point number
* `bool` (1 byte) - Boolean value (saved as 1 byte)

### Strings
The format is defined by **Data layer**.
* `string` - ANSI string
* `u32string` - UTF-32 string

### EGE objects
The format is defined by **Object layer**.
* `Object` - Any EGE object
* `ObjectMap` - EGE map object (referred also as `Map`)
* `ObjectBoolean` - EGE boolean object (referred also as `Boolean`)
* `ObjectString` - EGE ANSI string object (referred also as `String`)
* `ObjectInt` - EGE signed int object (referred also as `Int`)
* `ObjectUnsignedInt` - EGE unsigned int object (referred also as `UnsignedInt`)
* `ObjectFloat` - EGE float object (referred also as `Float`)
* `ObjectList` - EGE list object (referred also as `List`)

### EGE entities
The format is defined by **Serialization layer**.
* `ColorRGBA`
* `Part`
* `Rect`
* `Scene`
* `SceneObject`
    * `SceneObject.Data`
* `SceneObjectType`
* `SceneObjectRegistry`
* `Vec2`
* `Vec3`

### Enumerations
The enumeration value is `Int` or `UnsignedInt`.

* `RotationMode`

# EGE binary format (Object layer)

## Object
An object consists of **type prefix** and data. Type prefixes:

| Prefix    | Corresponding type    |
|-----------|-----------------------|
| `'n'`     | `null`                |
| `'m'`     | `ObjectMap`           |
| `'l'`     | `ObjectList`          |
| `'u'`     | `ObjectUnsignedInt`   |
| `'i'`     | `ObjectInt`           |
| `'f'`     | `ObjectFloat`         |
| `'s'`     | `ObjectString`        |
| `'B'`     | `ObjectBoolean`       |

Object with non-listed prefix is considered `null`.

> TODO: for algorithms: Handle errors/EOF

Reading an Object:
1. Let `data` be a resulting object.
1. Read an *`uint8`* and save it to `prefix`.
1. If `prefix` doesn't match any valid type, return with error and stop.
1. Read an object with type based on `prefix` and save it to `data`.
1. If `data` is *invalid*, return with error and stop.
1. Return appropriate object created from `data` and stop.

## ObjectMap
An **ObjectMap** consists of arbitrary amount of key-`Object` pairs, placed . The pair format:

```
0            ...
+------------+---------------+
| string key | Object object |
+------------+---------------+
```

An empty key indicates the end of the map.

Reading a Map:
1. Let `map` be a resulting map.
1. Read a *`string`* and save it to `key`.
1. If `key` is empty, return `map` and stop.
1. Read an *`Object`* and save it to `value`. If reading fails, return with error and stop.
1. Add `key: value` entry to `map`.
1. Go to point `2`.

## ObjectList
An **ObjectList** consists of arbitrary amount of `Objects`. The object with `0x00` prefix indicates the end of the array.

Reading a List:
1. Let `list` be a resulting list.
1. Peek an *`uint8`* and save it to `prefix`.
1. If `prefix` == `0x00`, return `list` and stop.
1. Read an *`Object`* and save it to `object`.
1. If `object` is *invalid*, return with error and stop.
1. Add an `object` to `list`.
1. Go to point `2`.

## ObjectInt and ObjectUnsignedInt
An **Object(Unsigned)Int** consists of `type` byte and a `value`.

Valid types are:

| Value | Corresponding type   |
|-------|----------------------|
| `'b'` | `(u)int8`            |
| `'s'` | `(u)int16`           |
| `'i'` | `(u)int32`           |
| `'l'` | `(u)int64`           |

Reading an UnsignedInt:
1. Let `output` be a resulting integer.
1. Read an *`uint8`* and save it to `type`.
1. If `type` doesn't correspond to any type, return with error and stop
1. Read a value corresponding to `type` and save it to `output`.
1. Return an `output` and stop.

## ObjectFloat
An **ObjectFloat** consists of a single *`double`*.

Reading a Float:
1. Let `output` be a resulting float.
1. Read a *`double`* and save it to `output`.
1. Return an `output` and stop.

## ObjectString
An **ObjectString** consists of a single *`string`*.

Reading a String:
1. Let `output` be a resulting string.
1. Read a *`string`* and save it to `output`.
1. If `output` is empty, consider it an invalid packet.
1. Return an `output` and stop.

## ObjectBoolean
An **ObjectBoolean** consists of a single *`bool`*.

Reading a Boolean:
1. Let `output` be a resulting boolean.
1. Read a *`bool`* and save it to `output`.
1. Return an `output` and stop.

# `egeNetwork` packet (Object/Serialization layer)
The single **EGE packet** consists of:
* `type [uint32]` - The type of packet that determine operation. They are described in **Packet types** section.
* `data [ObjectMap|null]` - Data (arguments) for operation.

This is handled by *object layer*.

Packet types have unique *number* (that is saved in `type` field), *direction*, which is determined by name prefix (see **Packet types (directions)**), and code name (which is used in this documentation).

The direction prefixes:
* `_` - Bidirectional
* `C` - Client-bound
* `S` - Server-bound

The packet types are:

| Number | Code name                | Purpose                                               |
|--------|--------------------------|-------------------------------------------------------|
| `0x00` | `_Data`                  | Exchange any data within endpoints                    |
| `0x01` | `_Ping`                  | A request `_Pong` packet                              |
| `0x02` | `_Pong`                  | A response for `_Ping`                                |
| `0x03` | `_ProtocolVersion`       | Exchange EGE protocol version                         |
| `0x04` | `SResult`                | A server response for error codes                     |
| `0x05` | `CLogin`                 | Provide login data to server                          |
| `0x06` | `SLoginRequest`          | Request client login                                  |
| `0x07` | `SDisconnectReason`      | Provide disconnect reason to client                   |
| `0x08` | `SSceneObjectCreation`   | Inform that SceneObject was created                   |
| `0x09` | `SSceneObjectUpdate`     | Inform that SceneObject was updated                   |
| `0x0a` | `SSceneObjectDeletion`   | Inform that SceneObject was deleted                   |
| `0x0b` | `SSceneCreation`         | Inform that Scene was created                         |
| `0x0c` | `SSceneDeletion`         | Inform that Scene was deleted                         |
| `0x0d` | `CSceneObjectControl`    | Request SceneObject control (server-side)             |
| `0x0e` | `SDefaultControllerId`   | Provide default controller ID                         |
| `0x0f` | `CSceneObjectRequest`    | Request SceneObject data from server                  |
| `0x10` | `SSceneObjectControl`    | Request SceneObject control (client-side)             |
| `0x11` | `_Version`               | Provide endpoint version                              |
| `0x12` | `SAdditionalControllerId`| Request addition/removal of additional controller ID  |

# EGE entity serialization (Serialization layer)

## ColorRGBA
RGBA color.

Format:
* `root [Map]`
    * `x [Float]`
    * `y [Float]`
    * `z [Float]`

## ControlPacket
A single object control command.

Format:
* `root [Map]`
    * `type [String]` - The control type (operation).
    * `args [Map]` - Arguments for operation
        * User-defined

## Part
A **Part** is an object contained in *SceneObject*. It defines appearance of *SceneObject*.

Format:
* `root [Map]`
    * `layer [Int]` - The render layer.
    * `type [String]` - The part type ID.
    * *Part type*-defined properties. See [part documentation](TODO).

## Rect
A rectangle.

Format:
* `root [Map]`
    * `pos [Vec2]` - The position of the rectangle
    * `size [Vec2]` - The size of the rectangle

## Scene
A **Scene** is a container for *SceneObjects*.

Format:
* `root [Map]`
    * `objects [List]` - The *`SceneObjects`*.
        * `... [SceneObject]`
    * `staticObjects [List]` - The static *`SceneObjects`*.
        * `... [SceneObject]`
    * Developer-defined properties.

## SceneObject
A **SceneObject** is an object contained in *`Scene`*. It's a single entity with type, position etc.

Format:
* `root [Map]`
    * `typeId [String]` - The ID of corresponding *`SceneObjectType`*
    * `data [SceneObject.Data]` - The SceneObject data.

## SceneObject.Data
* `root [Map]`
    * `name [String]` - The object name
    * `m [Map]` - The main data
        * `parent [String]` - The parent object name
        * `p [Vec2]` - Position
        * `m [Vec2]` - Motion
        * `layer [Int]` - The render layer
        * `yaw [Float]`
        * `pitch [Float]`
        * `roll [Float]`
        * `yawMode [UnsignedInt/RotationMode]`
        * `pitchMode [UnsignedInt/RotationMode]`
        * `rollMode [UnsignedInt/RotationMode]`
        * Developer-defined data
    * `x [Map]` - The extended data
        * Developer-defined data

## SceneObjectType
A **SceneObjectType** is a type of *`SceneObject`*. It has an identifying type ID.

Format:
* `root [Map]`
    * `data [Map]`
        * `defaults [SceneObject.Data]` - The default values for properties
        * `parts [List]`
            * `... [Part]`

## SceneObjectRegistry
A **SceneObjectRegistry** is a map with *`SceneObjectTypes`*. It uses *`SceneObjectType`* string ID as keys and serialized *`SceneObjectTypes`* as values.

Format:
* `root [Map]`
    * `<typeId> [SceneObjectType]`

## Vec2
2D vector.

Format:
* `root [Map]`
    * `x [Int|UnsignedInt|Float]`
    * `y [Int|UnsignedInt|Float]`

## Vec3
3D vector

Format:
* `root [Map]`
    * `x [Int|UnsignedInt|Float]`
    * `y [Int|UnsignedInt|Float]`
    * `z [Int|UnsignedInt|Float]`

## Enumerations

### RotationMode

| Value  | Code name | Purpose                                  |
|--------|-----------|------------------------------------------|
| `0x00` | `Inherit` | Object angle is parent angle + own angle |
| `0x01` | `Lock`    | Parent angle is ignored                  |

# `egeNetwork` packets (Processing layer)

## Endpoints

### Required data and handlers
* `RemoteEndpoint [Map]` (Abstract)
    * `agentVersion [Int] = Developer-specified`
    * `agentString [String] = Developer-specified`
    * `connectionTime [UnsignedInt] = time()`
    * `lastAliveTime [UnsignedInt] = time()`
    * `pinged [Boolean] = false`
    * `protocolVersion [Int] = 1`
    * `send ( id [Int], args [Map|null] )` (Abstract)
* `LocalEndpoint [Map]` (Abstract)
    * `disconnect ( )` (Abstract)
    * `onData ( remote [RemoteEndpoint], data [Map] ) : null` (Abstract)
    * `onDisconnect ( remote [RemoteEndpoint], message [String] ) : null` (Abstract)
* `ClientConnection [Map]` : `RemoteEndpoint`
    * `additionalControllers [List]`
        * `... [Int]`
    * `defaultController [Int] = 0`
    * `loggedIn [Boolean] = false`
* `Client [Map]` : `ClientConnection`
    * `sResultMap [Map]`
    * `scene [Scene]`
        * `objects [List]`
            * `... [SceneObject]`
    * `generateNextUniqueId ( ) : Int`
    * `onResult ( args [Map] ) : null`
* `Server [Map]` : `RemoteEndpoint`
    * `clients [List]`
        * `... [ClientConnection]`
            * `disconnect ( reason [String|null] )`
    * `scene [Scene]`
        * `objects [List]`
            * `... [SceneObject]`
    * `onLogin ( remote [RemoteEndpoint], loginData [Map] ) : Boolean`

### Endpoint connection handler
This procedure *may* run just after creating an endpoint.

```js
endpoint.onConnect : function(remote) {
    // Reset all timers
    let currentTime = time();
    remote.lastAliveTime = currentTime;
    remote.connectionTime = currentTime;

    // IMPLEMENTATION-DEFINED:
    //   protocolVersion, agentVersion, agentString

    remote.send(_ProtocolVersion, { value: this.protocolVersion });
    remote.send(_Version, { value: this.agentVersion, string: this.agentString });
}
```

### Endpoint loop
This procedure *may* run at least once per second per remote endpoint.

```js
endpoint.loop : function(remote) {
    // Keep alive
    if(remote.lastAliveTime + 3 > time()) {
        if(remote.pinged)
            // If already pinged, disconnect
            remote.disconnect("Timed out");
        else {
            // Otherwise, send a _Ping and reset timer.
            remote.lastAliveTime = time();
            remote.pinged = true;
        }
    }

    // Version check
    if(remote.connectionTime + 3 > time() && (protocolVersion == null || agentVersion == null)) {
        remote.disconnect("Version check timed out");
    }
}
```

### Send operation
```js
endpoint.send : function(remote, packet) {
    // Packet uses SResult (and we are on server)
    if(packet.type.useSResult() && this instanceof Server) {
        this.prepareSResultPacket(packet);
    }

    // Send implementation by object layer (non-blocking)
    this.objectLayer.sendTo(remote, packet);
}
```

### Receive operation
```js
endpoint.onReceive : function(remote, packet) {
    // Call appropriate handler.
    this.handlers[packet.type](remote, packet);
}
```

### Disconnect operation
```js
remote.disconnect : function(message) {
    if(this instanceof ClientConnection) {
        this.send(SDisconnectReason, { message: message });
    }
    this.objectLayer.disconnect(this);
}
```

## `SResult` handling
Some server-bound / bidirectional packets *may* cause server to send `SResult` in response. These packets have assigned an unique ID (`uid [Int]`) to arguments. The `SResult` packet contains that `uid` along with data about error (as *`Map`*)

Operation done before sending SResult packet:
```js
endpoint.prepareSResultPacket : function(packet) {
    let uid = this.generateNextUniqueId();
    packet.args.uid = uid;
    this.sResultMap[uid] = packet;
}
```

## Bidirectional packets

### _Data (`0x00`)
Send arbitrary data to endpoint. This does not necessarily trigger any operation.
> These packets uses `SResult` as response for client packets.

Format:
* `args [Map]`
    * User-defined (except `uid`)

Receive operation:
```js
endpoint._Data : function(remote, args) {
    this.onData(remote, args);
}
```

### _Ping (`0x01`)
Check client if is alive. This packet is used when endpoint does not send anything within the last 3 seconds. The response *may* be sent in 3 seconds, otherwise the connection is closed.

Format:
* `args [null]`

Receive operation:
```js
endpoint._Ping : function(remote, args) {
    // Response with _Pong.
    remote.send(_Pong, null);
}
```

### _Pong (`0x02`)
Response for `_Ping` packet.

Format:
* `args [null]`

Receive operation:
```js
endpoint._Pong : function(remote, args) {
    // Reset ping status.
    remote.pinged = false;
}
```

### _ProtocolVersion (`0x03`)
Inform endpoint about version of processing layer. This packet *may* be sent in 3 seconds from connection, otherwise the connection *may* be closed by endpoint.

If received `value` is other than endpoint version, the connection *may* be closed.

Format:
* `args [Map]`
    * `value [Int]` - The value for protocol version.

Receive operation:
```js
endpoint._ProtocolVersion : function(remote, args) {
    // Check if version is valid.
    if(args.value != this.protocolVersion) {
        remote.disconnect("Invalid protocol version");
        return;
    }

    remote.protocolVersion = args.value;
}
```

### _Version (`0x11`)
Inform endpoint about agent version. This packet *may* be sent in 3 seconds from connection, otherwise the connection *may* be closed by endpoint.

If received `value` or `string` is other than endpoint version value / name, the connection *may* be closed.

Format:
* `args [Map]`
    * `value [Int]` - The value for agent version.
    * `string [String]` - The value for agent version string (name).

Receive operation:
```js
endpoint._Version : function(remote, args) {
    // Check if version is valid.
    if(args.value != this.agentVersion) {
        remote.disconnect("Invalid agent version");
        return;
    }
    if(args.string != this.agentString) {
        remote.disconnect("Invalid agent string");
        return;
    }

    remote.agentVersion = args.value;
    remote.agentString = args.string;
}
```

## Client-bound packets

### SResult (`0x04`)
Inform client about result of operation.

Format:
* `args [Map]`
    * `uid [Int]` - The unique ID of operation.
    * User-defined (except `uid`)

Receive operation:
```js
client.SResult : function(remote, args) {
    // Call user-defined handler.
    this.onResult(args);

    // Remove packet from uid-map.
    this.sResultMap.remove(args.uid);
}
```

### SLoginRequest (`0x06`)
Inform client that a login is required to connect to server.

Format:
* `args [Map]`
    * User-defined

Receive operation:
```js
client.SLoginRequest : function(remote, args) {
    remote.send(CLogin, /* IMPLEMENTATION-DEFINED login data */);
}
```

### SDisconnectReason (`0x07`)
Inform client about disconnect reason. This is the last packet sent by server before connection is closed.

Format:
* `args [Map]`
    * `message [String]` - The disconnection message.

Receive operation:
```js
client.SResult : function(remote, args) {
    // Call user-defined handler.
    this.onDisconnect(remote, args.message);

    // Disconnect from server.
    remote.disconnect();
}
```

### SSceneObjectCreation (`0x08`)
Send full *`SceneObject`* data to client. This packet is used when object is created or just after login or as response for `CSceneObjectRequest`.

Format:
* `args [Map]`
    * `object [SceneObject.Data]` - The SceneObject data.
    * `id [Int]` - The object ID. Note that server-side objects have negative IDs to not collide with client-side objects (cameras, particle systems etc.)
    * `typeId [String]` - The type ID corresponding to *`SceneObjectType`*.

Receive operation:
> TODO: Describe getObject,createObject,addObjectWithId,deserialize,
```js
client.SSceneObjectCreation : function(remote, args) {
    if(this.scene.getObject(args.id)) {
        // Object already exists; ignore creation.
        return;
    }
    let object = this.scene.createObject(args.typeId);
    object.deserialize(args.object);
    this.scene.addObjectWithId(args.id);
}
```

### SSceneObjectUpdate (`0x09`)
Send partial *`SceneObject`* data to client. This does not neccesarily imply object creation. If client does not know about object, it should send `CSceneObjectRequest` packet.

Format:
* `args [Map]`
    * `object [SceneObject.Data]` - The SceneObject data.
    * `id [Int]` - The object ID.

Note that `object` may have `m` (main) OR `x` (extended) data map (does not need to have both). The data map that is not sent within this packet is considered unchanged. Hovewer, if neither map is specified, the packet is invalid.

Receive operation:
```js
client.SSceneObjectUpdate : function(remote, args) {
    let object = this.scene.getObject(args.id);
    if(!object) {
        // Object does not exist; request it.
        this.server.send(CSceneObjectRequest, { id: args.id });
        return;
    }
    object.deserialize(args.object);
}
```

### SSceneObjectDeletion (`0x0a`)
Informs client that *`SceneObject`* was deleted. If *`SceneObject`* does not exist, does nothing.

Format:
* `args [Map]`
    * `id [Int]` - The object ID.

Receive operation:
```js
client.SSceneObjectUpdate : function(remote, args) {
    let object = this.scene.getObject(args.id);
    if(!object) {
        // Object does not exist; ignore.
        return;
    }
    object.deserialize(args.object);
}
```

### SSceneCreation (`0x0b`)
Informs client that *`Scene`* was created.
> TODO

Receive operation:
```js
client.SSceneCreation : function(remote, args) {
    // Not implemented
}
```

### SSceneDeletion (`0x0c`)
Informs client that *`Scene`* was deleted.
> TODO

Receive operation:
```js
client.SSceneDeletion : function(remote, args) {
    // Not implemented
}
```

### SDefaultControllerId (`0x0e`)
Informs client about which object should be controlled by default.

Format:
* `args [Map]`
    * `id [Int]` - The object ID. If it is `0`, the default controller is removed.

Receive operation:
```js
client.SDefaultControllerId : function(remote, args) {
    this.defaultController = args.id;
}
```

### SSceneObjectControl (`0x10`)
Performs control action on client-side object.

Format:
* `args [Map]`
    * `id [Int]` - The object ID
    * `data [ControlPacket]` - The control data.

Receive operation:
```js
client.SSceneObjectControl : function(remote, args) {
    this.scene.objects.find(args.id).control(args.data);
}
```

### SAdditionalControllerId (`0x12`)
Informs client about additional objects that are allowed to be controlled.

Format:
* `args [Map]`
    * `id [Int]` - The object ID
    * `remove [Boolean]` - If true, the additional controller is removed. Otherwise, it is added.

Receive operation:
```js
client.SAdditionalControllerId : function(remote, args) {
    if(args.remove)
        this.additionalControllers.remove(this.id);
    else
        this.additionalControllers.add(this.id);
}
```

## Server-bound packets

### CLogin (`0x05`)
Provides login information to server. After successful login, the server data (objects, scene, controllers etc.) are sent by server.

If login fails, a `SResult` packet with error description in `message` key is sent.

> This packet uses `SResult` packets.

Format:
* `args [Map]`
    * Developer-specified

Receive operation:
```js
server.CLogin : function(remote, args) {
    if(remote.loggedIn)
        return;
    let success = this.onLogin(remote, args);
    if(!success)
        remote.disconnect("Login failed");
    remote.loggedIn = true;
}
```

### CSceneObjectControl (`0x0d`)
Performs control action on server-side object. This *can* be rejected by server; if it happens, a `SResult` packet with error description in `message` key is sent.

The control requests sent to controllers that are listed as default or additional are guaranteed to succeed.

> This packet *can* use `SResult` packets.

Format:
* `args [Map]`
    * `id [Int]` - The object ID
    * `data [ControlPacket]` - The control data.

Receive operation:
```js
server.CSceneObjectControl : function(remote, args) {
    if(!remote.loggedIn) {
        remote.disconnect("Control when not logged in");
        return;
    }
    if(args.id == 0)
    {
        remote.disconnect("Control of null controller");
        return;
    }
    if(remote.defaultController == args.id || remote.additionalControllers.contains(args.id))
        this.scene.objects.find(args.id).control(args);
    else
        remote.disconnect("Control of non default nor additional controller");
}
```

### CSceneObjectRequest (`0x0f`)
Requests full *`SceneObject`* data from server. A response for this packet is `SSceneObjectCreation`.

Format:
* `args [Map]`
    * `id [Int]` - The object ID

Receive operation:
```js
server.CSceneObjectRequest : function(remote, args) {
    let object = this.scene.objects.find(args.id);
    if(!object)
    {
        // Non-existing object requested
        return;
    }
    remote.send(SSceneObjectCreation, { id: args.id, typeId: object.typeId, object: object.data });
}
```

# Scene serialization (Processing layer)

TODO
