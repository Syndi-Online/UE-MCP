# batch_set_material_expression_properties

Set properties on multiple material expression nodes in one call. Up to 100 operations per batch.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| operations | array | Yes | Array of operations. Each has expression_index, property_name, and property_value. |

Each object in the `operations` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| expression_index | number | Index of the material expression node |
| property_name | string | Name of the property to set |
| property_value | string | Value to assign to the property |

## Returns

On success, returns a text message with the number of succeeded and failed operations (e.g. `"Batch completed: 3 succeeded, 0 failed"`). The `isError` flag is `true` only when all operations fail.

On error (missing or invalid parameters), returns an error message describing the issue.

If any individual operations fail, the errors are listed with their index and error message.

## Example

```json
{
  "material_path": "/Game/Materials/M_Example",
  "operations": [
    {
      "expression_index": 0,
      "property_name": "ConstA",
      "property_value": "1.0"
    },
    {
      "expression_index": 1,
      "property_name": "R",
      "property_value": "0.5"
    }
  ]
}
```

## Notes

- Maximum of 100 operations per batch. Exceeding this limit returns an error.
- All operations apply to the same material specified by `material_path`.
- Each operation is executed independently; a failure in one does not stop the others.
- The `isError` flag is only set to `true` when all operations fail (succeeded == 0 and failed > 0).
