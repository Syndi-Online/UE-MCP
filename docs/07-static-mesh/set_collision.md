# set_collision

Set collision type for a static mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| collision_type | string | Yes | Collision type: Box, Sphere, Capsule, NDOP10_X, NDOP10_Y, NDOP10_Z, NDOP18, NDOP26, ComplexAsSimple, UseSimpleAsComplex, RemoveAll |

## Returns

On success, returns a confirmation message indicating the collision type applied to the mesh.

On error, returns an error message describing the failure reason (e.g., mesh not found, invalid collision type).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Wall",
  "collision_type": "Box"
}
```

## Notes

- Supported collision types:
  - **Box** -- axis-aligned bounding box collision
  - **Sphere** -- sphere collision
  - **Capsule** -- capsule collision
  - **NDOP10_X**, **NDOP10_Y**, **NDOP10_Z** -- 10-DOP simplified collision along the specified axis
  - **NDOP18** -- 18-DOP simplified collision
  - **NDOP26** -- 26-DOP simplified collision
  - **ComplexAsSimple** -- use the complex mesh geometry as simple collision
  - **UseSimpleAsComplex** -- use simple collision for complex queries
  - **RemoveAll** -- remove all collision from the mesh
