# set_pin_default_value

Set the default value of a pin on a node in a Blueprint graph.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| graph_name | string | Yes | Name of the graph (e.g. EventGraph) |
| node_id | string | Yes | Node ID (GUID) of the target node |
| pin_name | string | Yes | Name of the pin to set the default value on |
| default_value | string | Yes | Default value to set (as string) |

## Returns

On success, returns a text message confirming the value was set (e.g. `"Default value of pin 'InString' on node ABC123 set to: Hello World"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "graph_name": "EventGraph",
  "node_id": "ABC123-DEF456",
  "pin_name": "InString",
  "default_value": "Hello World"
}
```

## Response

### Success
```
Default value of pin 'InString' on node 4A8B12C3-4D5E6F78-9A0B1C2D-3E4F5678 set to: Hello World
```

### Error
```
Missing required parameter: default_value
```
```
Failed to set pin default value: Pin 'NonExistent' not found on node
```

## Notes

- Default values are provided as strings and are converted internally by Unreal Engine based on the pin type.
- Only unconnected input pins can have default values set. If a pin is connected, its value comes from the connection.
- Use `get_graph_nodes` to find pin names and their current states.
