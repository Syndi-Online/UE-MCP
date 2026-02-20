# get_blueprint_component_property

Get a property value from a Blueprint component.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| component_name | string | Yes | Name of the component |
| property_name | string | Yes | Name of the property to get |

## Returns

On success, returns a text message with the property value (e.g. `"Property 'Intensity' on component 'MyLight': 5000.000000"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "component_name": "MyLight",
  "property_name": "Intensity"
}
```

## Response

### Success
```
Property 'Intensity' on component 'MyLight': 5000.000000
```

### Error
```
Missing required parameter: property_name
```
```
Failed to get component property: Component 'NonExistent' not found
```

## Notes

- The property value is returned as a string representation.
- Use `get_blueprint_components` to find available component names.
