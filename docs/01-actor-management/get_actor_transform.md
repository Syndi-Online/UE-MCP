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

## Notes

- Location values are in world-space units.
- Rotation values are in degrees (pitch, yaw, roll).
- Scale values are relative (1.0 = default scale).
