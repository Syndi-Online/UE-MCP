# sequencer_get_time

Get the current playback position from the sequencer timeline.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a JSON object containing:
- **time_seconds** -- current time position in seconds
- **frame_number** -- current frame number
- **frame_rate** -- frame rate of the sequence (frames per second)

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- This is a read-only tool that does not modify the sequencer state.
- The frame number and frame rate are useful for frame-precise positioning. Use `sequencer_set_time` to change the position.
