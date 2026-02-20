# build_all

Perform a full build (geometry, HLOD, navigation, texture streaming, lighting).

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Full build started successfully."

On error, returns an error message describing why the full build failed to start.

## Example

```json
{}
```

## Response

### Success
```
Full build started successfully.
```

### Error
```
Failed to start full build: No world available
```

## Notes

- This triggers a complete build of all level data including geometry, HLOD, navigation, texture streaming, and lighting.
- Equivalent to using Build > Build All Levels in the Unreal Editor menu.
- The build is started asynchronously. Use `get_build_status` to monitor progress.
- For building individual components, use the dedicated tools: `build_geometry`, `build_lighting`, `build_navigation_data`, `build_hlod`, or `build_texture_streaming`.
