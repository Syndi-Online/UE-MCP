# set_actor_transform

Set the transform (location, rotation, scale) of an actor. Only provided components are changed.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path |
| location | object | No | New location {x, y, z}. |
| rotation | object | No | New rotation {pitch, yaw, roll} in degrees. |
| scale | object | No | New scale {x, y, z}. |

### location object

| Field | Type | Description |
|-------|------|-------------|
| x | number | X coordinate |
| y | number | Y coordinate |
| z | number | Z coordinate |

### rotation object

| Field | Type | Description |
|-------|------|-------------|
| pitch | number | Pitch in degrees |
| yaw | number | Yaw in degrees |
| roll | number | Roll in degrees |

### scale object

| Field | Type | Description |
|-------|------|-------------|
| x | number | X scale factor |
| y | number | Y scale factor |
| z | number | Z scale factor |

## Returns

**On success:** Text containing the actor's updated transform -- location (x, y, z), rotation (pitch, yaw, roll), and scale (x, y, z).

**On error:** Error message describing why the transform could not be set (e.g. actor not found).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0",
  "location": { "x": 500.0, "y": 0.0, "z": 100.0 },
  "rotation": { "pitch": 0.0, "yaw": 90.0, "roll": 0.0 },
  "scale": { "x": 2.0, "y": 2.0, "z": 2.0 }
}
```

## Notes

- Only the provided components (location, rotation, scale) are modified. Omitted components remain unchanged.
- At least one of `location`, `rotation`, or `scale` should be provided for the call to have an effect.
- Scale defaults to (1, 1, 1) for individual components if the scale object is provided but fields are omitted.
