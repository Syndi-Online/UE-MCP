# spawn_actor

Spawn an actor in the editor world from a class path. Returns the spawned actor's name, label, class, location and rotation.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_class | string | Yes | Full class path, e.g. "/Script/Engine.StaticMeshActor" or "/Script/Engine.PointLight" |
| location | object | Yes | World location {x, y, z}. Each coordinate is a number. |
| rotation | object | No | World rotation {pitch, yaw, roll} in degrees. Defaults to zero. |

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

## Returns

**On success:** Text containing the spawned actor's name, label, class, location (x, y, z), and rotation (pitch, yaw, roll).

**On error:** Error message describing why the spawn failed (e.g. invalid class path, missing parameters).

## Example

```json
{
  "actor_class": "/Script/Engine.PointLight",
  "location": { "x": 100.0, "y": 200.0, "z": 300.0 },
  "rotation": { "pitch": 0.0, "yaw": 45.0, "roll": 0.0 }
}
```

## Response

### Success
```
Actor spawned successfully.
Name: StaticMeshActor_1
Label: StaticMeshActor
Class: /Script/Engine.StaticMeshActor
Location: (100.0, 200.0, 300.0)
Rotation: (Pitch=0.0, Yaw=45.0, Roll=0.0)
```

### Error
```
Failed to spawn actor: Class '/Script/Engine.InvalidClass' not found
```

## Notes

- The `actor_class` must be a full class path (e.g. "/Script/Engine.StaticMeshActor"), not a short name.
- If `rotation` is omitted, the actor is spawned with zero rotation.
- The actor is spawned in the current editor world.
