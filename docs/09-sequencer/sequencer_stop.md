# sequencer_stop

Stop playback and return to the beginning of the sequence.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a confirmation message that the sequencer has stopped and returned to the start.

On error, returns an error message describing the failure reason (e.g., no active sequencer).

## Example

```json
{}
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- Unlike `sequencer_pause`, stopping resets the playback position to the beginning of the sequence.
