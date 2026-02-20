# is_playing

Check if a Play In Editor session is active.

## Parameters

This tool takes no parameters.

## Returns

Returns a text message with two status flags:

- **Playing** -- `true` if a PIE session is currently active, `false` otherwise.
- **Simulating** -- `true` if a SIE (Simulate In Editor) session is currently active, `false` otherwise.

The response format is: `Playing: true/false, Simulating: true/false`.

This tool always succeeds (`isError` is always `false`).

## Example

```json
{}
```

## Response

### Success
```
Playing: false, Simulating: false
```
```
Playing: true, Simulating: false
```
```
Playing: false, Simulating: true
```

## Notes

- This is a read-only query tool that does not start or stop any session.
- Both `Playing` and `Simulating` can be `false` when no session is running.
- Use this tool to determine the current editor play state before calling `play_in_editor`, `simulate_in_editor`, or `stop_pie`.
