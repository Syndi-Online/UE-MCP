# play_in_editor

Start a Play In Editor session.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text message confirming the PIE session was started.

On error, returns `isError: true` with an error message describing what went wrong.

## Example

```json
{}
```

## Notes

- Starts a PIE (Play In Editor) session using the current editor settings.
- If a PIE or SIE session is already running, the behavior depends on the engine's PIE module implementation.
- Use `is_playing` to check whether a session is already active before starting a new one.
- Use `stop_pie` to end the session.
