# build_navigation

Build navigation mesh for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Navigation mesh built successfully."

On error, returns an error message describing why the navigation build failed.

## Example

```json
{}
```

## Response

### Success
```
Navigation mesh built successfully.
```

### Error
```
Failed to build navigation mesh: Navigation system not available
```

## Notes

- Builds the navigation mesh (NavMesh) for the currently loaded level.
- The level must have a NavMeshBoundsVolume placed for navigation data to be generated.
- This performs an incremental build. Use `rebuild_navigation` for a full rebuild from scratch.
