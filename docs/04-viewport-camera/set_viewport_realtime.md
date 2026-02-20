# set_viewport_realtime

Toggle realtime rendering in the viewport.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| enabled | boolean | Yes | Whether to enable or disable realtime rendering |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the realtime setting could not be changed.

## Example

```json
{
  "enabled": true
}
```

## Notes

- When realtime rendering is enabled, the viewport continuously updates (animations play, particles simulate, etc.).
- When disabled, the viewport only updates when interacted with, reducing resource usage.
- This is equivalent to toggling the "Realtime" button in the viewport toolbar.
