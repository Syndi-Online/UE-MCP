# save_level

Save the current level.

## Parameters

This tool takes no parameters.

## Returns

**On success:** Confirmation message with the saved level's name.

**On error:** Error message describing why the level could not be saved.

## Example

```json
{}
```

## Response

### Success
```
Level saved: MyLevel
```

### Error
```
Failed to save level: No level currently loaded
```

## Notes

- Saves the currently active level to disk.
- If the level has never been saved before, it will be saved to its assigned asset path.
