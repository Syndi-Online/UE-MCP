# register_foliage_mesh

Register a static mesh as a foliage type in the current level.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh to register as foliage type |

## Returns

On success, returns a message with the registered foliage type name.

On error, returns an error message describing the failure (e.g. mesh not found, invalid path).

## Example

```json
{
  "mesh_path": "/Game/Meshes/MyTree"
}
```

## Notes

- The mesh must be a valid static mesh asset in the project.
- Registering a mesh as a foliage type is required before adding foliage instances of that mesh type with `add_foliage_instances`.
- If the mesh is already registered as a foliage type, behavior depends on the engine implementation.
