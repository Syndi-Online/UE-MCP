# scc_delete

Mark files for deletion in source control.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message confirming the files were marked for deletion.

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if the `files` parameter is missing.

## Example

```json
{
  "files": [
    "/Game/OldAssets/DeprecatedMesh.uasset"
  ]
}
```

## Response

### Success
```
Marked 1 file(s) for delete
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
- This marks files for deletion in the source control system; the actual deletion is committed on submit.
