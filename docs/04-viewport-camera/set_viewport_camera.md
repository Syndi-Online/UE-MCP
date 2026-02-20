# set_viewport_camera

Set the viewport camera position and rotation.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| location | object | Yes | Camera position with `x`, `y`, `z` (number) properties |
| rotation | object | Yes | Camera rotation with `pitch`, `yaw`, `roll` (number) properties |

## Returns

On success, returns the new camera location and rotation values.

On error, returns an error message describing why the camera could not be set.

## Example

```json
{
  "location": {
    "x": 100.0,
    "y": 200.0,
    "z": 300.0
  },
  "rotation": {
    "pitch": -15.0,
    "yaw": 45.0,
    "roll": 0.0
  }
}
```

## Notes

- Both `location` and `rotation` are required parameters.
- Location values are in Unreal Engine world units (centimeters).
- Rotation values are in degrees.
- Operates on the active editor viewport.
