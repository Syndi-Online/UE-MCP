# set_material_instance_parent

Change the parent material of a Material Instance.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| instance_path | string | Yes | Asset path of the Material Instance |
| parent_path | string | Yes | Asset path of the new parent material |

## Returns

On success, returns a confirmation that the material instance parent was set successfully.

On error, returns a message describing the failure (e.g., instance not found, parent material not found).

## Example

```json
{
  "instance_path": "/Game/Materials/MI_Red",
  "parent_path": "/Game/Materials/M_NewBase"
}
```

## Response

### Success
```
Material instance parent set successfully.
```

### Error
```
Failed to set material instance parent: Material Instance not found: /Game/Materials/MI_Missing
```

## Notes

- The `parent_path` can point to either a Material or another Material Instance (creating a Material Instance chain).
- Changing the parent may invalidate existing parameter overrides if the new parent does not expose parameters with the same names.
- After changing the parent, verify that the Material Instance parameters are still valid.
