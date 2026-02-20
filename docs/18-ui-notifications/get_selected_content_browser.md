# get_selected_content_browser

Get current selection in Content Browser.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text listing of the currently selected items in the Content Browser, divided into two sections:

- **Selected Assets** -- lists the paths of all selected asset items.
- **Selected Folders** -- lists the paths of all selected folder items.

If nothing is selected, returns "No items selected in Content Browser".

On error, returns `isError: true` with a message in the format "Failed to get Content Browser selection: <error>".

## Example

```json
{}
```

## Notes

- This is a read-only query tool that does not modify the selection.
- Both assets and folders can be selected simultaneously in the Content Browser.
