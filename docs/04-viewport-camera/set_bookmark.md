# set_bookmark

Save the current camera position as a bookmark.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| index | number | Yes | Bookmark index (0-9) |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the bookmark could not be saved.

## Example

```json
{
  "index": 3
}
```

## Notes

- Bookmark indices range from 0 to 9, providing up to 10 saved camera positions.
- Saving a bookmark to an index that already has a bookmark will overwrite it.
- Bookmarks store the current viewport camera location and rotation.
- Use `jump_to_bookmark` to restore a saved camera position.
- This is equivalent to pressing Ctrl+0 through Ctrl+9 in the editor viewport.
