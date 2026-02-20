# get_graph_nodes

Get all nodes in a Blueprint graph with their id, class, title, position, size, pins, and comment text.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph within the Blueprint |
| node_ids | array of strings | No | If specified, return full data only for nodes with these GUIDs. Otherwise return all nodes. |

## Returns

On success, returns a text message with the node count and a JSON array of node objects. Each node object contains:

- `node_id` (string) -- GUID of the node
- `node_class` (string) -- class type of the node
- `node_title` (string) -- display title
- `position` (object) -- `{x, y}` coordinates
- `size` (object) -- `{width, height}` dimensions
- `comment` (string) -- comment text (if any)
- `pins` (array) -- array of pin objects, each with:
  - `pin_id` (string)
  - `pin_name` (string)
  - `pin_type` (string)
  - `direction` (string)
  - `connected_pin_ids` (array of strings)

If `node_ids` filter is provided, the response indicates how many nodes are returned out of the total.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph"
}
```

With node ID filter:

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_ids": ["ABC123-DEF456", "GHI789-JKL012"]
}
```

## Notes

- Use the `node_ids` filter to reduce response size when you only need data for specific nodes.
- Without a filter, all nodes in the graph are returned, which can be large for complex Blueprints.
- For a lightweight overview without pin and position data, use `get_graph_nodes_summary` instead.
