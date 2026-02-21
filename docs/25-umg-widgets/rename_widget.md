# rename_widget

Rename a widget in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Current name of the widget |
| new_name | string | Yes | New name for the widget |

## Returns

On success, returns `OldName` and `NewName`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_0",
  "new_name": "TextBlock_Title"
}
```

## Response

### Success
```
Widget renamed successfully.
OldName: TextBlock_0
NewName: TextBlock_Title
```

### Error
```
Failed to rename widget: Name 'TextBlock_Title' is already in use
```

## Notes

- Widget names must be unique within the Widget Blueprint.
- Renaming updates all internal references to the widget.
- Names should follow Unreal naming conventions (alphanumeric + underscores).
