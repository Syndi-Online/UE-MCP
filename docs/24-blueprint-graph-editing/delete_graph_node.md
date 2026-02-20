# delete_graph_node

Delete a node from a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_id | string | Yes | Node ID (GUID) of the node to delete |

## Returns

On success, returns a text message confirming deletion (e.g. `"Node ABC123-DEF456 deleted from graph 'EventGraph'"`).

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
Node 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678 deleted from graph 'EventGraph'
```

### Error
```
Missing required parameter: node_id
```
```
Failed to delete node: Node not found in graph
```

## Notes

- Deleting a node automatically disconnects all its pins.
- Use `get_graph_nodes` or `get_graph_nodes_summary` to find the node ID before deleting.
- This operation cannot be undone via the MCP API. Use Unreal Editor's undo system if available.
