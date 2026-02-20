# reparent_blueprint

Change the parent class of a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| new_parent_class | string | Yes | Class path of the new parent class |

## Returns

On success, returns `OldParentClass` and `NewParentClass` names.

On error, returns a message describing the failure (e.g., Blueprint not found, invalid parent class, incompatible class hierarchy).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "new_parent_class": "/Script/Engine.Character"
}
```

## Notes

- The `new_parent_class` should be a fully qualified class path (e.g., `/Script/Engine.Actor`, `/Script/Engine.Pawn`).
- Reparenting may cause compilation errors if the Blueprint uses features or variables specific to the old parent class.
- After reparenting, you should compile the Blueprint to verify compatibility.
