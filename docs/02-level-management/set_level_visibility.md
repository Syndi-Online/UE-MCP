# set_level_visibility

Set the visibility of a streaming level in the editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_name | string | Yes | Name of the level (e.g. "PersistentLevel" or sublevel name, partial match supported) |
| visible | boolean | Yes | Whether the level should be visible (true) or hidden (false) |

## Returns

**On success:** Confirmation message with the level name and its new visibility state (visible or hidden).

**On error:** Error message describing why the visibility could not be changed (e.g. level not found).

## Example

```json
{
  "level_name": "SubLevel_Lighting",
  "visible": false
}
```

## Response

### Success
```
Level 'SubLevel_Lighting' visibility set to hidden
```

### Error
```
Failed to set level visibility: Level 'NonExistentLevel' not found
```

## Notes

- Partial name matching is supported for `level_name`.
- Visibility affects editor display only; it does not change runtime streaming behavior.
- Hidden levels still exist in the world and their actors can still be queried, but they are not rendered in the editor viewport.
