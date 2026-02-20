# set_asset_metadata

Set a metadata tag on an asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path to set metadata on |
| tag_name | string | Yes | Name of the metadata tag |
| tag_value | string | Yes | Value of the metadata tag |

## Returns

On success, returns a confirmation message with the tag name, value, and asset path.

On error, returns an error message describing why the metadata could not be set.

## Example

```json
{
  "asset_path": "/Game/Meshes/MyMesh",
  "tag_name": "Author",
  "tag_value": "John"
}
```

## Notes

- All three parameters are required.
- If a tag with the same name already exists, its value will be overwritten.
- Metadata tags can be retrieved using the `get_asset_metadata` tool.
