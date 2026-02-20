# pilot_actor

Pilot (lock camera to) an actor in the viewport.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | The actor identifier (name or label) to pilot |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the piloting could not be started.

## Example

```json
{
  "actor_identifier": "CameraActor_1"
}
```

## Response

### Success
```
Now piloting actor: CameraActor_1
```

### Error
```
Failed to pilot actor: Actor not found: CameraActor_1
```

## Notes

- Piloting locks the viewport camera to the specified actor's position and rotation.
- This is equivalent to right-clicking an actor in the World Outliner and selecting "Pilot".
- Commonly used with CameraActor to preview camera shots.
- Use `eject_pilot` to stop piloting and return to free camera movement.
