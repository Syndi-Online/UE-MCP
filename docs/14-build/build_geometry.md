# build_geometry

Build BSP geometry for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Geometry built successfully."

On error, returns an error message describing why the geometry build failed.

## Example

```json
{}
```

## Notes

- Builds BSP (Binary Space Partitioning) geometry for the current level.
- This is relevant for levels using BSP brushes. Static mesh-based levels typically do not require geometry building.
- Part of the full build pipeline that can also be triggered via `build_all`.
