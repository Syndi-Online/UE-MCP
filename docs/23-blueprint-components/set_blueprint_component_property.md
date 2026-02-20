# set_blueprint_component_property

Set a property value on a Blueprint component.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| component_name | string | Yes | Name of the component |
| property_name | string | Yes | Name of the property to set |
| property_value | string | Yes | Value to set the property to (as string) |

## Returns

On success, returns a text message confirming the property was set (e.g. `"Property 'Intensity' set on component 'MyLight'"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "component_name": "MyLight",
  "property_name": "Intensity",
  "property_value": "5000.0"
}
```

## Response

### Success
```
Property 'Intensity' set on component 'MyLight'
```

### Error
```
Missing required parameter: property_value
```
```
Failed to set component property: Component 'NonExistent' not found
```

## Notes

- Property values are passed as strings and converted internally by Unreal Engine.
- Use `get_blueprint_components` to find available component names.
- Common properties depend on the component class (e.g. `Intensity` for light components, `StaticMesh` for mesh components, `RelativeLocation` for scene components).
