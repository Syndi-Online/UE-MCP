# scc_status

Get source control status of files.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message listing each file and its status. Possible status flags per file include:

- **CheckedOut** -- the file is checked out locally.
- **Added** -- the file is marked for add.
- **Deleted** -- the file is marked for deletion.
- **Modified** -- the file has been modified.
- **NotControlled** -- the file is not under source control.
- **LockedBy(username)** -- the file is checked out by another user.
- **UpToDate** -- the file has no pending changes.

Each file is listed in the format `<file_path>: <status flags>`.

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
/Game/Maps/MainLevel.umap: CheckedOut Modified
/Game/Blueprints/BP_Player.uasset: UpToDate
```
```
/Game/Textures/NewTexture.uasset: NotControlled
/Game/Maps/SharedLevel.umap: LockedBy(john.doe)
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
```
Failed to query file states
```

## Notes

- Requires a source control provider to be configured and enabled in the editor.
- Non-string values in the `files` array are silently skipped.
- A file can have multiple status flags at the same time (e.g., both CheckedOut and Modified).
