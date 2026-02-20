# get_mesh_bounds

Get the bounding box of a static mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |

## Returns

On success, returns bounding box information including:
- **Origin** -- center point of the bounding box (x, y, z)
- **BoxExtent** -- half-size of the bounding box in each axis (x, y, z)
- **SphereRadius** -- radius of the bounding sphere

On error, returns an error message describing the failure reason (e.g., mesh not found).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Table"
}
```

## Notes

- This is a read-only tool that does not modify the mesh.
- The bounds are in local space of the mesh asset, not world space.
- The sphere radius represents the smallest sphere that fully encloses the mesh from its origin.
