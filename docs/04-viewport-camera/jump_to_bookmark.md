# jump_to_bookmark

Jump to a previously saved camera bookmark.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| index | number | Yes | Bookmark index (0-9) |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the jump failed (e.g., no bookmark at the specified index).

## Example

```json
{
  "index": 3
}
```

## Notes

- Bookmark indices range from 0 to 9.
- The bookmark must have been previously saved using `set_bookmark`.
- Jumping to a bookmark restores the viewport camera to the saved location and rotation.
- This is equivalent to pressing 0 through 9 in the editor viewport.
