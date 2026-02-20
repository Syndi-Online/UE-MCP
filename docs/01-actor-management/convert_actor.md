# convert_actor

Convert an actor to a different class type. Preserves transform but destroys old actor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path to convert |
| new_class | string | Yes | Full class path of the target type (e.g. "/Script/Engine.PointLight") |

## Returns

**On success:** Text containing the new actor's name, label, and class after conversion.

**On error:** Error message describing why the conversion failed (e.g. source actor not found, invalid target class).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0",
  "new_class": "/Script/Engine.PointLight"
}
```

## Response

### Success
```
Actor converted successfully.
New Name: PointLight_0
New Label: PointLight
New Class: /Script/Engine.PointLight
```

### Error
```
Failed to convert actor: Class '/Script/Engine.InvalidClass' not found
```

## Notes

- The original actor is destroyed during conversion. A new actor of the target class is created in its place.
- The transform (location, rotation, scale) of the original actor is preserved.
- The new actor will have a different name and label than the original.
- The `new_class` must be a full class path.
