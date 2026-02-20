# remove_foliage_instances

Remove foliage instances within a sphere or all instances of a mesh type.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | No | Asset path of mesh type to remove (empty for all types) |
| center | object | No | Center of removal sphere with `x`, `y`, `z` (number). Defaults to (0, 0, 0) |
| radius | number | No | Radius of removal sphere (0 to remove all instances of the mesh type) |

## Returns

On success, returns a message indicating how many foliage instances were removed.

On error, returns an error message describing the failure.

## Example

```json
{
  "mesh_path": "/Game/Meshes/MyTree",
  "center": { "x": 100, "y": 200, "z": 0 },
  "radius": 500
}
```

## Response

### Success
```
Removed 15 foliage instances
```

### Error
```
Failed to remove foliage instances: No foliage instances found matching criteria
```

## Notes

- All parameters are optional. If no parameters are provided, the behavior depends on the implementation defaults (center at origin, radius 0).
- Setting `radius` to 0 removes all instances of the specified mesh type regardless of location.
- Omitting `mesh_path` (or passing an empty string) targets all foliage mesh types.
- To remove all foliage in the level, call with no parameters or with `radius` set to 0 and `mesh_path` left empty.
