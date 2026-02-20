# set_comment_box_properties

Change properties of a Comment Box (text, color, position, size).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_id | string | Yes | Node ID of the comment box to modify |
| comment_text | string | No | New text for the comment box |
| color | object | No | New color `{r, g, b, a}` as 0.0-1.0 floats. |
| position | object | No | New position `{x, y}`. |
| size | object | No | New size `{width, height}`. |

The `color` object:

| Field | Type | Description |
|-------|------|-------------|
| r | number | Red component (0.0 to 1.0) |
| g | number | Green component (0.0 to 1.0) |
| b | number | Blue component (0.0 to 1.0) |
| a | number | Alpha component (0.0 to 1.0) |

The `position` object:

| Field | Type | Description |
|-------|------|-------------|
| x | number | X coordinate |
| y | number | Y coordinate |

The `size` object:

| Field | Type | Description |
|-------|------|-------------|
| width | number | Width of the comment box |
| height | number | Height of the comment box |

## Returns

On success, returns `"Comment box properties updated"`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_id": "ABC123-DEF456",
  "comment_text": "Updated comment text",
  "color": { "r": 1.0, "g": 0.0, "b": 0.0, "a": 1.0 },
  "position": { "x": 100, "y": 200 },
  "size": { "width": 500, "height": 300 }
}
```

## Response

### Success
```
Comment box properties updated
```

### Error
```
Missing required parameter: node_id
```
```
Failed to update comment box properties: Node not found in graph
```

## Notes

- All optional parameters (`comment_text`, `color`, `position`, `size`) are independent. Only the provided ones are updated.
- If none of the optional parameters are provided, the call succeeds but nothing changes.
- Color values are linear color floats in the 0.0 to 1.0 range. Defaults to `{1.0, 1.0, 1.0, 1.0}` (white) if individual color fields are omitted.
