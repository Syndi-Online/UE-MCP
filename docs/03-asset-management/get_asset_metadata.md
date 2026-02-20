# get_asset_metadata

Get metadata tags from an asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to get metadata from |

## Returns

On success, returns the number of metadata tags and a list of key-value pairs for each tag.

On error, returns an error message describing why the metadata could not be retrieved.

## Example

```json
{
  "asset_path": "/Game/Meshes/MyMesh"
}
```

## Notes

- Metadata tags are custom key-value pairs stored on the asset's registry entry.
- Tags can be set using the `set_asset_metadata` tool.
