# generate_uv_channel

Generate UV coordinates using a projection method.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| uv_channel_index | integer | Yes | UV channel index to generate into |
| projection_type | string | Yes | Projection type: Planar, Cylindrical, Box |
| lod_index | integer | No | LOD index (default 0) |

## Returns

On success, returns a confirmation message with the UV channel index, mesh path, LOD index, projection type used, and the total number of UV channels.

On error, returns an error message describing the failure reason (e.g., mesh not found, invalid UV channel index, invalid projection type).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Pillar",
  "uv_channel_index": 1,
  "projection_type": "Box",
  "lod_index": 0
}
```

## Response

### Success
```
UV channel 1 generated for '/Game/Meshes/SM_Pillar' at LOD 0 using Box projection.
NumUVChannels: 3
```

### Error
```
Failed to generate UV channel: Asset not found: /Game/Meshes/SM_Missing
```

## Notes

- If `lod_index` is not specified, it defaults to 0 (the highest detail LOD).
- Supported projection types:
  - **Planar** -- projects UVs onto a plane
  - **Cylindrical** -- wraps UVs around a cylinder
  - **Box** -- projects UVs from all six box faces
- The target UV channel must already exist. Use `add_uv_channel` first if needed.
- This will overwrite any existing UV data in the specified channel.
