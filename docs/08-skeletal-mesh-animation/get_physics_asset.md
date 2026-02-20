# get_physics_asset

Get the Physics Asset assigned to a skeletal mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the skeletal mesh |

## Returns

On success, returns a JSON object containing:
- **physics_asset_path** -- asset path of the assigned Physics Asset
- **physics_asset_name** -- name of the Physics Asset
- **num_bodies** -- number of physics bodies in the asset
- **num_constraints** -- number of constraints in the asset

On error, returns an error message describing the failure reason (e.g., mesh not found, no Physics Asset assigned).

## Example

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin"
}
```

## Notes

- This is a read-only tool that does not modify any assets.
- If no Physics Asset is assigned to the skeletal mesh, the tool may return an error or empty path depending on the implementation.
- Use `set_physics_asset` to assign or change the Physics Asset.
