# get_widget_animations

List all widget animations in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |

## Returns

On success, returns a list of animations with their names, start times, and end times.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu"
}
```

## Response

### Success
```
Animations in WBP_MainMenu:
  FadeIn: 0.00s - 1.00s
  SlideIn: 0.00s - 0.50s
  ButtonPulse: 0.00s - 2.00s
TotalAnimations: 3
```

### Error
```
Failed to get widget animations: Asset not found at /Game/UI/WBP_MainMenu
```

## Notes

- Returns only animation metadata (name and time range), not keyframe details.
- Animations are created in the UMG editor's animation timeline.
- This tool is read-only; creating or modifying animations requires the editor UI or future dedicated tools.
