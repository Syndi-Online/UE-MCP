# get_asset_dependencies

Get the list of assets that a given asset depends on.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to query dependencies for |

## Returns

On success, returns the number of dependencies and a list of asset paths that the specified asset depends on.

On error, returns an error message describing why the query failed.

## Example

```json
{
  "asset_path": "/Game/Blueprints/BP_Character"
}
```

## Response

### Success
```
Asset '/Game/Blueprints/BP_Character' has 3 dependencies:
- /Game/Meshes/SK_Character
- /Game/Materials/M_CharacterBody
- /Game/Textures/T_CharacterDiffuse
```

### Error
```
Failed to get asset dependencies: Asset not found at /Game/Blueprints/BP_Character
```

## Notes

- Dependencies are assets that the queried asset references or uses (e.g., materials used by a mesh, textures used by a material).
- This is the inverse of `get_asset_referencers`.
