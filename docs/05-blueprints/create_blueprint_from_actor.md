# create_blueprint_from_actor

Create a Blueprint from an existing actor in the level.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path for the new Blueprint |
| actor_identifier | string | Yes | Name, label, or path of the source actor in the level |

## Returns

On success, returns `BlueprintName`, `BlueprintPath`, and `SourceActorName` of the created Blueprint.

On error, returns a message describing the failure (e.g., actor not found, invalid path).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_FromActor",
  "actor_identifier": "MyActor_01"
}
```

## Response

### Success
```
Blueprint created from actor successfully.
BlueprintName: BP_FromActor
BlueprintPath: /Game/Blueprints/BP_FromActor
SourceActorName: MyActor_01
```

### Error
```
Failed to create Blueprint from actor: Actor not found: MyActor_01
```

## Notes

- The `actor_identifier` can be the actor's name, display label, or full path in the level.
- The source actor must exist in the currently loaded level.
- The new Blueprint will inherit the components and properties of the source actor.
