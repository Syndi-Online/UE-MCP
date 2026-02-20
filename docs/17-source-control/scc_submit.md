# scc_submit

Submit checked out files with a description.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| files | array of string | Yes | Array of file paths |
| description | string | Yes | Changelist description for the submit |

## Returns

On success, returns a text message confirming the files were submitted.

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if the `files` or `description` parameter is missing.

## Example

```json
{
  "files": [
    "/Game/Maps/MainLevel.umap",
    "/Game/Blueprints/BP_Player.uasset"
  ],
  "description": "Fixed player spawn location and updated lighting"
}
```

## Notes

- Requires a source control provider to be configured and enabled in the editor.
- Non-string values in the `files` array are silently skipped.
- Both `files` and `description` are validated independently; a missing `description` is checked only after `files` is validated.
- The files must be checked out before they can be submitted.
