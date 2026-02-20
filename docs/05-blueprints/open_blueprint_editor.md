# open_blueprint_editor

Open a Blueprint in the Blueprint editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint to open |

## Returns

On success, returns `BlueprintName` of the opened Blueprint.

On error, returns a message describing the failure (e.g., Blueprint not found).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor"
}
```

## Response

### Success
```
Blueprint editor opened successfully.
BlueprintName: BP_MyActor
```

### Error
```
Failed to open Blueprint editor: Blueprint not found: /Game/Blueprints/BP_Missing
```

## Notes

- This opens the Blueprint Editor window in the Unreal Editor UI for the specified Blueprint.
- The Blueprint must exist at the specified asset path.
- If the Blueprint is already open in the editor, it will be brought to focus.
