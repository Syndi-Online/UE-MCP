# get_blueprint_components

Get all components of a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |

## Returns

On success, returns a text message with the component count and a JSON array of component objects. Each object contains:

- `name` (string) -- name of the component
- `class` (string) -- component class type
- `parent` (string) -- name of the parent component
- `is_root` (boolean) -- whether this is the root component

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor"
}
```

## Notes

- Returns all components in the Blueprint's component hierarchy.
- The root component is indicated by the `is_root` field being `true`.
- Use the component names from this response with `set_blueprint_component_property`, `get_blueprint_component_property`, and `remove_blueprint_component`.
