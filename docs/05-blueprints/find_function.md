# find_function

Search for Blueprint-callable functions by name substring. Use before `add_graph_node` to find the exact function name (K2_ prefix, display name differences, etc.).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| search | string | Yes | Search substring (case-insensitive, matched against function name and display name) |
| class_name | string | No | Restrict search to this class (e.g. `KismetSystemLibrary`, `Actor`). If omitted, searches common engine classes. |
| limit | integer | No | Maximum number of results to return (default: 10) |
| blueprint_callable_only | boolean | No | Only return BlueprintCallable functions (default: true) |

## Returns

On success, returns a list of matching functions with their class, display name, and parameter names.

On error, returns a message describing the failure.

## Example

### Search by name
```json
{
  "search": "PrintString"
}
```

### Search in specific class
```json
{
  "search": "SetTimer",
  "class_name": "KismetSystemLibrary",
  "limit": 5
}
```

### Search all functions (not just BlueprintCallable)
```json
{
  "search": "GetActorLocation",
  "class_name": "Actor",
  "blueprint_callable_only": false
}
```

## Response

### Success
```
Found 2 functions matching "PrintString":
- K2_PrintString (UKismetSystemLibrary) [Print String] params: InString, bPrintToScreen, bPrintToLog, TextColor, Duration, Key
- PrintString (UKismetSystemLibrary) [Print String] params: InString
```

### Error
```
Missing required parameter: search
```
```
No functions found matching "NonExistent" (searched 10 classes)
```

## Notes

- Searches function names and display names (from `DisplayName` metadata).
- When `class_name` is omitted, searches these default classes: `UKismetSystemLibrary`, `UKismetMathLibrary`, `UKismetStringLibrary`, `AActor`, `APawn`, `ACharacter`, `UActorComponent`, `USceneComponent`, `UPrimitiveComponent`.
- Class resolution supports: exact name, U-prefix, A-prefix, and full object path.
- Use the returned function name (not display name) as `member_name` in `add_graph_node`.
- Return parameters (with `CPF_ReturnParm` flag) are excluded from the params list.
- Results are deduplicated across class hierarchies.
