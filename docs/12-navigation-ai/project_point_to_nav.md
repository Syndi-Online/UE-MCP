# project_point_to_nav

Project a world position onto the navigation mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| point | object | Yes | World position to project with `x`, `y`, `z` (number) |
| extent | object | No | Search extent with `x`, `y`, `z` (number). Defaults to (50, 50, 250) |

## Returns

On success, returns the projected location coordinates (X, Y, Z) on the navigation mesh.

On error, returns an error message describing why the projection failed (e.g. no nav mesh nearby).

## Example

```json
{
  "point": { "x": 100, "y": 200, "z": 500 },
  "extent": { "x": 100, "y": 100, "z": 500 }
}
```

## Notes

- Navigation mesh must be built before using this tool.
- The `extent` parameter defines the search box size around the point. If no navigation mesh exists within this extent, the projection fails.
- Default extent of (50, 50, 250) works well for most cases, but increase it if the point is far from any navigable surface.
- This is useful for snapping arbitrary world positions to valid navigation locations.
