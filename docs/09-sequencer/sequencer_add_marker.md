# sequencer_add_marker

Add a labeled marker at a specific time on the sequencer timeline.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| time_seconds | number | Yes | Time in seconds where the marker will be placed |
| label | string | Yes | Label for the marker |

## Returns

On success, returns a confirmation message with the marker label, time position, and marker index.

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{
  "time_seconds": 5.0,
  "label": "Explosion Start"
}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- Markers are visual annotations on the timeline and do not affect playback behavior.
- The returned marker index can be used to reference the marker in subsequent operations.
