# get_asset_property

Read the value of a UPROPERTY on any asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path |
| property_name | string | Yes | UPROPERTY name |

## Returns

On success, returns the property name and its current value in the format `property_name = value`.

On error, returns an error message describing why the property could not be read.

## Example

```json
{
  "asset_path": "/Game/Sounds/TireSkid_Ice",
  "property_name": "VolumeMultiplier"
}
```

## Response

### Success
```
VolumeMultiplier = 0.750000
```

### Error
```
Failed to get property: Property 'NonExistentProp' not found on asset '/Game/Sounds/TireSkid_Ice'
```

## Notes

- Both parameters are required.
- The `property_name` must match a valid UPROPERTY on the asset's class.
- The returned value is in string format regardless of the underlying property type.
