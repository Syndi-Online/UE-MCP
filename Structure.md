# Project Architecture

## Project Structure

```
MCPServer/Source/MCPServer/
├── Public/
│   ├── Tools/              — Tool definitions (1 tool = 1 function)
│   │   ├── Interfaces/
│   │   └── Models/
│   ├── Protocol/           — MCP protocol layer (HTTP, JSON-RPC)
│   │   └── Models/
│   ├── Modules/            — Engine wrappers (UE5 subsystem access)
│   │   └── Interfaces/
│   ├── System/             — Background tasks (optional, only if needed)
│   └── Core/               — Plugin entry point, initialization
├── Private/
│   ├── Tools/
│   │   └── Impl/
│   ├── Protocol/
│   ├── Modules/
│   │   └── Impl/
│   ├── System/
│   └── Core/
└── MCPServer.Build.cs
```

## Tools

**1 Tool = 1 function.**

Tools are grouped by domain, for example:

```
Tools/
├── Interfaces/
│   ├── Actors/
│   │   ├── SpawnActorTool
│   │   ├── DeleteActorTool
│   │   └── GetActorsTool
│   ├── Levels/
│   │   ├── LoadLevelTool
│   │   └── SaveLevelTool
│   └── Viewport/
│       ├── GetCameraTool
│       └── SetCameraTool
└── Impl/
    ├── Actors/
    │   ├── SpawnActorImplTool
    │   └── ...
    └── ...
```

File layout for `Impl`, `Interfaces`, and `Models` must mirror each other as closely as possible.

### Naming conventions:
- All Tool interfaces end with **`Tool`**
- All Tool implementations end with **`ImplTool`**
- All Tool tests end with **`ToolTest`**

---

## Protocol

This layer handles incoming data from MCP clients.

It receives HTTP requests, parses JSON-RPC 2.0 messages, and dispatches them to the appropriate Tool.

Each MCP method has its own handler inside the Protocol layer with dedicated request/response models.

> **Protocol uses its own data models, not business models.**

### Example folder structure for a protocol handler:

```
Protocol/
├── MCPHttpServer          — HTTP server setup, route binding
├── MCPJsonRpc             — JSON-RPC 2.0 parsing and serialization
├── MCPSessionManager      — Session ID management
└── Models/
    ├── MCPInitializeRequest
    ├── MCPInitializeResponse
    ├── MCPToolCallRequest
    └── MCPToolCallResponse
```

---

## Models

Business-layer data models live here. **Only these models** are used for communication between layers.

> **Models must not contain any logic or functions.**

Models are organized into subfolders by domain.

All models in this layer end with **`Model`**.

---

## Modules

This package contains code for all **external dependencies**, such as:

- UE5 Editor subsystems (UEditorActorSubsystem, UEditorAssetSubsystem, etc.)
- UE5 Engine APIs (GEditor, IConsoleManager, etc.)
- Viewport access (FEditorViewportClient)
- Asset Registry / Asset Tools
- Navigation System
- Source Control

Each Module wraps a specific UE5 subsystem behind an interface, so the rest of the plugin never calls UE5 APIs directly.

### Naming conventions:
- All Module interfaces end with **`Module`**
  *(Folder name reflects the purpose, not the implementation, to allow easy swapping.)*
- All Module implementations end with **`ImplModule`**
  *(Folder and implementation name reflects the UE5 subsystem or API used.)*
- All Module tests end with **`ModuleTest`**

---

## System

This layer is **used only when needed** to perform actions **independent of user requests**.

For example:
- Periodic health checks
- Background monitoring tasks
- Ticking operations

---

## Core

Contains the **plugin module entry point** (`FMCPServerModule`) and **initialization logic**.

### Initialization process:
- Module registration and dependency setup happens at plugin startup (`StartupModule`)
- All Modules are created and configured in Core
- Tool Registry is populated in Core
- HTTP server is started in Core

---

## Layer Communication Rules

Each layer can only communicate with **itself** or **one level directly below**:

```
Protocol  →  Tools  →  Modules  →  UE5 Engine
              ↑
           System
```

- **Protocol** receives requests and calls **Tools**
- **Tools** execute business logic and call **Modules**
- **Modules** interact with **UE5 Engine APIs**
- **System** can call **Tools**, but **Tools cannot call System**
- **System** cannot call **Modules** directly
- If **System** needs events, it subscribes via a **Tool**, which in turn subscribes to events from a **Module**

> **Tools and System do not hold state.** All state is stored in **Modules**.

---

## Testing

> **Only interfaces are tested.** Implementations are swapped via initialization configuration.

---

## Utilities

> **No utility classes or files.**

Instead:
- Create a **Module** with a proper interface
- Or use a **helper function** placed in a `Helpers/` subfolder within the layer where it is used

---

## Code Style Notes

- Never use single-letter variable names — always use descriptive, meaningful names
- Keep functions focused — one function, one responsibility
- Prefer composition over inheritance where practical
