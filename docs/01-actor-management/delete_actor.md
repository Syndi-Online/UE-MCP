# delete_actor

Delete an actor from the editor world by name, label, or path.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path (e.g. "StaticMeshActor_0" or "PersistentLevel.StaticMeshActor_0") |

## Returns

**On success:** Confirmation message with the deleted actor's name.

**On error:** Error message describing why the deletion failed (e.g. actor not found).

## Example

```json
{
  "actor_identifier": "StaticMeshActor_0"
}
```

## Response

### Success
```
Actor 'StaticMeshActor_0' deleted successfully.
```

### Error
```
Failed to delete actor: Actor 'NonExistentActor' not found
```

## Notes

- The actor can be identified by its internal name, display label, or full path.
- The actor is permanently removed from the editor world.
