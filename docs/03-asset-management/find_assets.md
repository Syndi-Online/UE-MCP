# find_assets

Find assets by path, class, or tags.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| package_path | string | No | Package path to search in (e.g. '/Game/Meshes') |
| class_name | string | No | Class name to filter by (e.g. 'StaticMesh') |
| recursive | boolean | No | Whether to search recursively (default: true) |
| name_filter | string | No | Wildcard filter for asset name (e.g. '*Repair*', 'BP_Old_Bus_*') |
| limit | integer | No | Maximum number of results (default: 0 = no limit) |

## Returns

On success, returns a list of found assets with the following information for each:
- AssetName
- AssetPath
- AssetClass
- PackagePath

On error, returns an error message describing why the search failed.

## Example

```json
{
  "package_path": "/Game/Meshes",
  "class_name": "StaticMesh",
  "recursive": true,
  "name_filter": "*Wall*",
  "limit": 10
}
```

## Response

### Success
```
Found 3 assets:
- AssetName: SM_Wall_01  AssetPath: /Game/Meshes/SM_Wall_01  AssetClass: StaticMesh  PackagePath: /Game/Meshes
- AssetName: SM_Wall_02  AssetPath: /Game/Meshes/SM_Wall_02  AssetClass: StaticMesh  PackagePath: /Game/Meshes
- AssetName: SM_Wall_Corner  AssetPath: /Game/Meshes/SM_Wall_Corner  AssetClass: StaticMesh  PackagePath: /Game/Meshes
```

### Error
```
Failed to find assets: Invalid package path '/NonExistent'
```

## Notes

- All parameters are optional. Calling with no parameters returns all assets in the project.
- The `name_filter` parameter supports wildcard patterns using `*`.
- Set `recursive` to `false` to search only the specified directory without subdirectories.
- Use `limit` to restrict the number of results for large projects.
