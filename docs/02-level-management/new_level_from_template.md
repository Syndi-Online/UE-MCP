# new_level_from_template

Create a new level from an existing template level.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_path | string | Yes | Asset path for the new level (e.g. "/Game/Maps/NewMap") |
| template_path | string | Yes | Asset path of the template level (e.g. "/Game/Maps/Template") |

## Returns

**On success:** Confirmation message with the created level's name and asset path.

**On error:** Error message describing why the level could not be created from the template (e.g. template not found, invalid paths).

## Example

```json
{
  "level_path": "/Game/Maps/MyNewLevel",
  "template_path": "/Game/Maps/MyTemplate"
}
```

## Response

### Success
```
Level created from template: MyNewLevel (/Game/Maps/MyNewLevel)
```

### Error
```
Failed to create level from template: Template '/Game/Maps/NonExistentTemplate' not found
```

## Notes

- The `template_path` must point to an existing level asset.
- The new level is a copy of the template, including all actors and settings from the template.
- Both paths must be valid Unreal Engine asset paths.
