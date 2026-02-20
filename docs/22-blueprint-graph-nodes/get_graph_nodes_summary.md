# get_graph_nodes_summary

Get a lightweight summary of all nodes in a Blueprint graph -- id, class, title, and connected node ids. No pins, no positions, no sizes.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph within the Blueprint |
| class_filter | string | No | If specified, return only nodes of this class |

## Returns

On success, returns a text message with the node count and a JSON array of summary node objects. Each object contains:

- `node_id` (string) -- GUID of the node
- `node_class` (string) -- class type of the node
- `node_title` (string) -- display title
- `connected_node_ids` (array of strings) -- IDs of nodes connected to this node (derived from pin connections)

If `class_filter` is used, the response shows filtered count out of total.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph"
}
```

With class filter:

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "class_filter": "K2Node_CallFunction"
}
```

## Response

### Success
```
Graph has 3 nodes.
[{"node_id":"4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678","node_class":"K2Node_Event","node_title":"Event BeginPlay","connected_node_ids":["F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6"]}]
```

With class_filter:
```
Graph has 1 nodes (filtered from 3 total).
[{"node_id":"F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6","node_class":"K2Node_CallFunction","node_title":"Print String","connected_node_ids":["4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678"]}]
```

### Error
```
Missing required parameter: graph_name
```
```
Failed to get graph nodes summary: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- This is a lighter-weight alternative to `get_graph_nodes`. It does not return pin details, positions, or sizes.
- Use `class_filter` to narrow results to a specific node type (e.g. `K2Node_CallFunction`, `K2Node_Event`).
- Connected node IDs are computed by resolving pin connections across all nodes.
