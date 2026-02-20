# deselect_all

Deselect all currently selected actors in the editor.

## Parameters

This tool takes no parameters.

## Returns

**On success:** Confirmation message that all actors have been deselected.

**On error:** Error message describing why the deselection failed.

## Example

```json
{}
```

## Response

### Success
```
All actors deselected
```

### Error
```
Failed to deselect: No editor world available
```

## Notes

- This clears the entire editor selection in one call.
- If no actors are currently selected, the tool still succeeds.
