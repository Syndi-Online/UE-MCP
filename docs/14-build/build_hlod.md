# build_hlod

Build Hierarchical LOD clusters for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "HLOD built successfully."

On error, returns an error message describing why the HLOD build failed.

## Example

```json
{}
```

## Notes

- HLOD (Hierarchical Level of Detail) combines multiple static meshes into clustered proxy meshes for improved rendering performance at distance.
- The level must have HLOD settings configured (World Settings > World Partition / HLOD) for this to have any effect.
- Part of the full build pipeline that can also be triggered via `build_all`.
