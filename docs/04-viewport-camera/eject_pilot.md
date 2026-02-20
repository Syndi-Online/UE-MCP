# eject_pilot

Eject from the currently piloted actor.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the ejection failed.

## Example

```json
{}
```

## Notes

- This stops piloting mode and returns the viewport camera to free movement.
- If no actor is currently being piloted, the tool may return an error or no-op message.
- Use `pilot_actor` to start piloting an actor.
