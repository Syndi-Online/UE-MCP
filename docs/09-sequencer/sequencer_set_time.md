# sequencer_set_time

Set the current playback position on the sequencer timeline (in seconds).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| time_seconds | number | Yes | Time position in seconds |

## Returns

On success, returns a confirmation message with the time position that was set.

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{
  "time_seconds": 2.5
}
```

## Response

### Success
```
Sequencer time set to 2.500 seconds.
```

### Error
```
Failed to set time: No active sequencer found
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- The time value is in seconds relative to the sequence start.
- Setting a time outside the playback range is allowed but may not produce visible results.
- Use `sequencer_get_time` to read the current position.
