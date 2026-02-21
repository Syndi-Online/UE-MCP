# import_widgets

Import widgets from serialized text into a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| exported_text | string | Yes | Serialized widget text from export_widgets |
| parent_name | string | No | Parent widget to import under (root if omitted) |

## Returns

On success, returns the number of widgets imported.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_Settings",
  "exported_text": "<serialized UE widget text>",
  "parent_name": "CanvasPanel_0"
}
```

## Response

### Success
```
Imported 3 widget(s) into WBP_Settings under CanvasPanel_0.
```

### Error
```
Failed to import widgets: Invalid exported text format
```

## Notes

- Uses `FWidgetBlueprintEditorUtils::ImportWidgetsFromText()` for deserialization.
- The `exported_text` must come from `export_widgets` or a compatible UE clipboard export.
- Widget names are auto-resolved if conflicts exist (e.g. "TextBlock_0" becomes "TextBlock_1").
- If `parent_name` is omitted, widgets are added under the root widget.
