# add_graph_node

Add a node to a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_type | string | Yes | Type of node: CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum |
| member_name | string | No | Function/variable/macro name. Required for: CallFunction, Event, VariableGet, VariableSet, MacroInstance. E.g. PrintString, ForEachLoop |
| target | string | No | Target class/type. For CallFunction: class name. For DynamicCast: class to cast to. For SwitchEnum: enum path. For ComponentBoundEvent: component variable name |
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

## Notes

- The `member_name` parameter is required for most node types (CallFunction, Event, VariableGet, VariableSet, MacroInstance) but not for IfThenElse/Branch or DynamicCast.
- The `target` parameter meaning varies by node type:
  - **CallFunction**: the owning class name (e.g. `KismetSystemLibrary`, `Actor`)
  - **DynamicCast**: the class to cast to
  - **SwitchEnum**: the enum asset path
  - **ComponentBoundEvent**: the component variable name
- If `position` is omitted, the node is placed at (0, 0).
- The response includes all pins on the newly created node, which is useful for immediately connecting them.
