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

## Response

### Success (no nodes found)
```
Area is free — 0 nodes found.
```

### Success (nodes found)
```
Found 2 nodes in area:
- node_id: 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678  title: Event BeginPlay  position: (0, 0)  size: (200 x 75)
- node_id: F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6  title: Print String  position: (300, 0)  size: (250 x 100)
```

### Error
```
Missing required parameters: blueprint_path, graph_name, min_x, min_y, max_x, max_y
```
```
Failed: Graph 'NonExistent' not found in Blueprint
```

## Notes

- The area is defined by its min/max corners in graph space coordinates.
- Useful for checking whether a region is free before placing new nodes, or for finding which nodes exist in a specific area.
- Returns node_id, title, position (x, y), and size (width x height) for each intersecting node.
