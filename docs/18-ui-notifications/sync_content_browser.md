# sync_content_browser

Navigate Content Browser to a path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| path | string | Yes | Asset or folder path to navigate to |

## Returns

On success, returns a message in the format "Content Browser navigated to: <path>".

On error, returns `isError: true` with a message in the format "Failed to sync Content Browser: <error>". Returns an error if `path` is missing.

## Example

```json
{
  "path": "/Game/Blueprints"
}
```

## Notes

- This focuses the Content Browser and navigates it to the specified asset or folder path.
- Can be used with both folder paths (to navigate to a directory) and asset paths (to select a specific asset).
