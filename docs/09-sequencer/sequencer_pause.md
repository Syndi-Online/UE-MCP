# sequencer_pause

Pause playback of the active Level Sequence.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a confirmation message that sequencer playback has been paused.

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- Pausing preserves the current playback position. Use `sequencer_play` to resume from the paused position.
- Use `sequencer_stop` to stop playback and return to the beginning of the sequence.
