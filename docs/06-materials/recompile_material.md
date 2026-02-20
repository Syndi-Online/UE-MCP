# recompile_material

Recompile a material after modifications.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material to recompile |

## Returns

On success, returns a confirmation that the material was recompiled successfully.

On error, returns a message describing the failure (e.g., material not found, compilation errors).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base"
}
```

## Notes

- Recompilation is necessary after making changes to a material's expression graph (creating, deleting, or connecting expressions).
- This triggers shader recompilation, which updates the material's rendering in the viewport.
- Recompilation may take some time depending on the material complexity and number of shader permutations.
