# add_foliage_instances

Add foliage instances at specified transforms for a given static mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh to use as foliage (e.g. /Game/Meshes/MyTree) |
| transforms | array | Yes | Array of transforms for each instance |

Each element in the `transforms` array is an object with the following optional fields:

| Field | Type | Description |
|-------|------|-------------|
| location | object | Position with `x`, `y`, `z` (number). Defaults to (0, 0, 0) |
| rotation | object | Rotation with `pitch`, `yaw`, `roll` (number). Defaults to (0, 0, 0) |
| scale | object | Scale with `x`, `y`, `z` (number). Defaults to (1, 1, 1) |

## Returns

On success, returns a message indicating how many foliage instances were added and for which mesh.

On error, returns an error message describing the failure (e.g. missing parameters, invalid mesh path).

## Example

```json
{
  "mesh_path": "/Game/Meshes/MyTree",
  "transforms": [
    {
      "location": { "x": 100, "y": 200, "z": 0 },
      "rotation": { "yaw": 45 },
      "scale": { "x": 1, "y": 1, "z": 1 }
    },
    {
      "location": { "x": 500, "y": 300, "z": 0 },
      "rotation": { "yaw": 90 },
      "scale": { "x": 1.5, "y": 1.5, "z": 1.5 }
    }
  ]
}
```

## Notes

- The mesh must already exist as an asset in the project. Use `register_foliage_mesh` first if needed to register it as a foliage type.
- Transform entries that are not valid objects are silently skipped.
- If `location`, `rotation`, or `scale` fields are omitted from a transform entry, they default to zero vector, zero rotator, and unit scale respectively.
