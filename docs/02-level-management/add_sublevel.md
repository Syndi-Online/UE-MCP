# add_sublevel

Add an existing level as a streaming sublevel to the current world.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_path | string | Yes | Asset path of the level to add as sublevel (e.g. "/Game/Maps/SubLevel") |

## Returns

**On success:** Confirmation message with the added sublevel's name.

**On error:** Error message describing why the sublevel could not be added (e.g. level not found, already added).

## Example

```json
{
  "level_path": "/Game/Maps/SubLevel_Lighting"
}
```

## Response

### Success
```
Sublevel added: SubLevel_Lighting
```

### Error
```
Failed to add sublevel: Level '/Game/Maps/NonExistentLevel' not found
```

## Notes

- The `level_path` must point to an existing level asset.
- The sublevel is added as a streaming level to the current world.
- Once added, the sublevel's actors become visible and editable in the editor (depending on visibility settings).
