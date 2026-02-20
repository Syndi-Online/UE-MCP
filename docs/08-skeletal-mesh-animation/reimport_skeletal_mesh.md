# reimport_skeletal_mesh

Reimport a skeletal mesh from its original source file.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the skeletal mesh to reimport |

## Returns

On success, returns a confirmation message indicating the skeletal mesh was reimported successfully.

On error, returns an error message describing the failure reason (e.g., mesh not found, source file missing, import error).

## Example

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin"
}
```

## Response

### Success
```
Skeletal mesh '/Game/Characters/SK_Mannequin' reimported successfully.
```

### Error
```
Failed to reimport skeletal mesh: Source file not found for asset '/Game/Characters/SK_Mannequin'
```

## Notes

- The skeletal mesh must have been originally imported with a source file reference. If the original source file path is no longer valid, the reimport will fail.
- This tool uses the existing import settings stored with the asset.
- Reimporting will update the mesh geometry, skeleton, and related data from the source file.
