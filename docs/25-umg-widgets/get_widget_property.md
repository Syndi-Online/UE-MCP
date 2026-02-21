# get_widget_property

Read a property value from a widget in a Widget Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the target widget |
| property_name | string | Yes | Property name to read |

## Returns

On success, returns the property value as a string.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_Title",
  "property_name": "Text"
}
```

## Response

### Success
```
TextBlock_Title.Text = Main Menu
```

### Error
```
Failed to get widget property: Property 'InvalidProp' not found on widget 'TextBlock_Title'
```

## Notes

- Uses UE property reflection (UPROPERTY) — works with any exposed widget property.
- Complex types (structs, colors, enums) are returned in UE string export format.
- See `set_widget_property` notes for common property names per widget type.
