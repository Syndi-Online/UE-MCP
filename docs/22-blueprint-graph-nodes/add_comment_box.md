# add_comment_box

Add a Comment Box to a Blueprint graph. Optionally wraps specified nodes.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| comment_text | string | Yes | Text displayed in the comment box |
| position | object | No | Position of the comment box `{x, y}`. Default (0, 0). |
| size | object | No | Size of the comment box `{width, height}`. Default (400, 200). |
| color | object | No | Color of the comment box `{r, g, b, a}` as 0.0-1.0 floats. |
| node_ids | array of strings | No | Array of node IDs to wrap with the comment box |

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

The `color` object:

| Field | Type | Description |
|-------|------|-------------|
| r | number | Red component (0.0 to 1.0) |
| g | number | Green component (0.0 to 1.0) |
| b | number | Blue component (0.0 to 1.0) |
| a | number | Alpha component (0.0 to 1.0) |

## Returns

On success, returns a text message with the node ID of the created comment box (e.g. `"Comment box created with ID: ABC123-DEF456"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "comment_text": "Player Input Handling",
  "position": { "x": -100, "y": -50 },
  "size": { "width": 600, "height": 400 },
  "color": { "r": 0.2, "g": 0.5, "b": 0.8, "a": 1.0 },
  "node_ids": ["ABC123-DEF456", "GHI789-JKL012"]
}
```

## Response

### Success
```
Comment box created with ID: 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678
```

### Error
```
Missing required parameter: comment_text
```
```
Failed to add comment box: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- If `position` is omitted, the comment box is placed at (0, 0).
- If `size` is omitted, the default size is 400 x 200.
- If `node_ids` is provided, the comment box wraps the specified nodes. The position and size may be adjusted to encompass them.
- Color values are linear color floats in the 0.0 to 1.0 range.
