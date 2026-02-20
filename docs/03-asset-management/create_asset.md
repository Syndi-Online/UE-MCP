# create_asset

Create a new asset of the specified class.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_name | string | Yes | Name of the asset to create |
| package_path | string | Yes | Package path for the asset (e.g. '/Game/Path') |
| asset_class | string | Yes | Full class path of the asset type |

## Returns

On success, returns the created asset information including:
- AssetName
- AssetPath
- AssetClass

On error, returns an error message describing why the asset could not be created.

## Example

```json
{
  "asset_name": "MyNewMaterial",
  "package_path": "/Game/Materials",
  "asset_class": "/Script/Engine.Material"
}
```

## Response

### Success
```
Asset created successfully.
AssetName: MyNewMaterial
AssetPath: /Game/Materials/MyNewMaterial
AssetClass: Material
```

### Error
```
Failed to create asset: Asset already exists at /Game/Materials/MyNewMaterial
```

## Notes

- All three parameters are required. The tool returns an error if any are missing.
- The `asset_class` must be a valid Unreal Engine class path.
- The `package_path` directory will be used as the location for the new asset.
