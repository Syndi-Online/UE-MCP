# add_graph_node

Add a node to a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_type | string | Yes | Type of node: CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum, MapForEach, FormatText, EnumToString/GetEnumeratorNameAsString |
| member_name | string | No | Function/variable/macro name. Required for: CallFunction, Event, VariableGet, VariableSet, MacroInstance. E.g. PrintString, ForEachLoop |
| target | string | No | Target class/type. For CallFunction: class name. For DynamicCast: class to cast to. For SwitchEnum: enum path. For ComponentBoundEvent: component variable name. For VariableGet: owner class for external member access (creates Target input pin) |
| position | object | No | Optional position of the node `{x, y}` |

The `position` object:

| Field | Type | Description |
|-------|------|-------------|
| x | number | X coordinate |
| y | number | Y coordinate |

## Returns

On success, returns a text message with the node ID, pin count, and a JSON object containing:

- `node_id` (string) -- GUID of the created node
- `pins` (array) -- array of pin objects, each with:
  - `pin_id` (string)
  - `pin_name` (string)
  - `pin_type` (string)
  - `direction` (string)
  - `connected_pin_ids` (array of strings)

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_type": "CallFunction",
  "member_name": "PrintString",
  "target": "KismetSystemLibrary",
  "position": { "x": 200, "y": 100 }
}
```

### External VariableGet (accessing a member of another class)

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyCharacter",
  "graph_name": "EventGraph",
  "node_type": "VariableGet",
  "member_name": "CapsuleComponent",
  "target": "Character",
  "position": { "x": 400, "y": 200 }
}
```

This creates a VariableGet node for `ACharacter::CapsuleComponent` with a Target input pin. Connect a Cast To Character output to this Target pin.

## Response

### Success
```
Node added with ID: 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678 and 5 pins.
{"node_id":"4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678","pins":[{"pin_id":"A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6","pin_name":"execute","pin_type":"exec","direction":"input","connected_pin_ids":[]},{"pin_id":"B2C3D4E5-F6A7B8C9-D0E1F2A3-B4C5D6E7","pin_name":"then","pin_type":"exec","direction":"output","connected_pin_ids":[]},{"pin_id":"C3D4E5F6-A7B8C9D0-E1F2A3B4-C5D6E7F8","pin_name":"InString","pin_type":"string","direction":"input","connected_pin_ids":[]}]}
```

### Error
```
Missing required parameter: node_type
```
```
Failed to add graph node: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- The `member_name` parameter is required for most node types (CallFunction, Event, VariableGet, VariableSet, MacroInstance) but not for IfThenElse/Branch, DynamicCast, MapForEach, or EnumToString.
- For **FormatText**, `member_name` is optional — if provided it sets the format string (e.g. `"{Name}: {Amount}"`).
- The `target` parameter meaning varies by node type:
  - **CallFunction**: the owning class name (e.g. `KismetSystemLibrary`, `Actor`)
  - **DynamicCast**: the class to cast to
  - **SwitchEnum**: the enum asset path
  - **ComponentBoundEvent**: the component variable name
  - **VariableGet**: the owner class for external member access (e.g. `Character`, `Pawn`). When `target` is provided, the node accesses a member of the specified class instead of the current Blueprint, creating a Target input pin for connecting an external object reference (e.g., Cast result). Class resolution tries: exact name, U-prefix, A-prefix, full path.
- **MapForEach** iterates a `TMap`, providing Key, Value, loop body, and Completed exec pins.
- **FormatText** creates a Format Text node for `FText::Format`-style string building.
- **EnumToString** (alias **GetEnumeratorNameAsString**) converts an enum value to its display string.
- If `position` is omitted, the node is placed at (0, 0).
- The response includes all pins on the newly created node, which is useful for immediately connecting them.
