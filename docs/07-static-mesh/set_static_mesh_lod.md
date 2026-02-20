# set_static_mesh_lod

Set LOD screen size thresholds for a static mesh.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the static mesh |
| screen_sizes | array (number) | Yes | Screen size thresholds per LOD, e.g. [1.0, 0.5, 0.25] |

## Returns

On success, returns a confirmation message indicating the mesh path and the number of LOD levels updated.

On error, returns an error message describing the failure reason (e.g., mesh not found, invalid path).

## Example

```json
{
  "mesh_path": "/Game/Meshes/SM_Rock",
  "screen_sizes": [1.0, 0.5, 0.25]
}
```

## Response

### Success
```
LOD screen sizes updated for '/Game/Meshes/SM_Rock' with 3 LOD level(s).
```

### Error
```
Failed to set LOD screen sizes: Asset not found: /Game/Meshes/SM_Missing
```

## Notes

- The `screen_sizes` array length determines how many LOD levels are configured. Each entry corresponds to a LOD index.
- Screen size values typically range from 0.0 to 1.0, where 1.0 means full screen coverage and smaller values trigger lower LOD levels.
- The array values should be in descending order (highest detail first).
