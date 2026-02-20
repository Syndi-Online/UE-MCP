# add_uv_channel

Add a UV channel to a static mesh LOD.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| lod_index | integer | No | LOD index (default 0) |

## Returns

On success, returns a confirmation message with the mesh path, LOD index, and the new total number of UV channels.

On error, returns an error message describing the failure reason (e.g., mesh not found, maximum UV channels reached).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Floor",
  "lod_index": 0
}
```

## Notes

- If `lod_index` is not specified, it defaults to 0 (the highest detail LOD).
- UV channels are added at the end of the existing channel list.
- Unreal Engine has a maximum number of UV channels per mesh. Adding beyond that limit will fail.
