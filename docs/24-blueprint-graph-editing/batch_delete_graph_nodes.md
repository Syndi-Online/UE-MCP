# batch_delete_graph_nodes

Delete multiple graph nodes in a single call.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph |
| node_ids | array | Yes | Array of node GUID strings to delete (max 100) |

## Returns

On success, returns the number of succeeded and failed deletions.

On error (all deletions failed), returns the error details.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_ids": [
    "4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678",
    "F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6",
    "A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6"
  ]
}
```

## Response

### Success
```
Batch delete: 3 succeeded, 0 failed
```

### Partial failure
```
Batch delete: 2 succeeded, 1 failed
Errors:
- [2] Node not found: A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6
```

### Error
```
Missing required parameters: blueprint_path, graph_name, node_ids
```

## Notes

- Maximum 100 node IDs per call.
- Each deletion is attempted independently; partial success is possible.
- `isError` is only true when all deletions fail and none succeed.
- Connected pins are automatically disconnected when a node is deleted.
