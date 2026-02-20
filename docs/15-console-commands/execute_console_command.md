# execute_console_command

Execute a console command in the editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| command | string | Yes | Console command to execute |

## Returns

On success, returns the command output text.

On error, returns the error message from the command execution.

## Example

```json
{
  "command": "stat fps"
}
```

## Notes

- Executes any valid Unreal Engine console command in the editor context.
- The command output is captured and returned as text.
- Be cautious with commands that modify engine state, as they may affect editor stability.
- Some commands may produce no output but still execute successfully.
