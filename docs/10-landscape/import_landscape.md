# import_landscape

Import a landscape from a heightmap file.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| heightmap_file_path | string | Yes | File path to the heightmap image (PNG or RAW) |
| location | object | No | World location {x, y, z} for the landscape |
| scale | object | No | Scale {x, y, z} for the landscape (default: 100, 100, 100) |

## Returns

On success, returns a confirmation message indicating the landscape heightmap data was loaded from the specified file.

On error, returns an error message describing the failure reason (e.g., file not found, invalid format).

## Example

```json
{
  "heightmap_file_path": "D:/Assets/terrain_heightmap.png",
  "location": { "x": 0, "y": 0, "z": 0 },
  "scale": { "x": 100, "y": 100, "z": 100 }
}
```

Minimal example with defaults:

```json
{
  "heightmap_file_path": "D:/Assets/terrain_heightmap.png"
}
```

## Response

### Success
```
Landscape heightmap data loaded from 'D:/Assets/terrain_heightmap.png'.
```

### Error
```
Failed to import landscape: File not found: D:/Assets/missing_heightmap.png
```

## Notes

- The heightmap file must be in PNG or RAW format.
- If `location` is not specified, the landscape is placed at the world origin (0, 0, 0).
- If `scale` is not specified, the default scale of (100, 100, 100) is used.
- The `location` and `scale` objects must contain `x`, `y`, and `z` number fields.
- This creates a new landscape actor in the current level from the provided heightmap data.
