# add_blueprint_variable

Add a variable to a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| variable_name | string | Yes | Name of the variable to add |
| variable_type | string | Yes | Type: Boolean, Integer, Int64, Float, String, Name, Text, Vector, Rotator, Transform, Byte, or class path |

## Returns

On success, returns `VariableName` of the newly added variable.

On error, returns a message describing the failure (e.g., Blueprint not found, duplicate variable name, unsupported type).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "variable_name": "Health",
  "variable_type": "Float"
}
```

## Response

### Success
```
Variable added successfully.
VariableName: Health
```

### Error
```
Failed to add variable: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- Supported built-in types: `Boolean`, `Integer`, `Int64`, `Float`, `String`, `Name`, `Text`, `Vector`, `Rotator`, `Transform`, `Byte`.
- For object references, use the full class path (e.g., `/Script/Engine.StaticMesh`).
- Variable names must be unique within the Blueprint.
- After adding a variable, you may need to compile the Blueprint for changes to take effect.
