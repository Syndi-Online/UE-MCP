# create_blueprint

Create a new Blueprint from a parent class.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path for the new Blueprint, e.g. "/Game/Blueprints/MyBP" |
| parent_class | string | Yes | Parent class path, e.g. "/Script/Engine.Actor" |

## Returns

On success, returns `BlueprintName`, `BlueprintPath`, and `ParentClass` of the newly created Blueprint.

On error, returns a message describing the failure (e.g., invalid path, class not found).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyCharacter",
  "parent_class": "/Script/Engine.Character"
}
```

## Response

### Success
```
Blueprint created successfully.
BlueprintName: BP_MyCharacter
BlueprintPath: /Game/Blueprints/BP_MyCharacter
ParentClass: Character
```

### Error
```
Failed to create Blueprint: Asset already exists at /Game/Blueprints/BP_MyCharacter
```

## Notes

- The `blueprint_path` must be a valid content path starting with `/Game/`.
- The `parent_class` should be a fully qualified class path (e.g., `/Script/Engine.Actor`, `/Script/Engine.Pawn`, `/Script/Engine.Character`).
- If a Blueprint already exists at the specified path, the operation will fail.
