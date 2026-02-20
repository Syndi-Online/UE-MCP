# get_actor_property

Get the value of a property on an actor by property name.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path |
| property_name | string | Yes | Name of the property to read (e.g. "bHidden", "Mobility", "Tags") |

## Returns

**On success:** Text containing the property name, its type, and its current value.

**On error:** Error message describing why the property could not be read (e.g. actor not found, property does not exist).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0",
  "property_name": "Mobility"
}
```

## Notes

- The `property_name` must match the UProperty name exactly (case-sensitive).
- The returned value is formatted as a string representation of the property.
- Works with any property exposed to the Unreal reflection system (UProperty/FProperty).
