# get_random_reachable_point

Get a random reachable point on the navigation mesh within a radius.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| origin | object | Yes | Center of search area with `x`, `y`, `z` (number) |
| radius | number | Yes | Search radius |

## Returns

On success, returns the coordinates (X, Y, Z) of a random reachable point on the navigation mesh.

On error, returns an error message describing why no reachable point could be found.

## Example

```json
{
  "origin": { "x": 0, "y": 0, "z": 100 },
  "radius": 2000
}
```

## Notes

- Navigation mesh must be built before using this tool.
- The returned point is guaranteed to be on the navigation mesh and reachable from the origin.
- A larger radius increases the search area but the point will still be within that radius of the origin.
- If no navigable area exists within the radius, the tool returns an error.
