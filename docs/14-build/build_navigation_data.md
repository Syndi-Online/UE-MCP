# build_navigation_data

Build AI navigation paths for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Navigation data built successfully."

On error, returns an error message describing why the navigation data build failed.

## Example

```json
{}
```

## Response

### Success
```
Navigation data built successfully.
```

### Error
```
Failed to build navigation data: No world available
```

## Notes

- Builds navigation data (NavMesh) as part of the editor build pipeline.
- This is the build-system variant of navigation building. For direct navigation operations, see `build_navigation` and `rebuild_navigation` in the Navigation / AI section.
- The level must have a NavMeshBoundsVolume placed for navigation data to be generated.
- Part of the full build pipeline that can also be triggered via `build_all`.
