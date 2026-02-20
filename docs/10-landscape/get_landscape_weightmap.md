# get_landscape_weightmap

Get landscape weight/paint layer names.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| landscape_name | string | No | Name of the landscape actor (empty for first landscape in level) |

## Returns

On success, returns a JSON object containing:
- **layers** -- array of paint layer names used by the landscape

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
```json
{
    "layers":
    [
        "Grass",
        "Rock",
        "Sand"
    ]
}
```

### Error
```
Failed to get weightmap: No landscape found in level
```

## Notes

- If `landscape_name` is omitted or empty, the tool uses the first landscape actor found in the current level.
- This is a read-only tool that does not modify the landscape.
- The returned layer names can be used with `set_landscape_weightmap` to update specific paint layers.
