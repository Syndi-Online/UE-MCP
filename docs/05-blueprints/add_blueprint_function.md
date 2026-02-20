# add_blueprint_function

Add a function graph to a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| function_name | string | Yes | Name of the function to add |

## Returns

On success, returns `FunctionName` of the newly added function.

On error, returns a message describing the failure (e.g., Blueprint not found, duplicate function name).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "function_name": "CalculateDamage"
}
```

## Notes

- Function names must be unique within the Blueprint.
- The function is created as an empty function graph; you can add nodes to it afterward using Blueprint graph node tools.
- After adding a function, you may need to compile the Blueprint for changes to take effect.
