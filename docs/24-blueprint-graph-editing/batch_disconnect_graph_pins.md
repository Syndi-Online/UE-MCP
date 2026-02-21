# batch_disconnect_graph_pins

Disconnect multiple pin pairs between graph nodes in a single call.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph |
| disconnections | array | Yes | Array of disconnections to make (max 100) |

Each object in the `disconnections` array has the following fields:

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| source_node_id | string | Yes | Node GUID of the source node |
| source_pin | string | Yes | Pin name on the source node |
| target_node_id | string | Yes | Node GUID of the target node |
| target_pin | string | Yes | Pin name on the target node |

## Returns

On success, returns the number of succeeded and failed disconnections.

On error (all disconnections failed), returns the error details.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "disconnections": [
    {
      "source_node_id": "4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678",
      "source_pin": "then",
      "target_node_id": "F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6",
      "target_pin": "execute"
    },
    {
      "source_node_id": "4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678",
      "source_pin": "ReturnValue",
      "target_node_id": "F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6",
      "target_pin": "InString"
    }
  ]
}
```

## Response

### Success
```
Batch disconnect: 2 succeeded, 0 failed
```

### Partial failure
```
Batch disconnect: 1 succeeded, 1 failed
Errors:
- [1] Pins are not connected
```

### Error
```
Missing required parameters: blueprint_path, graph_name, disconnections
```

## Notes

- Maximum 100 disconnections per call.
- Each disconnection is attempted independently; partial success is possible.
- `isError` is only true when all disconnections fail and none succeed.
- Use `get_graph_nodes` to discover existing connections before disconnecting.
