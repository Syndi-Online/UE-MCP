# export_asset

Export an asset to a file on disk.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to export |
| export_path | string | Yes | Absolute directory on disk to export to |

## Returns

On success, returns a confirmation message with the asset path and export destination.

On error, returns an error message describing why the export failed.

## Example

```json
{
  "asset_path": "/Game/Meshes/MyMesh",
  "export_path": "C:/Exports/Meshes"
}
```

## Response

### Success
```
Asset '/Game/Meshes/MyMesh' exported successfully to 'C:/Exports/Meshes'.
```

### Error
```
Failed to export asset: Asset not found at /Game/Meshes/MyMesh
```

## Notes

- The `export_path` must be an absolute directory path on disk.
- The export format depends on the asset type (e.g., FBX for meshes, PNG for textures).
- The asset must exist at the specified content path.
