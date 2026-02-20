# set_material_expression_property

Set a property value on a material expression node.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| expression_index | number | Yes | Index of the expression node |
| property_name | string | Yes | Name of the property to set |
| property_value | string | Yes | Value to set the property to (as text) |

## Returns

On success, returns a confirmation that the property was set on the specified expression node.

On error, returns a message describing the failure (e.g., material not found, invalid expression index, property not found, invalid value).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "expression_index": 0,
  "property_name": "R",
  "property_value": "0.5"
}
```

## Response

### Success
```
Property 'R' on expression 0 set successfully
```

### Error
```
Failed to set material expression property: Expression index 5 out of range
```

## Notes

- The `expression_index` can be obtained from `get_material_expressions` or from the result of `create_material_expression`.
- Property names and expected value formats are specific to each expression class.
- The `property_value` is provided as a text string and is parsed according to the property type internally.
- After modifying expression properties, consider recompiling the material with `recompile_material` to see the changes in the viewport.
