# export_widgets

Export widgets from a Widget Blueprint as serialized text.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_names | array | No | Names of specific widgets to export (exports all if omitted) |

## Returns

On success, returns the serialized text representation of the widgets.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_names": ["VerticalBox_0", "TextBlock_Title"]
}
```

## Response

### Success
```
Exported 2 widget(s) from WBP_MainMenu.
ExportedText: <serialized UE widget text>
```

### Error
```
Failed to export widgets: Widget 'InvalidWidget' not found
```

## Notes

- Uses `FWidgetBlueprintEditorUtils::ExportWidgetsToText()` for serialization.
- The exported text can be used with `import_widgets` to paste widgets into the same or different Widget Blueprint.
- Exporting a panel widget includes all of its children.
- The text format is UE's internal clipboard format and is not human-readable.
