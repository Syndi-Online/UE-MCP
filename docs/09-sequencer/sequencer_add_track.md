# sequencer_add_track

Add a track to the sequencer (Transform, Visibility, or class path). Optionally bind to an actor GUID.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| track_type | string | Yes | Track type: Transform, Visibility, or full class path |
| binding_guid | string | No | Optional binding GUID from sequencer_add_actor (omit for unbound track) |

## Returns

On success, returns a confirmation message with the name of the added track.

On error, returns an error message describing the failure reason (e.g., invalid track type, binding not found, no active sequencer).

## Example

Adding a transform track to a bound actor:

```json
{
  "track_type": "Transform",
  "binding_guid": "A1B2C3D4-E5F6-7890-ABCD-EF1234567890"
}
```

Adding an unbound track:

```json
{
  "track_type": "Visibility"
}
```

## Notes

- Common track types include:
  - **Transform** -- controls actor location, rotation, and scale
  - **Visibility** -- controls actor visibility
  - Full class paths can be used for other track types (e.g., custom tracks)
- If `binding_guid` is provided, the track is added to that actor's binding in the sequencer. Use `sequencer_add_actor` to obtain the GUID.
- If `binding_guid` is omitted, the track is added as an unbound (master) track.
