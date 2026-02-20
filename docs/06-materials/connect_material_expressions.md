# connect_material_expressions

Connect two material expression nodes.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| from_index | number | Yes | Index of the source expression |
| from_output | string | No | Output pin name on the source expression (default: "") |
| to_index | number | Yes | Index of the destination expression |
| to_input | string | No | Input pin name on the destination expression (default: "") |

## Returns

On success, returns a confirmation that the material expressions were connected successfully.

On error, returns a message describing the failure (e.g., material not found, invalid expression index, incompatible pin types).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "from_index": 0,
  "from_output": "",
  "to_index": 1,
  "to_input": "A"
}
```

## Response

### Success
```
Material expressions connected successfully.
```

### Error
```
Failed to connect material expressions: Expression index 5 out of range
```

## Notes

- The `from_output` and `to_input` parameters specify the pin names on the source and destination expressions. If left empty (default), the first available pin is used.
- Use `get_material_expressions` to find valid expression indices.
- Pin names depend on the expression type. For example, an `Add` expression has inputs `A` and `B`.
- When connecting expressions that have a single output or input, you can omit the pin name parameters.
