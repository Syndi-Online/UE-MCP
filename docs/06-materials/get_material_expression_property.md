# get_material_expression_property

Get a property value from a material expression node.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| expression_index | number | Yes | Index of the expression node |
| property_name | string | Yes | Name of the property to get |

## Returns

On success, returns the property value as a string for the specified property on the given expression node.

On error, returns a message describing the failure (e.g., material not found, invalid expression index, property not found).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "expression_index": 0,
  "property_name": "R"
}
```

## Response

### Success
```
Property 'R' on expression 0: 0.500000
```

### Error
```
Failed to get material expression property: Expression index 5 out of range
```

## Notes

- The `expression_index` can be obtained from `get_material_expressions` or from the result of `create_material_expression`.
- Property names are specific to each expression class. For example, a `Constant` expression has an `R` property, while a `Constant3Vector` has a `Constant` property.
- Use this tool to inspect expression configuration before modifying it with `set_material_expression_property`.
