# replace_widget

Replace a widget's type while preserving its children (if the new type supports children).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the widget to replace |
| new_widget_class | string | Yes | New widget class to replace with |

## Returns

On success, returns `WidgetName`, `OldClass`, and `NewClass`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "HorizontalBox_0",
  "new_widget_class": "VerticalBox"
}
```

## Response

### Success
```
Widget replaced successfully.
WidgetName: HorizontalBox_0
OldClass: HorizontalBox
NewClass: VerticalBox
```

### Error
```
Failed to replace widget: Cannot replace panel widget with leaf widget when children exist
```

## Notes

- Replacing a panel widget with another panel widget preserves children.
- Replacing a panel widget with a leaf widget is only allowed if the panel has no children.
- The widget name is preserved after replacement.
- Widget-specific properties (e.g., text content) are not transferred to the new type.
