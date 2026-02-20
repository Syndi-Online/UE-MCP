# get_material_parameter

Get a material parameter value (scalar, vector, texture, or static switch).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| parameter_name | string | Yes | Name of the parameter to get |

## Returns

On success, returns `ParameterName`, `ParameterType`, and `Value` of the material parameter.

On error, returns a message describing the failure (e.g., material not found, parameter not found).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "parameter_name": "BaseColorTint"
}
```

## Notes

- This tool works with material parameters exposed via parameter expressions (e.g., ScalarParameter, VectorParameter, TextureSampleParameter2D, StaticSwitchParameter).
- The `ParameterType` in the response indicates the kind of parameter (e.g., Scalar, Vector, Texture, StaticSwitch).
- The `Value` is returned as a string representation of the parameter's current value.
