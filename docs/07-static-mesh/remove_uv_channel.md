# remove_uv_channel

Remove a UV channel from a static mesh LOD.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| uv_channel_index | integer | Yes | Index of the UV channel to remove |
| lod_index | integer | No | LOD index (default 0) |

## Returns

On success, returns a confirmation message with the removed UV channel index, the mesh path, LOD index, and the remaining number of UV channels.

On error, returns an error message describing the failure reason (e.g., mesh not found, invalid UV channel index).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Floor",
  "uv_channel_index": 2,
  "lod_index": 0
}
```

## Notes

- If `lod_index` is not specified, it defaults to 0 (the highest detail LOD).
- The `uv_channel_index` must refer to an existing UV channel. Removing a non-existent index will result in an error.
- Removing UV channel 0 (the primary UV channel) may affect material rendering.
