# scc_add

Add files to source control.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |

## Returns

On success, returns a text message confirming the files were added to source control.

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if the `files` parameter is missing.

## Example

```json
{
  "files": [
    "/Game/Textures/NewTexture.uasset",
    "/Game/Materials/NewMaterial.uasset"
  ]
}
```

## Response

### Success
```
Marked 2 file(s) for add
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
- Use this to mark new files for addition to version control before submitting.
