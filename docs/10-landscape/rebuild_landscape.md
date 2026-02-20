# rebuild_landscape

Rebuild landscape data: collision, grass maps, and force layer updates.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| landscape_name | string | No | Name of the landscape actor (empty for first landscape in level) |

## Returns

On success, returns a confirmation message that the landscape data was rebuilt successfully.

On error, returns an error message describing the failure reason (e.g., no landscape found in level).

## Example

```json
{
  "landscape_name": "Landscape_0"
}
```

Using the first landscape in the level:

```json
{}
```

## Response

### Success
```
Landscape data rebuilt successfully.
```

### Error
```
Failed to rebuild landscape: No landscape found in level
```

## Notes

- If `landscape_name` is omitted or empty, the tool rebuilds the first landscape actor found in the current level.
- Rebuilding updates collision meshes, grass/foliage maps, and forces all landscape layers to refresh.
- This is useful after modifying heightmap or weightmap data to ensure all derived data is up to date.
- The rebuild operation may take some time for large landscapes.
