# rebuild_navigation

Rebuild navigation mesh from scratch.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Navigation mesh rebuilt successfully."

On error, returns an error message describing why the navigation rebuild failed.

## Example

```json
{}
```

## Notes

- Unlike `build_navigation`, this performs a full rebuild of the navigation mesh from scratch rather than an incremental update.
- Use this when the navigation data appears stale or when significant level changes have been made.
- The level must have a NavMeshBoundsVolume placed for navigation data to be generated.
