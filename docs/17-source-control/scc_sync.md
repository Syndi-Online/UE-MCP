# scc_sync

Sync files to latest revision from source control.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message confirming the files were synced to the latest revision.

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

## Notes

- Requires a source control provider to be configured and enabled in the editor.
- Non-string values in the `files` array are silently skipped.
- This fetches the latest version of the specified files from the remote repository.
