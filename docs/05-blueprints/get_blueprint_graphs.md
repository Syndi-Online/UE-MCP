# get_blueprint_graphs

List all graphs in a Blueprint (event graphs, functions, macros, delegates).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |

## Returns

On success, returns a list of all graphs in the Blueprint. Each entry includes the graph name and its type (e.g., EventGraph, Function, Macro, Delegate).

On error, returns a message describing the failure (e.g., Blueprint not found).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor"
}
```

## Notes

- The response lists all graph types: event graphs, function graphs, macro graphs, and delegate graphs.
- Each graph entry contains its `GraphName` and `GraphType`.
- This tool is useful for inspecting the structure of a Blueprint before making modifications.
