# add_graph_nodes_batch

Create multiple graph nodes and connections in a single call.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph |
| nodes | array | Yes | Array of nodes to create (each with local_id, node_type, member_name, target, pos_x, pos_y) |
| connections | array | No | Array of connections: {source, source_pin, target, target_pin} using local_ids |

Each object in the `nodes` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| local_id | string | A temporary ID you assign, used to reference this node in the connections array |
| node_type | string | Type of node (CallFunction, Event, VariableGet, VariableSet, etc.) |
| member_name | string | Function/variable/macro name (optional, depends on node_type) |
| target | string | Target class/type (optional, depends on node_type) |
| pos_x | number | X position on the graph (optional) |
| pos_y | number | Y position on the graph (optional) |

Each object in the `connections` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| source | string | local_id of the source node |
| source_pin | string | Pin name on the source node |
| target | string | local_id of the target node |
| target_pin | string | Pin name on the target node |

## Returns

On success, returns a text message with the number of created nodes and connections, along with a mapping of local_id to actual node_id and pin count for each node.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "nodes": [
    {
      "local_id": "event",
      "node_type": "Event",
      "member_name": "BeginPlay"
    },
    {
      "local_id": "print",
      "node_type": "CallFunction",
      "member_name": "PrintString",
      "target": "KismetSystemLibrary",
      "pos_x": 300,
      "pos_y": 0
    }
  ],
  "connections": [
    {
      "source": "event",
      "source_pin": "then",
      "target": "print",
      "target_pin": "execute"
    }
  ]
}
```

## Response

### Success
```
Created 2 nodes, 1 connections:
- event -> node_id: 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678 (3 pins)
- print -> node_id: F1E2D3C4-B5A6F7E8-D9C0B1A2-F3E4D5C6 (5 pins)
```

### Error
```
Missing required parameters: blueprint_path, graph_name
```
```
Missing or empty required parameter: nodes
```
```
Batch failed: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- The `local_id` fields are temporary identifiers used only within this call to define connections between nodes being created. They are not persisted.
- Nodes are created first, then connections are made between them using the local_id mapping.
- The `connections` array is optional. If omitted, only nodes are created with no connections.
- Both `local_id` and `node_type` are required for each node entry; entries missing either are skipped.
- The response maps each `local_id` to the actual `node_id` (GUID) assigned by the engine.
