# get_blueprint_parent_class

Get the parent class and parent Blueprint of a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |

## Returns

On success, returns `parent_class` (the class path of the parent class) and `parent_blueprint` (the asset path of the parent Blueprint, or `null` if the parent is a native C++ class).

On error, returns a message describing the failure (e.g., Blueprint not found).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor"
}
```

## Notes

- If the Blueprint inherits from a C++ class directly (e.g., Actor, Character), `parent_blueprint` will be `null`.
- If the Blueprint inherits from another Blueprint, `parent_blueprint` will contain the asset path of that parent Blueprint.
- This tool is useful for inspecting class hierarchy before reparenting or adding interfaces.
