# get_material_expressions

Get all expression nodes in a material with their index, name, class, and editor position.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |

## Returns

On success, returns the total expression count and a JSON array where each entry contains:
- `index` -- the expression index (used to reference this node in other tools)
- `name` -- the expression name
- `class` -- the expression class name
- `position` -- an object with `x` and `y` coordinates in the material editor

On error, returns a message describing the failure (e.g., material not found).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base"
}
```

## Response

### Success
```
Material has 3 expressions.
[
    {
        "index": 0,
        "name": "MaterialExpressionConstant3Vector_0",
        "class": "MaterialExpressionConstant3Vector",
        "position": { "x": -300, "y": 0 }
    },
    {
        "index": 1,
        "name": "MaterialExpressionMultiply_0",
        "class": "MaterialExpressionMultiply",
        "position": { "x": -150, "y": 0 }
    },
    {
        "index": 2,
        "name": "MaterialExpressionScalarParameter_0",
        "class": "MaterialExpressionScalarParameter",
        "position": { "x": -300, "y": 200 }
    }
]
```

### Error
```
Failed to get material expressions: Material not found: /Game/Materials/M_Missing
```

## Notes

- The `index` field is used to reference expressions in tools like `connect_material_expressions`, `connect_material_property`, `delete_material_expression`, `get_material_expression_property`, and `set_material_expression_property`.
- Expression indices may change after deleting an expression. Always re-query after modifications.
- This tool is useful for inspecting the full expression graph of a material before making connections or modifications.
