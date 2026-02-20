# set_mesh_material

Assign a material to a material slot on a static mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| material_index | integer | Yes | Material slot index |
| material_path | string | Yes | Asset path of the material to assign |

## Returns

On success, returns a confirmation message indicating the material path, slot index, and mesh path.

On error, returns an error message describing the failure reason (e.g., mesh not found, material not found, invalid slot index).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Wall",
  "material_index": 0,
  "material_path": "/Game/Materials/M_BrickWall"
}
```

## Response

### Success
```
Material '/Game/Materials/M_BrickWall' assigned to slot 0 on '/Game/Meshes/SM_Wall'.
```

### Error
```
Failed to set mesh material: Asset not found: /Game/Meshes/SM_Missing
```

## Notes

- The `material_index` is zero-based. Use `get_lod_settings` or inspect the mesh to determine how many material slots are available.
- The material at `material_path` must already exist in the project.
- This modifies the material assignment on the static mesh asset itself, not on individual instances in the level.
