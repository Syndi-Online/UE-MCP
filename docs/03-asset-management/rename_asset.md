# rename_asset

Rename or move an asset to a new path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| source_path | string | Yes | Current asset path |
| destination_path | string | Yes | New asset path |

## Returns

On success, returns the new path of the renamed asset:
- NewPath

On error, returns an error message describing why the asset could not be renamed.

## Example

```json
{
  "source_path": "/Game/Meshes/OldName",
  "destination_path": "/Game/Meshes/NewName"
}
```

## Response

### Success
```
Asset renamed successfully.
NewPath: /Game/Meshes/NewName
```

### Error
```
Failed to rename asset: Source asset not found at /Game/Meshes/OldName
```

## Notes

- This tool can be used both to rename an asset and to move it to a different directory.
- Both source and destination paths are required.
- The source asset must exist at the specified path.
- References to the asset may need to be updated after renaming.
