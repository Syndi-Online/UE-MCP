# stop_pie

Stop the current Play In Editor session.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text message confirming the PIE session was stopped.

On error, returns `isError: true` with an error message describing what went wrong.

## Example

```json
{}
```

## Notes

- Stops both PIE (Play In Editor) and SIE (Simulate In Editor) sessions.
- If no session is currently running, the behavior depends on the engine's PIE module implementation.
- Use `is_playing` to check whether a session is active before stopping.
