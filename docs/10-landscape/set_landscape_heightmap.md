# set_landscape_heightmap

Import a heightmap file (PNG/RAW) into an existing landscape.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| file_path | string | Yes | File path to the heightmap image (PNG or RAW) |
| landscape_name | string | No | Name of the landscape actor |

## Returns

On success, returns a confirmation message indicating the heightmap was imported from the specified file.

On error, returns an error message describing the failure reason (e.g., landscape not found, file not found, dimension mismatch).

## Example

```json
{
  "file_path": "D:/Assets/heightmap.png",
  "landscape_name": "Landscape_0"
}
```

## Notes

- The heightmap file must be in PNG or RAW format.
- The heightmap dimensions should match the existing landscape dimensions. Mismatched sizes may cause errors or unexpected results.
- If `landscape_name` is omitted, the tool targets the first landscape actor in the level.
- This operation modifies the landscape geometry and cannot be easily undone at runtime.
