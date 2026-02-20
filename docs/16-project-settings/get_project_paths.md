# get_project_paths

Get important project directory paths.

## Parameters

This tool takes no parameters.

## Returns

On success, returns the following directory paths:
- **Project** -- the root project directory
- **Content** -- the Content directory
- **Config** -- the Config directory
- **Saved** -- the Saved directory
- **Plugins** -- the Plugins directory
- **Intermediate** -- the Intermediate directory
- **Logs** -- the Logs directory
- **Project File** -- the full path to the .uproject file
- **Engine** -- the engine installation directory

On error, returns "Failed to retrieve project paths."

## Example

```json
{}
```

## Notes

- All returned paths are absolute filesystem paths.
- This is useful for discovering the project layout when performing file operations or configuring external tools.
- This tool always succeeds under normal operating conditions.
