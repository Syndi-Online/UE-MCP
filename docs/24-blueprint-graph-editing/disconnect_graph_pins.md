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

## Notes

- The specified pins must currently be connected; otherwise the operation will fail.
- Use `get_graph_nodes` to inspect current pin connections before disconnecting.
