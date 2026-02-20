# simulate_in_editor

Start a Simulate In Editor session.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text message confirming the SIE session was started.

On error, returns `isError: true` with an error message describing what went wrong.

## Example

```json
{}
```

## Response

### Success
```
Simulate In Editor session requested
```

### Error
```
Editor engine not available
```
```
A play session is already in progress
```

## Notes

- Simulate In Editor (SIE) runs the game simulation while keeping the editor viewport active, allowing you to inspect and modify actors during gameplay.
- Unlike Play In Editor, SIE does not possess a player controller -- the editor camera remains active.
- If a PIE or SIE session is already running, the behavior depends on the engine's PIE module implementation.
- Use `is_playing` to check whether a session is already active before starting a new one.
- Use `stop_pie` to end the simulation session.
