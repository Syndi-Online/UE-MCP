# import_lod

Import a LOD level from an FBX file.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| lod_index | integer | Yes | LOD index to import into |
| source_file | string | Yes | Absolute path to FBX file |

## Returns

On success, returns a confirmation message with the mesh path and the LOD index that was imported.

On error, returns an error message describing the failure reason (e.g., mesh not found, invalid file path, unsupported format).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Building",
  "lod_index": 1,
  "source_file": "D:/Assets/SM_Building_LOD1.fbx"
}
```

## Notes

- The `source_file` must be an absolute path to an FBX file on disk.
- The `lod_index` specifies which LOD slot to import into. LOD 0 is the highest detail level.
- The target mesh must already exist in the project. Use this tool to add or replace LOD levels from external geometry.
