# set_node_position

Move a node to a new position on the Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph within the Blueprint |
| node_id | string | Yes | ID of the node to move |
| position | object | Yes | New position `{x, y}` |

The `position` object has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| x | number | X coordinate |
| y | number | Y coordinate |

## Returns

On success, returns a text message confirming the new position (e.g. `"Node moved to (200, 300)"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_id": "ABC123-DEF456",
  "position": { "x": 200, "y": 300 }
}
```

## Notes

- Coordinates are integer values in graph space.
- To move multiple nodes at once, use `batch_set_node_positions` instead.
