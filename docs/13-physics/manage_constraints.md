# manage_constraints

Create a physics constraint between two actors.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_name_1 | string | Yes | Name of the first actor |
| actor_name_2 | string | Yes | Name of the second actor |
| constraint_type | string | Yes | Type of constraint: weld, hinge, ball, or free |
| disable_collision | boolean | No | Disable collision between constrained actors. Defaults to true |

## Returns

On success, returns a message with the constraint type and the name of the created constraint.

On error, returns an error message describing the failure (e.g. actors not found, invalid constraint type).

## Example

```json
{
  "actor_name_1": "Cube_0",
  "actor_name_2": "Cube_1",
  "constraint_type": "hinge",
  "disable_collision": true
}
```

## Response

### Success
```
Created hinge constraint: PhysicsConstraintActor_0
```

### Error
```
Missing required parameter: actor_name_1
```
```
Missing required parameter: actor_name_2
```
```
Missing required parameter: constraint_type
```
```
Failed to create constraint: Actor 'InvalidActor' not found
```

## Notes

- Valid constraint types are:
  - `weld` -- locks all degrees of freedom between the two actors
  - `hinge` -- allows rotation around a single axis
  - `ball` -- allows rotation around all axes (ball-and-socket joint)
  - `free` -- no motion restrictions (useful for setting up custom constraints)
- By default, collision between the two constrained actors is disabled to prevent physics jitter.
- Both actors must exist in the current level and have physics-capable components.
