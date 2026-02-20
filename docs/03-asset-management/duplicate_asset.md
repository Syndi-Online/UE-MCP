# duplicate_asset

Duplicate an asset to a new path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| source_path | string | Yes | Source asset path to duplicate |
| destination_path | string | Yes | Destination path for the duplicated asset |

## Returns

On success, returns the duplicated asset information including:
- AssetName
- AssetPath

On error, returns an error message describing why the asset could not be duplicated.

## Example

```json
{
  "source_path": "/Game/Meshes/MyMesh",
  "destination_path": "/Game/Meshes/MyMesh_Copy"
}
```

## Notes

- Both source and destination paths are required.
- The source asset must exist at the specified path.
- If an asset already exists at the destination path, the operation may fail.
