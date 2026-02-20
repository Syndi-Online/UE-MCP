# sequencer_add_actor

Add an actor from the level to the active sequencer as a possessable.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_name | string | Yes | Name or label of the actor to add |

## Returns

On success, returns a confirmation message with the actor name and the binding GUID assigned by the sequencer.

On error, returns an error message describing the failure reason (e.g., actor not found, no active sequencer).

## Example

```json
{
  "actor_name": "DirectionalLight"
}
```

## Notes

- The actor must exist in the current level.
- The `actor_name` can be either the internal actor name or its display label.
- The returned binding GUID can be used with `sequencer_add_track` to add tracks to this actor's binding.
- A "possessable" binding means the sequencer controls an existing actor in the level, as opposed to a "spawnable" which creates a new actor.
