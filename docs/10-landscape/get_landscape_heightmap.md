# get_landscape_heightmap

Get landscape heightmap info: dimensions, scale, extent, and paint layer names.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| landscape_name | string | No | Name of the landscape actor (empty for first landscape in level) |

## Returns

On success, returns a JSON object containing:
- **min_x**, **min_y** -- minimum coordinates of the heightmap extent
- **max_x**, **max_y** -- maximum coordinates of the heightmap extent
- **width** -- heightmap width in samples
- **height** -- heightmap height in samples
- **scale** -- landscape scale object with x, y, z components
- **layers** -- array of paint layer names

On error, returns an error message describing the failure reason (e.g., no landscape found in level).

## Example

```json
{
  "landscape_name": "Landscape_0"
}
```

Using the first landscape in the level:

```json
{}
```

## Response

### Success
```json
{
    "min_x": 0,
    "min_y": 0,
    "max_x": 504,
    "max_y": 504,
    "width": 505,
    "height": 505,
    "scale":
    {
        "x": 100,
        "y": 100,
        "z": 100
    },
    "layers":
    [
        "Grass",
        "Rock",
        "Sand"
    ]
}
```

### Error
```
Failed to get heightmap: No landscape found in level
```

## Notes

- If `landscape_name` is omitted or empty, the tool uses the first landscape actor found in the current level.
- This is a read-only tool that does not modify the landscape.
- The scale values represent the landscape actor's transform scale, which affects how heightmap data maps to world space.
