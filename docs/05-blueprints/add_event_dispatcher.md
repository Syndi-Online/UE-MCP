# add_event_dispatcher

Add an Event Dispatcher (multicast delegate) to a Blueprint with optional parameters.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| dispatcher_name | string | Yes | Name of the Event Dispatcher to create |
| parameters | array | No | Optional array of parameters for the dispatcher signature |

Each item in the `parameters` array is an object with the following fields:

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| name | string | Yes | Parameter name |
| type | string | Yes | Type: Boolean, Integer, Int64, Float, String, Name, Text, Vector, Rotator, Transform, Byte, or class path |

## Returns

On success, returns `dispatcher_name` and `graph_name` of the created Event Dispatcher.

On error, returns a message describing the failure (e.g., Blueprint not found, duplicate dispatcher name).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "dispatcher_name": "OnHealthChanged",
  "parameters": [
    { "name": "NewHealth", "type": "Float" },
    { "name": "OldHealth", "type": "Float" }
  ]
}
```

## Response

### Success
```
Event Dispatcher created successfully.
dispatcher_name: OnHealthChanged
graph_name: OnHealthChanged
```

### Error
```
Failed to add event dispatcher: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- Event Dispatchers are multicast delegates that can be bound to by other Blueprints or code.
- The `parameters` array is optional. If omitted, the dispatcher will have no parameters.
- Supported parameter types: `Boolean`, `Integer`, `Int64`, `Float`, `String`, `Name`, `Text`, `Vector`, `Rotator`, `Transform`, `Byte`, or a class path for object references.
- Dispatcher names must be unique within the Blueprint.
