# set_physics_asset

Set or clear the Physics Asset on a skeletal mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the skeletal mesh |
| physics_asset_path | string | Yes | Asset path of the Physics Asset to assign (empty string to clear) |

## Returns

On success, returns a confirmation message indicating the Physics Asset was assigned to or cleared from the skeletal mesh.

On error, returns an error message describing the failure reason (e.g., mesh not found, Physics Asset not found).

## Example

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin",
  "physics_asset_path": "/Game/Characters/PA_Mannequin"
}
```

To clear the Physics Asset:

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin",
  "physics_asset_path": ""
}
```

## Response

### Success (assign)
```
Physics asset '/Game/Characters/PA_Mannequin' assigned to '/Game/Characters/SK_Mannequin'.
```

### Success (clear)
```
Physics asset cleared from '/Game/Characters/SK_Mannequin'.
```

### Error
```
Failed to set physics asset: Asset not found: /Game/Characters/SK_Missing
```

## Notes

- Pass an empty string for `physics_asset_path` to clear (remove) the Physics Asset from the skeletal mesh.
- The Physics Asset must already exist in the project when assigning.
- Changing the Physics Asset affects ragdoll simulation, physical animation, and collision behavior for the skeletal mesh.
