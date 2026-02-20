# delete_material_expression

Delete a material expression node from a material.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| expression_index | number | Yes | Index of the expression to delete |

## Returns

On success, returns a confirmation that the material expression was deleted successfully.

On error, returns a message describing the failure (e.g., material not found, invalid expression index).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "expression_index": 2
}
```

## Notes

- The `expression_index` corresponds to the index returned by `create_material_expression` or found via `get_material_expressions`.
- Deleting an expression that is connected to other nodes will remove those connections.
- After deleting an expression, the indices of remaining expressions may change. Use `get_material_expressions` to get the updated indices.
- Consider recompiling the material after deleting expressions.
