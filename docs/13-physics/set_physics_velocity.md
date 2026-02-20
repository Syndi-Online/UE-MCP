# set_physics_velocity

Set linear and angular velocity on an actor's physics body.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_name | string | Yes | Name of the actor |
| linear_velocity | object | No | Linear velocity in cm/s with `x`, `y`, `z` (number). Defaults to (0, 0, 0) |
| angular_velocity | object | No | Angular velocity in degrees/s with `x`, `y`, `z` (number). Defaults to (0, 0, 0) |
| enable_physics | boolean | No | Enable physics simulation before setting velocity. Defaults to true |

## Returns

On success, returns a confirmation message that the physics velocity was set on the actor.

On error, returns an error message describing the failure (e.g. actor not found, no physics body).

## Example

```json
{
  "actor_name": "Cube_0",
  "linear_velocity": { "x": 0, "y": 0, "z": 500 },
  "angular_velocity": { "x": 0, "y": 90, "z": 0 },
  "enable_physics": true
}
```

## Notes

- By default, physics simulation is enabled on the actor before setting velocity. Set `enable_physics` to `false` if the actor already has physics enabled and you do not want to reset it.
- Linear velocity is specified in cm/s (Unreal units per second).
- Angular velocity is specified in degrees/s.
- The actor must have a primitive component capable of physics simulation.
