# set_widget_slot

Set layout slot properties on a widget (anchors, padding, alignment, size).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Widget Blueprint |
| widget_name | string | Yes | Name of the target widget |
| slot_properties | object | Yes | Key-value pairs of slot properties to set (see notes for available keys) |

## Returns

On success, confirms the slot properties were set.

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/UI/WBP_MainMenu",
  "widget_name": "TextBlock_Title",
  "slot_properties": {
    "position_x": "100",
    "position_y": "50",
    "size_x": "400",
    "size_y": "60",
    "anchor_min_x": "0.5",
    "anchor_min_y": "0.0",
    "anchor_max_x": "0.5",
    "anchor_max_y": "0.0",
    "alignment_x": "0.5",
    "alignment_y": "0.0"
  }
}
```

## Response

### Success
```
Widget slot properties set successfully: TextBlock_Title (CanvasPanelSlot)
```

### Error
```
Failed to set widget slot: Widget 'TextBlock_Title' has no slot (not parented)
```

## Notes

- Slot properties depend on the parent widget type:

### CanvasPanelSlot
| Key | Type | Description |
|-----|------|-------------|
| position_x | float | X offset from anchor |
| position_y | float | Y offset from anchor |
| size_x | float | Width (when not auto-sized) |
| size_y | float | Height (when not auto-sized) |
| anchor_min_x | float | Minimum anchor X (0.0-1.0) |
| anchor_min_y | float | Minimum anchor Y (0.0-1.0) |
| anchor_max_x | float | Maximum anchor X (0.0-1.0) |
| anchor_max_y | float | Maximum anchor Y (0.0-1.0) |
| alignment_x | float | Pivot X (0.0-1.0) |
| alignment_y | float | Pivot Y (0.0-1.0) |
| auto_size | bool | Size to content |
| z_order | int | Rendering order |

### HorizontalBoxSlot / VerticalBoxSlot
| Key | Type | Description |
|-----|------|-------------|
| padding_left | float | Left padding |
| padding_top | float | Top padding |
| padding_right | float | Right padding |
| padding_bottom | float | Bottom padding |
| size_rule | string | "Auto" or "Fill" |
| fill_weight | float | Fill weight (when size_rule is Fill) |
| h_align | string | "Left", "Center", "Right", or "Fill" |
| v_align | string | "Top", "Center", "Bottom", or "Fill" |

### OverlaySlot
| Key | Type | Description |
|-----|------|-------------|
| padding_left | float | Left padding |
| padding_top | float | Top padding |
| padding_right | float | Right padding |
| padding_bottom | float | Bottom padding |
| h_align | string | "Left", "Center", "Right", or "Fill" |
| v_align | string | "Top", "Center", "Bottom", or "Fill" |

- Setting `anchor_min == anchor_max` creates a fixed-position widget; different values create a stretch layout.
- Common anchor presets: top-left `(0,0,0,0)`, center `(0.5,0.5,0.5,0.5)`, stretch `(0,0,1,1)`.
