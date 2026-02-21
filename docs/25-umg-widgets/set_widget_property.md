# set_widget_property

Set a property value on a widget in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the target widget |
| property_name | string | Yes | Property name (e.g. Text, ColorAndOpacity, Font, Visibility, IsEnabled, RenderOpacity, ToolTipText, bIsVariable) |
| property_value | string | Yes | Property value as string (parsed according to property type) |

## Returns

On success, confirms the property was set.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_Title",
  "property_name": "Text",
  "property_value": "Main Menu"
}
```

## Response

### Success
```
Widget property set successfully: TextBlock_Title.Text
```

### Error
```
Failed to set widget property: Property 'InvalidProp' not found on widget 'TextBlock_Title'
```

## Notes

- Uses UE property reflection (UPROPERTY) — works with any exposed widget property.
- Common TextBlock properties: `Text`, `ColorAndOpacity`, `Font`, `ShadowOffset`, `ShadowColorAndOpacity`, `MinDesiredWidth`, `AutoWrapText`, `TextTransformPolicy`.
- Common Image properties: `Brush`, `ColorAndOpacity`, `BrushTintColor`.
- Common Button properties: `BackgroundColor`, `ColorAndOpacity`.
- Common base widget properties: `Visibility` (Visible, Collapsed, Hidden, HitTestInvisible, SelfHitTestInvisible), `IsEnabled`, `RenderOpacity`, `ToolTipText`, `RenderTransformPivot`, `Clipping`.
- Set `bIsVariable` to `true` to expose the widget as a variable in the Blueprint graph.
- Complex types (structs, colors) use UE string import format, e.g. `"(R=1.0,G=0.0,B=0.0,A=1.0)"`.
