# Contributing to UE-MCP

## Adding a New Tool — Two-Commit Workflow

Every new tool is added in **two commits**. This ensures documentation and API contracts are reviewed before implementation.

---

### Commit 1: Signature + Documentation + Checklist

Define the tool's interface and document it. **No implementation yet.**

#### Steps

1. **Define the result struct** in the module interface
   `Public/Modules/Interfaces/I<Section>Module.h`
   ```cpp
   struct FMyToolResult
   {
       bool bSuccess = false;
       FString SomeField;
       FString ErrorMessage;
   };
   ```
   Add a pure virtual method:
   ```cpp
   virtual FMyToolResult MyTool(const FString& Param) = 0;
   ```

2. **Add a stub declaration** in the module implementation header
   `Private/Modules/Impl/<Section>ImplModule.h`
   ```cpp
   virtual FMyToolResult MyTool(const FString& Param) override;
   ```

3. **Add a stub to the mock** for testing
   `Private/Tests/Mocks/Mock<Section>Module.h`
   ```cpp
   FMyToolResult MyToolResult; // preset by tests
   virtual FMyToolResult MyTool(const FString& Param) override { return MyToolResult; }
   ```

4. **Create the tool header**
   `Private/Tools/Impl/MyToolImplTool.h`
   ```cpp
   #pragma once
   #include "Tools/Interfaces/IMCPTool.h"

   class I<Section>Module;

   class FMyToolImplTool : public IMCPTool
   {
   public:
       explicit FMyToolImplTool(I<Section>Module& InModule);
       virtual FString GetName() const override;
       virtual FString GetDescription() const override;
       virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
       virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;
   private:
       I<Section>Module& Module;
   };
   ```

5. **Create the tool .cpp with schema only** (Execute returns "not implemented")
   `Private/Tools/Impl/MyToolImplTool.cpp`
   ```cpp
   FString FMyToolImplTool::GetName() const { return TEXT("my_tool"); }
   FString FMyToolImplTool::GetDescription() const { return TEXT("Description of what the tool does"); }
   TSharedPtr<FJsonObject> FMyToolImplTool::GetInputSchema() const { /* full JSON schema */ }
   TSharedPtr<FJsonObject> FMyToolImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
   {
       // TODO: implement in commit 2
       return MakeError(TEXT("my_tool is not yet implemented"));
   }
   ```

6. **Create documentation**
   `docs/<NN>-<section-name>/my_tool.md`
   Include: description, parameters table, response examples (success + error), JSON request example, notes.

7. **Update the docs index**
   `docs/README.md` — add link under the correct section.

8. **Update README.md checklist** — mark as `[ ]` (not implemented):
   ```markdown
   - [ ] [my_tool](docs/<NN>-<section-name>/my_tool.md) — short description
   ```

9. **Commit**:
   ```
   Add my_tool signature, docs, and checklist entry
   ```

---

### Commit 2: Implementation + Tests

Implement the logic and add full test coverage.

#### Steps

1. **Implement the module method**
   `Private/Modules/Impl/<Section>ImplModule.cpp`
   Write the actual UE5 logic (GameThread execution, error handling, etc.)

2. **Implement Execute() in the tool .cpp**
   `Private/Tools/Impl/MyToolImplTool.cpp`
   Parse arguments, call the module method, format the response text.

3. **Register the tool**
   `Private/Core/MCPServerModule.cpp`
   ```cpp
   #include "Tools/Impl/MyToolImplTool.h"
   // in RegisterTools():
   Registry->RegisterTool(MakeShared<FMyToolImplTool>(*SectionModule));
   ```

4. **Add unit tests**
   `Private/Tests/Unit/<Section>ToolsTest.cpp`
   - Test with valid input (mock returns success)
   - Test with missing required params
   - Test error cases (mock returns failure)

5. **Add integration test step**
   `Private/Tests/Integration/<Section>IntegrationTest.cpp`
   - Call the tool through the full JSON-RPC pipeline
   - Verify the response structure

6. **Update README.md checklist** — mark as `[x]`:
   ```markdown
   - [x] [my_tool](docs/<NN>-<section-name>/my_tool.md) — short description
   ```

7. **Build and verify tests pass**:
   ```bash
   "D:/UE_5.7/Engine/Build/BatchFiles/RunUAT.bat" BuildPlugin \
     -Plugin="D:/Git/UE-MCP/MCPServer/MCPServer.uplugin" \
     -Package="D:/Git/UE-MCP/Output" -Rocket
   ```

8. **Commit**:
   ```
   Implement my_tool with unit and integration tests
   ```

---

## Project Structure Reference

```
MCPServer/Source/MCPServer/
├── Public/
│   └── Modules/Interfaces/       — I<Section>Module.h (result structs + pure virtual)
├── Private/
│   ├── Core/
│   │   └── MCPServerModule.cpp   — tool registration
│   ├── Modules/Impl/             — <Section>ImplModule.h/.cpp (UE5 logic)
│   ├── Tools/
│   │   ├── Interfaces/IMCPTool.h — base tool interface
│   │   └── Impl/                 — <ToolName>ImplTool.h/.cpp
│   └── Tests/
│       ├── Mocks/                — Mock<Section>Module.h
│       ├── Unit/                 — <Section>ToolsTest.cpp
│       └── Integration/          — <Section>IntegrationTest.cpp
docs/
├── README.md                     — index with links to all tools
└── <NN>-<section-name>/          — one .md per tool
```

## Documentation Format

Each tool doc (`docs/<NN>-<section>/tool_name.md`) follows this structure:

```markdown
# tool_name

Short description of what the tool does.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| param1    | string | Yes    | ...         |

## Returns

Description of success/error return values.

## Example

\```json
{ "param1": "value" }
\```

## Response

### Success
\```
Actual success output text
\```

### Error
\```
Actual error output text
\```

## Notes

- Additional usage notes
```
