# scc_checkout

Check out files from source control.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message confirming the checkout operation completed.

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
Checked out 2 file(s)
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
