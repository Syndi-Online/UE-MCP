# set_material_instance_parameter

Set a parameter value on a Material Instance.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| instance_path | string | Yes | Asset path of the Material Instance |
| parameter_name | string | Yes | Name of the parameter to set |
| value | string | Yes | Parameter value as string |
| parameter_type | string | Yes | Type: Scalar, Vector, Texture, StaticSwitch |

## Returns

On success, returns a confirmation that the material instance parameter was set successfully.

On error, returns a message describing the failure (e.g., instance not found, parameter not found, invalid value format).

## Example

```json
{
  "instance_path": "/Game/Materials/MI_Red",
  "parameter_name": "BaseColorTint",
  "value": "(R=1.0,G=0.0,B=0.0,A=1.0)",
  "parameter_type": "Vector"
}
```

## Notes

- The `parameter_type` must match the actual type of the parameter in the parent material.
- Supported types: `Scalar` (float value), `Vector` (color/vector value), `Texture` (texture asset path), `StaticSwitch` (true/false).
- For `Scalar` type, provide a numeric value as a string (e.g., `"0.5"`).
- For `Vector` type, provide the value in Unreal's linear color format (e.g., `"(R=1.0,G=0.0,B=0.0,A=1.0)"`).
- For `Texture` type, provide the asset path of the texture (e.g., `"/Game/Textures/T_MyTexture"`).
- For `StaticSwitch` type, provide `"true"` or `"false"`.
