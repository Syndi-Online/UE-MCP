# load_asset

Load an asset by its path and return its info.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path (e.g. '/Game/Meshes/MyMesh') |

## Returns

On success, returns the loaded asset information including:
- AssetName
- AssetPath
- AssetClass

On error, returns an error message describing why the asset could not be loaded.

## Example

```json
{
  "asset_path": "/Game/Meshes/MyMesh"
}
```

## Notes

- The asset must exist at the specified path. If the asset is not found, the tool returns an error.
- Uses Unreal Engine's asset registry to locate and load the asset.
