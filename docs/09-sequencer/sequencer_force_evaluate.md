# sequencer_force_evaluate

Force the sequencer to re-evaluate at the current time.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a confirmation message that the sequencer was force-evaluated at the current time.

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- This is useful when you need the sequencer to refresh its evaluation after making changes to tracks, keyframes, or bindings without moving the playhead.
- Force evaluation updates all bound actors and properties to reflect the current timeline state.
