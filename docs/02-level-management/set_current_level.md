# set_current_level

Set the current working level for new actor placement.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_name | string | Yes | Name of the level to make current (e.g. "PersistentLevel" or sublevel name) |

## Returns

**On success:** Confirmation message with the name of the level that is now current.

**On error:** Error message describing why the level could not be set as current (e.g. level not found).

## Example

```json
{
  "level_name": "SubLevel_Gameplay"
}
```

## Response

### Success
```
Current level set to: SubLevel_Gameplay
```

### Error
```
Failed to set current level: Level 'NonExistentLevel' not found
```

## Notes

- The current level determines where newly spawned actors are placed.
- The level must be loaded (either the persistent level or an added sublevel).
- Use "PersistentLevel" to switch back to the main persistent level.
