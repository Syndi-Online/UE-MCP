# get_project_info

Get information about the current project.

## Parameters

This tool takes no parameters.

## Returns

On success, returns project information including:
- **Project Name** -- the name of the project
- **Project Version** -- the version string of the project
- **Description** -- the project description
- **Company Name** -- the company or organization name
- **Engine Version** -- the Unreal Engine version
- **Project File Path** -- the full path to the .uproject file

On error, returns an error message.

## Example

```json
{}
```

## Notes

- This tool provides a quick overview of the currently loaded project.
- The information is sourced from the project's .uproject file and engine settings.
- This tool always succeeds unless the project data is unavailable.
