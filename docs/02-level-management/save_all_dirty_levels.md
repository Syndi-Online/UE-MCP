# save_all_dirty_levels

Save all modified (dirty) levels.

## Parameters

This tool takes no parameters.

## Returns

**On success:** Confirmation message that all dirty levels were saved successfully.

**On error:** Error message describing why the save operation failed.

## Example

```json
{}
```

## Response

### Success
```
All dirty levels saved successfully
```

### Error
```
Failed to save dirty levels: No editor world available
```

## Notes

- Saves all levels that have unsaved modifications, including the persistent level and any loaded sublevels.
- This is useful after performing multiple operations across different levels.
- Levels that have not been modified are skipped.
