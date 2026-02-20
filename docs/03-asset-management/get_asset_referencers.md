# get_asset_referencers

Get the list of assets that reference a given asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to query referencers for |

## Returns

On success, returns the number of referencers and a list of asset paths that reference the specified asset.

On error, returns an error message describing why the query failed.

## Example

```json
{
  "asset_path": "/Game/Textures/MyTexture"
}
```

## Response

### Success
```
Asset '/Game/Textures/MyTexture' is referenced by 2 assets:
- /Game/Materials/M_Wall
- /Game/Materials/M_Floor
```

### Error
```
Failed to get asset referencers: Asset not found at /Game/Textures/MyTexture
```

## Notes

- Referencers are assets that use or depend on the queried asset (e.g., materials that use a specific texture).
- This is the inverse of `get_asset_dependencies`.
- Useful for checking if an asset is safe to delete or modify.
