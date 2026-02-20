# export_landscape

Export landscape heightmap to a file (PNG format).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| output_file_path | string | Yes | Output file path for the heightmap (e.g. D:/output/heightmap.png) |
| landscape_name | string | No | Name of the landscape actor |

## Returns

On success, returns a confirmation message with the file path where the heightmap was exported.

On error, returns an error message describing the failure reason (e.g., landscape not found, write permission denied).

## Example

```json
{
  "output_file_path": "D:/output/heightmap.png",
  "landscape_name": "Landscape_0"
}
```

## Notes

- The exported file is in PNG format.
- If `landscape_name` is omitted, the tool exports the first landscape actor found in the level.
- The output directory must exist and be writable.
- The exported heightmap can be used with `set_landscape_heightmap` or `import_landscape` to recreate or modify landscapes.
