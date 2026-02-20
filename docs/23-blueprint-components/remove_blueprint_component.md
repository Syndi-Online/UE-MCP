# remove_blueprint_component

Remove a component from a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| component_name | string | Yes | Name of the component to remove |

## Returns

On success, returns a text message confirming removal (e.g. `"Component 'MyLight' removed from blueprint"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "component_name": "MyLight"
}
```

## Response

### Success
```
Component 'MyLight' removed from blueprint
```

### Error
```
Missing required parameter: component_name
```
```
Failed to remove component: Component 'NonExistent' not found
```

## Notes

- The component name must match exactly. Use `get_blueprint_components` to list available components and their names.
- Removing a parent component may affect child components attached to it.
