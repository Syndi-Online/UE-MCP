# get_actor_transform

Get the transform (location, rotation, scale) of an actor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path |

## Returns

**On success:** Text containing the actor's full transform -- location (x, y, z), rotation (pitch, yaw, roll), and scale (x, y, z).

**On error:** Error message describing why the transform could not be retrieved (e.g. actor not found).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0"
}
```

## Response

### Success
```
Transform for 'StaticMeshActor_0':
Location: (100.000, 200.000, 300.000)
Rotation: (Pitch=0.000, Yaw=45.000, Roll=0.000)
Scale: (1.000, 1.000, 1.000)
```

### Error
```
Failed to get transform: Actor 'NonExistentActor' not found
```

## Notes

- Location values are in world-space units.
- Rotation values are in degrees (pitch, yaw, roll).
- Scale values are relative (1.0 = default scale).
