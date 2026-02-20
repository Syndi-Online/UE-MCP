# remove_blueprint_variable

Remove a variable from a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| variable_name | string | Yes | Name of the variable to remove |

## Returns

On success, returns `VariableName` of the removed variable.

On error, returns a message describing the failure (e.g., Blueprint not found, variable not found).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "variable_name": "Health"
}
```

## Notes

- The variable must exist in the Blueprint.
- Removing a variable that is referenced in Blueprint graphs may cause compilation errors.
- After removing a variable, you may need to compile the Blueprint for changes to take effect.
