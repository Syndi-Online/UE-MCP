# connect_graph_pins

Connect two pins between nodes in a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| source_node_id | string | Yes | Node ID of the source node |
| source_pin_name | string | Yes | Pin name on the source node |
| target_node_id | string | Yes | Node ID of the target node |
| target_pin_name | string | Yes | Pin name on the target node |

## Returns

On success, returns a text message confirming the connection (e.g. `"Connected ABC123.OutputPin -> DEF456.InputPin"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "source_node_id": "ABC123-DEF456",
  "source_pin_name": "then",
  "target_node_id": "GHI789-JKL012",
  "target_pin_name": "execute"
}
```

## Response

### Success
```
Connected 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678.then -> F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6.execute
```

### Error
```
Missing required parameter: target_pin_name
```
```
Failed to connect pins: Pin 'NonExistent' not found on source node
```

## Notes

- Pin names must match exactly. Use `get_graph_nodes` to inspect available pin names on nodes.
- Connections are directional: source is typically an output pin, target is an input pin.
- Type compatibility is enforced by Unreal Engine; incompatible pin types will cause an error.
