# create_widget_blueprint

Create a new Widget Blueprint with an optional root widget type.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path for the new Widget Blueprint, e.g. "/Game/UI/WBP_MainMenu" |
| root_widget_class | string | No | Root widget class (default: CanvasPanel). Options: CanvasPanel, VerticalBox, HorizontalBox, Overlay, GridPanel, ScrollBox |

## Returns

On success, returns `BlueprintName`, `BlueprintPath`, and `RootWidgetClass`.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "root_widget_class": "CanvasPanel"
}
```

## Response

### Success
```
Widget Blueprint created successfully.
BlueprintName: WBP_MainMenu
BlueprintPath: /Game/UI/WBP_MainMenu
RootWidgetClass: CanvasPanel
```

### Error
```
Failed to create Widget Blueprint: Asset already exists at /Game/UI/WBP_MainMenu
```

## Notes

- The parent class is automatically set to `UserWidget`.
- If `root_widget_class` is omitted, `CanvasPanel` is used by default.
- The `blueprint_path` must be a valid content path starting with `/Game/`.
- The root widget is the top-level container for all other widgets in the hierarchy.
