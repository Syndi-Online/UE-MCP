# get_viewport_camera

Get the current viewport camera position, rotation, and FOV.

## Parameters

This tool takes no parameters.

## Returns

On success, returns the current viewport camera state:
- Location (X, Y, Z)
- Rotation (Pitch, Yaw, Roll)
- FOV (field of view in degrees)

On error, returns an error message describing why the camera information could not be retrieved.

## Example

```json
{}
```

## Notes

- Operates on the active editor viewport.
- The location values are in Unreal Engine world units (centimeters).
- The rotation values are in degrees.
