# batch_set_node_positions

Move multiple nodes on a Blueprint graph in one call. Up to 100 operations per batch.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph within the Blueprint |
| operations | array | Yes | Array of operations. Each has node_id (string) and position {x, y} (numbers). |

Each object in the `operations` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| node_id | string | ID of the node to move |
| position | object | New position `{x, y}` |

## Returns

On success, returns a text message with the number of succeeded and failed operations (e.g. `"Batch completed: 5 succeeded, 0 failed"`). The `isError` flag is `true` only when all operations fail.

On error (missing or invalid parameters), returns an error message describing the issue.

If any individual operations fail, the errors are listed with their index and error message.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "operations": [
    {
      "node_id": "ABC123-DEF456",
      "position": { "x": 100, "y": 200 }
    },
    {
      "node_id": "GHI789-JKL012",
      "position": { "x": 400, "y": 200 }
    }
  ]
}
```

## Response

### Success
```
Batch completed: 2 succeeded, 0 failed
```

### Error
```
Missing required parameter: blueprint_path
```
```
Too many operations. Maximum 100 per batch.
```
```
Batch completed: 1 succeeded, 1 failed

Errors:
- [1] Node not found: INVALID-NODE-ID
```

## Notes

- Maximum of 100 operations per batch. Exceeding this limit returns an error.
- All operations apply to the same Blueprint graph specified by `blueprint_path` and `graph_name`.
- Each operation is executed independently; a failure in one does not stop the others.
- The `isError` flag is only set to `true` when all operations fail (succeeded == 0 and failed > 0).
