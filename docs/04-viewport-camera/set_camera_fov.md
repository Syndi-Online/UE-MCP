# set_camera_fov

Set the viewport camera field of view.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| fov | number | Yes | FOV in degrees (5-170) |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the FOV could not be set.

## Example

```json
{
  "fov": 90.0
}
```

## Response

### Success
```
Camera FOV set to 90.0
```

### Error
```
Failed to set camera FOV: FOV must be between 5 and 170, got: 200.0
```

## Notes

- The FOV value must be between 5 and 170 degrees.
- The default Unreal Engine viewport FOV is typically around 90 degrees.
- Lower values create a telephoto/zoom effect; higher values create a wide-angle effect.
