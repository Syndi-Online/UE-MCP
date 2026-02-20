# disconnect_graph_pins

Disconnect two connected pins in a Blueprint graph.

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

On success, returns a text message confirming the disconnection (e.g. `"Disconnected ABC123.OutputPin from DEF456.InputPin"`).

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
Disconnected 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678.then from F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6.execute
```

### Error
```
Missing required parameter: source_pin_name
```
```
Failed to disconnect pins: Pins are not connected
```

## Notes

- The specified pins must currently be connected; otherwise the operation will fail.
- Use `get_graph_nodes` to inspect current pin connections before disconnecting.
