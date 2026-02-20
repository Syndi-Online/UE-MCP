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

## Notes

- This is a lighter-weight alternative to `get_graph_nodes`. It does not return pin details, positions, or sizes.
- Use `class_filter` to narrow results to a specific node type (e.g. `K2Node_CallFunction`, `K2Node_Event`).
- Connected node IDs are computed by resolving pin connections across all nodes.
