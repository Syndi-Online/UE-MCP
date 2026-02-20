# get_lod_settings

Get LOD settings for a static mesh (count, screen sizes, vertex/triangle counts).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |

## Returns

On success, returns LOD settings information including:
- LOD count
- LOD group name
- Per-LOD details: LOD index, screen size threshold, vertex count, and triangle count

On error, returns an error message describing the failure reason (e.g., mesh not found).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Chair"
}
```

## Response

### Success
```
LOD settings for '/Game/Meshes/SM_Chair':
LodCount: 3
LodGroup: SmallProp
  LOD 0: ScreenSize=1.0000, Vertices=1250, Triangles=800
  LOD 1: ScreenSize=0.5000, Vertices=600, Triangles=400
  LOD 2: ScreenSize=0.2500, Vertices=200, Triangles=120
```

### Error
```
Failed to get LOD settings: Asset not found: /Game/Meshes/SM_Missing
```

## Notes

- This is a read-only tool that does not modify the mesh.
- The returned information is useful for understanding the current LOD configuration before making changes with `set_static_mesh_lod` or `import_lod`.
