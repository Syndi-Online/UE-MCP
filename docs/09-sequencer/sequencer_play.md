# sequencer_play

Start playback of the active Level Sequence in the editor.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a confirmation message that sequencer playback has started.

On error, returns an error message describing the failure reason (e.g., no active sequencer, no sequence loaded).

## Example

```json
{}
```

## Response

### Success
```
Sequencer playback started.
```

### Error
```
Failed to start playback: No active sequencer found
```

## Notes

- A Level Sequence must be open in the Sequencer editor for this tool to work.
- If the sequencer is already playing, calling this tool again has no additional effect.
- Use `sequencer_pause` to pause and `sequencer_stop` to stop playback.
