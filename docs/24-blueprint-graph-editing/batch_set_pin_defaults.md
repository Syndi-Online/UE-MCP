# batch_set_pin_defaults

Set default values on multiple pins in a single call.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph |
| operations | array | Yes | Array of pin default operations (max 100) |

Each object in the `operations` array has the following fields:

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| node_id | string | Yes | Node GUID |
| pin_name | string | Yes | Pin name to set the default value on |
| value | string | Yes | Default value as string |

## Returns

On success, returns the number of succeeded and failed operations.

On error (all operations failed), returns the error details.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "operations": [
    {
      "node_id": "F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6",
      "pin_name": "InString",
      "value": "Hello World"
    },
    {
      "node_id": "F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6",
      "pin_name": "bPrintToScreen",
      "value": "true"
    },
    {
      "node_id": "A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6",
      "pin_name": "Duration",
      "value": "2.0"
    }
  ]
}
```

## Response

### Success
```
Batch set pin defaults: 3 succeeded, 0 failed
```

### Partial failure
```
Batch set pin defaults: 2 succeeded, 1 failed
Errors:
- [2] Pin not found: BadPin on node A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6
```

### Error
```
Missing required parameters: blueprint_path, graph_name, operations
```

## Notes

- Maximum 100 operations per call.
- Each operation is attempted independently; partial success is possible.
- `isError` is only true when all operations fail and none succeed.
- Values are strings, same format as `set_pin_default_value` (e.g., "true", "2.0", "(X=1,Y=2,Z=3)").
