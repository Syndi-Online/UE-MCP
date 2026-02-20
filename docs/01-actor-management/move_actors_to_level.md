# move_actors_to_level

Move one or more actors to a different streaming level.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifiers | array of string | Yes | Array of actor names, labels, or paths to move |
| level_name | string | Yes | Name of the target level (e.g. "PersistentLevel" or sublevel name) |

## Returns

**On success:** Confirmation message with the number of actors moved and the target level name.

**On error:** Error message describing why the move failed (e.g. actors not found, target level not found).

## Example

```json
{
  "actor_identifiers": ["StaticMeshActor_0", "PointLight_1"],
  "level_name": "SubLevel_Lighting"
}
```

## Notes

- The target level must be a loaded streaming sublevel or the persistent level.
- Actors retain their world-space transforms after being moved.
- At least one valid actor identifier must be provided.
