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

## Response

### Success
```
Graph has 3 nodes.
[{"node_id":"4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678","node_class":"K2Node_Event","node_title":"Event BeginPlay","position":{"x":0,"y":0},"size":{"width":200,"height":75},"comment":"","pins":[{"pin_id":"A1B2C3D4-E5F6A7B8-C9D0E1F2-A3B4C5D6","pin_name":"then","pin_type":"exec","direction":"output","connected_pin_ids":["F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6"]}]}]
```

With node_ids filter:
```
Returning 1 of 3 total nodes.
[{"node_id":"4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678","node_class":"K2Node_Event","node_title":"Event BeginPlay","position":{"x":0,"y":0},"size":{"width":200,"height":75},"comment":"","pins":[]}]
```

### Error
```
Missing required parameter: graph_name
```
```
Failed to get graph nodes: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- Use the `node_ids` filter to reduce response size when you only need data for specific nodes.
- Without a filter, all nodes in the graph are returned, which can be large for complex Blueprints.
- For a lightweight overview without pin and position data, use `get_graph_nodes_summary` instead.
