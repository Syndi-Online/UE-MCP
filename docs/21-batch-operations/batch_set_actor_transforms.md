# batch_set_actor_transforms

Set transforms (location, rotation, scale) on multiple actors in one call. Up to 100 operations per batch.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| operations | array | Yes | Array of operations. Each has actor_identifier and optional location, rotation, scale. |

Each object in the `operations` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| actor_identifier | string | Identifier of the actor (name or label) |
| location | object | Optional. Location as `{x, y, z}` |
| rotation | object | Optional. Rotation as `{pitch, yaw, roll}` in degrees |
| scale | object | Optional. Scale as `{x, y, z}` |

## Returns

On success, returns a text message with the number of succeeded and failed operations (e.g. `"Batch completed: 3 succeeded, 0 failed"`). The `isError` flag is `true` only when all operations fail.

On error (missing or invalid parameters), returns an error message describing the issue.

If any individual operations fail, the errors are listed with their index and error message.

## Example

```json
{
  "operations": [
    {
      "actor_identifier": "Cube_1",
      "location": { "x": 100, "y": 200, "z": 0 },
      "rotation": { "pitch": 0, "yaw": 45, "roll": 0 },
      "scale": { "x": 1, "y": 1, "z": 1 }
    },
    {
      "actor_identifier": "Sphere_0",
      "location": { "x": -50, "y": 0, "z": 300 }
    }
  ]
}
```

## Response

### Success
```
Batch completed: 2 succeeded, 0 failed
```

### Error
```
Missing required parameter: operations (array)
```
```
Too many operations. Maximum 100 per batch.
```
```
Batch completed: 1 succeeded, 1 failed

Errors:
- [0] Actor not found: MissingActor
```

## Notes

- Maximum of 100 operations per batch. Exceeding this limit returns an error.
- Each transform component (location, rotation, scale) is optional. Only provided components are updated.
- If `location` is omitted, the actor's position is not changed. Same for `rotation` and `scale`.
- Default scale values when the `scale` object is provided are `{x: 1, y: 1, z: 1}`.
- Default location values when the `location` object is provided are `{x: 0, y: 0, z: 0}`.
- Default rotation values when the `rotation` object is provided are `{pitch: 0, yaw: 0, roll: 0}`.
- Each operation is executed independently; a failure in one does not stop the others.
- The `isError` flag is only set to `true` when all operations fail (succeeded == 0 and failed > 0).
