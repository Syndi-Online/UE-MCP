# take_screenshot

Take a screenshot of the viewport.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| filename | string | No | Output file path (optional, auto-generated if empty) |

## Returns

On success, returns the file path where the screenshot was saved.

On error, returns an error message describing why the screenshot could not be taken.

## Example

```json
{
  "filename": "C:/Screenshots/viewport_capture.png"
}
```

## Response

### Success
```
Screenshot saved to: C:/Screenshots/viewport_capture.png
```

### Error
```
Failed to take screenshot: No active viewport client
```

## Notes

- If `filename` is not provided or is empty, the file path will be auto-generated.
- The screenshot captures the current state of the active editor viewport.
- No parameters are required; the tool can be called with an empty object to use auto-generated filename.
