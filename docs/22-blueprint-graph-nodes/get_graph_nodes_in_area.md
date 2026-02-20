# get_graph_nodes_in_area

Query graph nodes that intersect a rectangular area. Returns empty array if area is free.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph |
| min_x | number | Yes | Left boundary of area |
| min_y | number | Yes | Top boundary of area |
| max_x | number | Yes | Right boundary of area |
| max_y | number | Yes | Bottom boundary of area |

## Returns

On success with no nodes in the area, returns `"Area is free -- 0 nodes found."`.

On success with nodes found, returns a text listing of found nodes with their node_id, title, position, and size.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "min_x": 0,
  "min_y": 0,
  "max_x": 500,
  "max_y": 400
}
```

## Notes

- The area is defined by its min/max corners in graph space coordinates.
- Useful for checking whether a region is free before placing new nodes, or for finding which nodes exist in a specific area.
- Returns node_id, title, position (x, y), and size (width x height) for each intersecting node.
