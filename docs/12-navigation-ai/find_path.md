# find_path

Find a navigation path between two world positions.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| start | object | Yes | Start position with `x`, `y`, `z` (number) |
| end | object | Yes | End position with `x`, `y`, `z` (number) |

## Returns

On success, returns path information including:
- Whether the path is partial or complete
- The total number of waypoints
- A list of all waypoint coordinates (X, Y, Z)

On error, returns an error message describing why pathfinding failed.

## Example

```json
{
  "start": { "x": 0, "y": 0, "z": 100 },
  "end": { "x": 1000, "y": 500, "z": 100 }
}
```

## Notes

- Navigation mesh must be built before using this tool. Use `build_navigation` or `rebuild_navigation` first.
- A partial path is returned when the destination is not fully reachable but the closest reachable point can be found.
- The returned waypoints are world-space coordinates along the navigation mesh.
- If start or end coordinates are omitted, they default to (0, 0, 0).
