# scc_revert

Revert files to source control version.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message confirming the files were reverted to their source control version.

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if the `files` parameter is missing.

## Example

```json
{
  "files": [
    "/Game/Maps/MainLevel.umap",
    "/Game/Blueprints/BP_Player.uasset"
  ]
}
```

## Response

### Success
```
Reverted 2 file(s)
```

### Error
```
Missing required parameter: files
```
```
Source control is not enabled
```
```
Source control provider is not available
```

## Notes

- Requires a source control provider to be configured and enabled in the editor.
- Non-string values in the `files` array are silently skipped.
- Reverting discards all local changes and restores the files to their last synced revision.
