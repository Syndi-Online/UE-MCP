# set_landscape_weightmap

Import a weightmap file (PNG/RAW) into a landscape paint layer.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| layer_name | string | Yes | Name of the paint layer to update |
| file_path | string | Yes | File path to the weightmap image (PNG or RAW) |
| landscape_name | string | No | Name of the landscape actor |

## Returns

On success, returns a confirmation message indicating the weightmap was imported for the specified layer.

On error, returns an error message describing the failure reason (e.g., landscape not found, layer not found, file not found).

## Example

```json
{
  "layer_name": "Grass",
  "file_path": "D:/Assets/grass_weightmap.png",
  "landscape_name": "Landscape_0"
}
```

## Notes

- The weightmap file must be in PNG or RAW format.
- The `layer_name` must match an existing paint layer on the landscape. Use `get_landscape_weightmap` to list available layers.
- If `landscape_name` is omitted, the tool targets the first landscape actor in the level.
- Weightmap values control the blending of landscape materials. Higher values mean more of that layer is visible.
