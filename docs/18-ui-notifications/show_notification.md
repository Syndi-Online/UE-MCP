# show_notification

Show a toast notification in the editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| message | string | Yes | The notification message to display |
| sub_text | string | No | Optional sub-text displayed below the main message |
| duration | number | No | Duration in seconds, default 3.0 |

## Returns

On success, returns the text "Notification shown successfully".

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if the `message` parameter is missing.

## Example

```json
{
  "message": "Build completed",
  "sub_text": "All shaders compiled successfully",
  "duration": 5.0
}
```

## Response

### Success
```
Notification shown successfully
```

### Error
```
Missing required parameter: message
```
```
Failed to create notification
```

## Notes

- The notification appears as a toast popup in the lower-right corner of the Unreal Editor.
- If `duration` is not specified, the notification is displayed for 3 seconds by default.
- If `sub_text` is not provided, only the main message is displayed.
