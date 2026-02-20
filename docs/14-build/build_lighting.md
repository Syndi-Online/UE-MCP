# build_lighting

Build lighting (lightmaps) for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Lighting build started successfully."

On error, returns an error message describing why the lighting build failed.

## Example

```json
{}
```

## Notes

- Initiates the lighting build process which generates lightmaps for the current level.
- Lighting builds can take a significant amount of time depending on level complexity and quality settings.
- The build is started asynchronously. Use `get_build_status` to check if the lighting build is still in progress.
- Part of the full build pipeline that can also be triggered via `build_all`.
