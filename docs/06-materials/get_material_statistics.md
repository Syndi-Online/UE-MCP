# get_material_statistics

Get shader statistics for a material (instruction counts, samplers, etc.).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |

## Returns

On success, returns the following shader statistics:
- `NumVertexShaderInstructions` -- number of vertex shader instructions
- `NumPixelShaderInstructions` -- number of pixel shader instructions
- `NumSamplers` -- number of texture samplers used
- `NumVertexTextureSamples` -- number of vertex texture samples
- `NumPixelTextureSamples` -- number of pixel texture samples
- `NumVirtualTextureSamples` -- number of virtual texture samples
- `NumInterpolatorScalars` -- number of interpolator scalars

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
Material statistics retrieved successfully.
NumVertexShaderInstructions: 78
NumPixelShaderInstructions: 142
NumSamplers: 3
NumVertexTextureSamples: 0
NumPixelTextureSamples: 3
NumVirtualTextureSamples: 0
NumInterpolatorScalars: 4
```

### Error
```
Failed to get material statistics: Material not found: /Game/Materials/M_Missing
```

## Notes

- The material must be compiled for statistics to be available. Use `recompile_material` first if needed.
- These statistics are useful for performance profiling and optimization of materials.
- High instruction counts or sampler usage may indicate a material that is too complex for target hardware.
