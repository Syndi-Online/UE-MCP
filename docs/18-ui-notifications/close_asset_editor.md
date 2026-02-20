# close_asset_editor

Close the editor for an asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to close editor for |

## Returns

On success, returns a message in the format "Editor closed for: <asset_path>".

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if `asset_path` is missing.

## Example

```json
{
  "asset_path": "/Game/Blueprints/BP_Player.uasset"
}
```

## Notes

- Only closes the editor if it is currently open for the specified asset.
- The asset must exist at the specified path; otherwise an error is returned.
