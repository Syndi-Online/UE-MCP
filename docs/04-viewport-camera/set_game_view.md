# set_game_view

Toggle Game View mode in the viewport.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| enabled | boolean | Yes | Whether to enable or disable Game View mode |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the view mode could not be set.

## Example

```json
{
  "enabled": true
}
```

## Response

### Success
```
Game View enabled
```

### Error
```
Failed to set game view: ULevelEditorSubsystem is not available
```

## Notes

- Game View mode hides editor-only elements (icons, wireframes, debug shapes, etc.) to show the viewport as it would appear in-game.
- This is equivalent to pressing the `G` shortcut key in the editor viewport.
