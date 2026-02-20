# list_assets

List all assets in a directory.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| directory_path | string | Yes | Directory path to list assets from (e.g. '/Game/Meshes') |
| recursive | boolean | No | Whether to list recursively (default: true) |

## Returns

On success, returns a list of asset paths found in the directory.

On error, returns an error message describing why the listing failed.

## Example

```json
{
  "directory_path": "/Game/Meshes",
  "recursive": false
}
```

## Response

### Success
```
Found 3 assets:
- /Game/Meshes/SM_Chair
- /Game/Meshes/SM_Table
- /Game/Meshes/SM_Lamp
```

### Error
```
Failed to list assets: Directory not found: /Game/NonExistent
```

## Notes

- The `directory_path` parameter is required and must be a valid content path.
- By default, the search is recursive and includes all subdirectories.
- Set `recursive` to `false` to list only the assets directly in the specified directory.
