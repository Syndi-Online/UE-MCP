# create_physics_collision

Add a collision shape (box, sphere, or capsule) to an actor's static mesh body setup.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_name | string | Yes | Name of the actor |
| shape_type | string | Yes | Type of shape: box, sphere, or capsule |
| dimensions | object | Yes | Shape dimensions with `x`, `y`, `z` (number). Box: half-extents XYZ. Sphere: radius in X. Capsule: radius in X and length in Y |
| offset | object | No | Local offset of the shape with `x`, `y`, `z` (number). Defaults to (0, 0, 0) |

## Returns

On success, returns a message indicating the shape type added and the total number of shapes on the body.

On error, returns an error message describing the failure (e.g. actor not found, invalid shape type).

## Example

```json
{
  "actor_name": "StaticMeshActor_0",
  "shape_type": "box",
  "dimensions": { "x": 50, "y": 50, "z": 50 },
  "offset": { "x": 0, "y": 0, "z": 25 }
}
```

## Response

### Success
```
Added box collision shape (3 shape(s) total)
```

### Error
```
Missing required parameter: actor_name
```
```
Missing required parameter: shape_type
```
```
Missing required parameter: dimensions
```
```
Failed to create physics collision: Actor 'InvalidActor' not found
```

## Notes

- Valid shape types are: `box`, `sphere`, and `capsule`.
- For `box`, dimensions represent half-extents along each axis (X, Y, Z).
- For `sphere`, only the `x` component is used as the radius.
- For `capsule`, `x` is the radius and `y` is the half-length.
- The actor must have a static mesh component with a body setup.
- Multiple shapes can be added to the same actor by calling this tool repeatedly.
