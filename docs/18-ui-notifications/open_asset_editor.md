# open_asset_editor

Open the editor for an asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to open editor for |

## Returns

On success, returns a message in the format "Editor opened for: <asset_path>".

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if `asset_path` is missing.

## Example

```json
{
  "asset_path": "/Game/Blueprints/BP_Player.uasset"
}
```

## Notes

- The appropriate editor type is determined automatically based on the asset type (e.g., Blueprint Editor for Blueprints, Material Editor for Materials).
- The asset must exist at the specified path; otherwise an error is returned.
