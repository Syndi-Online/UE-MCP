# set_actor_property

Set the value of a property on an actor. Uses UE text import format for the value.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path |
| property_name | string | Yes | Name of the property to set (e.g. "bHidden", "Mobility") |
| property_value | string | Yes | Value to set (as text, e.g. "true", "1.0", "Movable") |

## Returns

**On success:** Confirmation message that the property was set to the specified value.

**On error:** Error message describing why the property could not be set (e.g. actor not found, property does not exist, invalid value format).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0",
  "property_name": "bHidden",
  "property_value": "true"
}
```

## Response

### Success
```
Property 'bHidden' set to 'true' successfully
```

### Error
```
Failed to set property: Property 'InvalidProperty' not found on actor 'StaticMeshActor_0'
```

## Notes

- The `property_value` must be provided as a string in Unreal Engine text import format.
- Boolean values use "true" / "false".
- Enum values use their display name (e.g. "Movable", "Static", "Stationary" for Mobility).
- Numeric values are passed as string representations (e.g. "1.0", "42").
- The `property_name` must match the UProperty name exactly (case-sensitive).
