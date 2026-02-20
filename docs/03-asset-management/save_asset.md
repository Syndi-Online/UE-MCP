# save_asset

Save an asset to disk.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Path of the asset to save |

## Returns

On success, returns a confirmation message with the saved asset path.

On error, returns an error message describing why the asset could not be saved.

## Example

```json
{
  "asset_path": "/Game/Meshes/MyMesh"
}
```

## Notes

- The asset must be loaded in memory before it can be saved.
- This persists any in-memory changes to the asset's package file on disk.
