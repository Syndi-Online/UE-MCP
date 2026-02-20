# batch_connect_material_expressions

Connect multiple material expression nodes in one call. Up to 100 connections per batch.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| connections | array | Yes | Array of connections. Each has from_index, to_index, from_output, and to_input. |

Each object in the `connections` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| from_index | number | Index of the source expression node |
| to_index | number | Index of the destination expression node |
| from_output | string | Output name on the source node (optional, defaults to empty string) |
| to_input | string | Input name on the destination node |

## Returns

On success, returns a text message with the number of succeeded and failed connections (e.g. `"Batch completed: 4 succeeded, 0 failed"`). The `isError` flag is `true` only when all connections fail.

On error (missing or invalid parameters), returns an error message describing the issue.

If any individual connections fail, the errors are listed with their index and error message.

## Example

```json
{
  "material_path": "/Game/Materials/M_Example",
  "connections": [
    {
      "from_index": 0,
      "to_index": 2,
      "from_output": "",
      "to_input": "A"
    },
    {
      "from_index": 1,
      "to_index": 2,
      "from_output": "RGB",
      "to_input": "B"
    }
  ]
}
```

## Notes

- Maximum of 100 connections per batch. Exceeding this limit returns an error.
- All connections apply to the same material specified by `material_path`.
- The `from_output` field is optional; if not provided, it defaults to an empty string (the default output).
- The `to_input` field is required for each connection.
- Each connection is executed independently; a failure in one does not stop the others.
- The `isError` flag is only set to `true` when all connections fail (succeeded == 0 and failed > 0).
