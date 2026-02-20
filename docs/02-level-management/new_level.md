# new_level

Create a new empty level at the specified asset path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_path | string | Yes | Asset path for the new level (e.g. "/Game/Maps/NewMap") |

## Returns

**On success:** Confirmation message with the created level's name and asset path.

**On error:** Error message describing why the level could not be created (e.g. invalid path, path already exists).

## Example

```json
{
  "level_path": "/Game/Maps/NewMap"
}
```

## Response

### Success
```
New level created: NewMap (/Game/Maps/NewMap)
```

### Error
```
Failed to create level: Path '/Game/Maps/ExistingMap' already exists
```

## Notes

- The `level_path` must be a valid Unreal Engine asset path starting with "/Game/".
- The created level is empty and becomes the currently loaded level.
- If a level already exists at the specified path, the operation may fail.
