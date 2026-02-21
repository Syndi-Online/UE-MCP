# get_widget_tree

Get the widget hierarchy tree of a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |

## Returns

On success, returns the root widget info and a recursive tree of all widgets with their names, classes, parent names, slot types, and child indices.

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
Widget tree for WBP_MainMenu:
RootWidget: CanvasPanel_0 (CanvasPanel)
  [0] VerticalBox_0 (VerticalBox) [slot: CanvasPanelSlot]
    [0] TextBlock_Title (TextBlock) [slot: VerticalBoxSlot]
    [1] Button_Start (Button) [slot: VerticalBoxSlot]
      [0] TextBlock_StartLabel (TextBlock) [slot: ButtonSlot]
  [1] Image_Background (Image) [slot: CanvasPanelSlot]
TotalWidgets: 5
```

### Error
```
Failed to get widget tree: Asset not found at /Game/UI/WBP_MainMenu
```

## Notes

- The tree is returned recursively, showing the full hierarchy.
- Each widget includes its slot class, indicating how it is positioned in its parent.
- Leaf widgets (TextBlock, Image, etc.) have no children.
