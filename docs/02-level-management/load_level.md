# load_level

Load/open an existing level by asset path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_path | string | Yes | Asset path of the level to load (e.g. "/Game/Maps/MyLevel") |

## Returns

**On success:** Confirmation message with the loaded level's name.

**On error:** Error message describing why the level could not be loaded (e.g. level not found, invalid path).

## Example

```json
{
  "level_path": "/Game/Maps/MyLevel"
}
```

## Response

### Success
```
Level loaded: MyLevel
```

### Error
```
Failed to load level: Level '/Game/Maps/NonExistentLevel' not found
```

## Notes

- The `level_path` must point to an existing level asset.
- Loading a level replaces the currently open level in the editor.
- Any unsaved changes in the current level may be lost; consider using `save_level` or `save_all_dirty_levels` first.
