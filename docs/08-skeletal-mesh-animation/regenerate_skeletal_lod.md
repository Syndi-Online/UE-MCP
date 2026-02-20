# regenerate_skeletal_lod

Regenerate LOD levels for a skeletal mesh using mesh reduction.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the skeletal mesh |
| lod_count | integer | No | Number of LOD levels to generate (0 = use default) |
| regenerate_even_if_imported | boolean | No | Regenerate even if LODs were imported |

## Returns

On success, returns a confirmation message with the mesh path and the new LOD count.

On error, returns an error message describing the failure reason (e.g., mesh not found, reduction failed).

## Example

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin",
  "lod_count": 3,
  "regenerate_even_if_imported": false
}
```

## Notes

- If `lod_count` is not specified or is set to 0, the engine's default LOD generation settings are used.
- By default, LODs that were imported from external files will not be regenerated. Set `regenerate_even_if_imported` to `true` to override this behavior.
- The mesh reduction algorithm automatically simplifies the geometry for each LOD level.
