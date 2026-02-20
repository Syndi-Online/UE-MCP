# build_texture_streaming

Build texture streaming data for the current level.

## Parameters

This tool takes no parameters.

## Returns

On success, returns "Texture streaming data built successfully."

On error, returns an error message describing why the texture streaming build failed.

## Example

```json
{}
```

## Notes

- Builds texture streaming data which helps the engine efficiently manage texture memory by streaming textures in and out based on distance and screen coverage.
- This improves runtime performance and memory usage for texture-heavy levels.
- Part of the full build pipeline that can also be triggered via `build_all`.
