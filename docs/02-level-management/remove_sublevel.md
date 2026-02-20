# remove_sublevel

Remove a streaming sublevel from the current world.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| level_name | string | Yes | Name of the sublevel to remove (partial match supported) |

## Returns

**On success:** Confirmation message with the removed sublevel's name.

**On error:** Error message describing why the sublevel could not be removed (e.g. sublevel not found).

## Example

```json
{
  "level_name": "SubLevel_Lighting"
}
```

## Notes

- Partial name matching is supported, so you do not need to provide the full path.
- Removing a sublevel detaches it from the current world but does not delete the level asset from disk.
- The persistent level cannot be removed.
