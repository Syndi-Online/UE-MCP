# move_widget

Move a widget to a different parent panel in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the widget to move |
| new_parent_name | string | Yes | Name of the new parent panel widget |
| insert_index | integer | No | Index at which to insert in the new parent (-1 or omit for end) |

## Returns

On success, returns `WidgetName` and `NewParentName`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_Title",
  "new_parent_name": "VerticalBox_0",
  "insert_index": 0
}
```

## Response

### Success
```
Widget moved successfully.
WidgetName: TextBlock_Title
NewParentName: VerticalBox_0
```

### Error
```
Failed to move widget: Cannot move widget into its own descendant
```

## Notes

- The widget is removed from its current parent and added to the new parent.
- All children of the moved widget move with it.
- The new parent must be a panel widget that supports children.
- Moving a widget into its own descendant is not allowed (prevents circular references).
