# remove_widget

Remove a widget and its children from a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the widget to remove |

## Returns

On success, confirms removal.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_Title"
}
```

## Response

### Success
```
Widget removed successfully: TextBlock_Title
```

### Error
```
Failed to remove widget: Widget 'TextBlock_Title' not found
```

## Notes

- Removing a panel widget also removes all of its children recursively.
- The root widget cannot be removed; use `replace_widget` to change it.
- This operation cannot be undone via MCP (use editor Undo if needed).
