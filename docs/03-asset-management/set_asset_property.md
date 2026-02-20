# set_asset_property

Set the value of a UPROPERTY on any asset (SoundWave, Material, DataAsset, etc.)

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| asset_path | string | Yes | Asset path, e.g. /Game/Sounds/TireSkid_Ice |
| property_name | string | Yes | UPROPERTY name, e.g. bLooping, VolumeMultiplier |
| property_value | string | Yes | Value in string format |

## Returns

On success, returns a confirmation message indicating the property was set on the asset.

On error, returns an error message describing why the property could not be set.

## Example

```json
{
  "asset_path": "/Game/Sounds/TireSkid_Ice",
  "property_name": "VolumeMultiplier",
  "property_value": "0.75"
}
```

## Response

### Success
```
Property 'VolumeMultiplier' set on asset '/Game/Sounds/TireSkid_Ice'
```

### Error
```
Failed to set property: Property 'NonExistentProp' not found on asset '/Game/Sounds/TireSkid_Ice'
```

## Notes

- All three parameters are required.
- The `property_name` must match a valid UPROPERTY on the asset's class.
- The `property_value` is provided as a string and will be converted to the appropriate type internally.
- Works with any asset type that has exposed UPROPERTY fields (SoundWave, Material, DataAsset, etc.).
