# delete_asset

Delete an asset by path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Path of the asset to delete |

## Returns

On success, returns a confirmation message that the asset was deleted.

On error, returns an error message describing why the asset could not be deleted.

## Example

```json
{
  "asset_path": "/Game/Meshes/UnusedMesh"
}
```

## Response

### Success
```
Asset '/Game/Meshes/UnusedMesh' deleted successfully.
```

### Error
```
Failed to delete asset: Asset not found at /Game/Meshes/UnusedMesh
```

## Notes

- The asset must exist at the specified path.
- Deleting an asset that is referenced by other assets may cause issues. Consider checking referencers first using `get_asset_referencers`.
