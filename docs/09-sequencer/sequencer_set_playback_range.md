# sequencer_set_playback_range

Set the playback range (start and end time in seconds) for the sequence.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| start_time_seconds | number | Yes | Start time in seconds |
| end_time_seconds | number | Yes | End time in seconds |

## Returns

On success, returns a confirmation message with the start and end times that were set.

On error, returns an error message describing the failure reason (e.g., no active sequencer, invalid range).

## Example

```json
{
  "start_time_seconds": 0.0,
  "end_time_seconds": 10.0
}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- The `start_time_seconds` must be less than `end_time_seconds`.
- This sets the green playback range markers in the Sequencer UI.
- Playback will loop or stop within this range depending on the sequencer's loop settings.
