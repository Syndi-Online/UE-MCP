# get_build_status

Get the current build status.

## Parameters

This tool takes no parameters.

## Returns

Returns a status report with the following fields:
- **Building** -- whether any build process is currently active (yes/no)
- **Lighting Build** -- whether a lighting build is in progress (yes/no)
- **Navigation Build** -- whether a navigation build is in progress (yes/no)
- **Cancelled** -- whether the last build was cancelled (yes/no)

## Example

```json
{}
```

## Response

### Success
```
Build Status:
- Building: yes
- Lighting Build: yes
- Navigation Build: no
- Cancelled: no
```

## Notes

- Use this tool to check whether an asynchronous build operation (such as `build_lighting` or `build_all`) is still running.
- This tool always succeeds and never returns an error.
- Useful for polling build completion before proceeding with other operations.
