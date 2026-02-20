# delete_comment_box

Delete a Comment Box from a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_id | string | Yes | Node ID of the comment box to delete |

## Returns

On success, returns `"Comment box deleted"`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_id": "ABC123-DEF456"
}
```

## Response

### Success
```
Comment box deleted
```

### Error
```
Missing required parameter: node_id
```
```
Failed to delete comment box: Node not found in graph
```

## Notes

- The `node_id` must refer to a comment box node. Use `get_graph_nodes` to find comment box node IDs.
