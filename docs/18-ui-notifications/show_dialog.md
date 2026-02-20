# show_dialog

Show a modal dialog in the editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| title | string | Yes | The dialog title |
| message | string | Yes | The dialog message |
| type | string | No | Dialog type: Ok, YesNo, OkCancel, YesNoCancel. Default: Ok |

## Returns

On success, returns the user's response to the dialog (e.g., "Ok", "Yes", "No", "Cancel").

On error, returns `isError: true` with an error message describing what went wrong. Returns an error if `title` or `message` is missing.

## Example

```json
{
  "title": "Confirm Delete",
  "message": "Are you sure you want to delete the selected assets?",
  "type": "YesNo"
}
```

## Response

### Success
```
Yes
```
```
Ok
```
```
Cancel
```

### Error
```
Missing required parameter: title
```
```
Missing required parameter: message
```

## Notes

- This is a blocking modal dialog -- the editor waits for the user to respond before continuing.
- If `type` is not specified, the dialog defaults to "Ok" (a single OK button).
- Valid dialog types are: `Ok`, `YesNo`, `OkCancel`, `YesNoCancel`.
- The return value indicates which button the user clicked.
