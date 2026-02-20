# connect_material_property

Connect a material expression to a material output property.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| from_index | number | Yes | Index of the source expression |
| from_output | string | No | Output pin name on the source expression (default: "") |
| property | string | Yes | Material property: BaseColor, Metallic, Specular, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, AmbientOcclusion, WorldPositionOffset, SubsurfaceColor, Refraction, Anisotropy, Tangent |

## Returns

On success, returns `PropertyName` of the connected material property.

On error, returns a message describing the failure (e.g., material not found, invalid expression index, invalid property name).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "from_index": 0,
  "property": "BaseColor"
}
```

## Response

### Success
```
Expression connected to material property successfully.
PropertyName: BaseColor
```

### Error
```
Failed to connect material property: Unknown material property: InvalidProperty
```

## Notes

- Supported material properties: `BaseColor`, `Metallic`, `Specular`, `Roughness`, `Normal`, `EmissiveColor`, `Opacity`, `OpacityMask`, `AmbientOcclusion`, `WorldPositionOffset`, `SubsurfaceColor`, `Refraction`, `Anisotropy`, `Tangent`.
- The `from_output` parameter specifies which output pin on the expression to connect. If empty, the first output is used.
- Not all properties are available for all material blend modes. For example, `Opacity` requires a translucent blend mode.
- This tool connects an expression node to the final material output node (the main material result).
