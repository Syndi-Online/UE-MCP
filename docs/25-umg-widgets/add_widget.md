# add_widget

Add a widget to a parent panel in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_class | string | Yes | Widget class to add (e.g. TextBlock, Button, Image, VerticalBox, HorizontalBox, CanvasPanel, Overlay, SizeBox, Border, ScrollBox, CheckBox, Slider, ProgressBar, EditableTextBox, ComboBoxString, Spacer, RichTextBlock) |
| parent_name | string | Yes | Name of the parent panel widget to add to |
| widget_name | string | No | Custom name for the new widget (auto-generated if omitted) |
| insert_index | integer | No | Index at which to insert the child (-1 or omit for end) |

## Returns

On success, returns `WidgetName`, `WidgetClass`, and `ParentName`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_class": "TextBlock",
  "parent_name": "CanvasPanel_0",
  "widget_name": "TextBlock_Title"
}
```

## Response

### Success
```
Widget added successfully.
WidgetName: TextBlock_Title
WidgetClass: TextBlock
ParentName: CanvasPanel_0
```

### Error
```
Failed to add widget: Parent widget 'CanvasPanel_0' not found
```

## Notes

- The parent must be a panel widget (UPanelWidget subclass) that supports children.
- If `widget_name` is omitted, a unique name is auto-generated (e.g. "TextBlock_0").
- Single-child containers (SizeBox, Border, Button) can only hold one child.
- Use `set_widget_property` after adding to configure the widget's properties.
- Use `set_widget_slot` after adding to configure positioning (anchors, padding, etc.).
