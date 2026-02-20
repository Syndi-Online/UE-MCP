# group_actors

Group multiple actors together in the editor.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifiers | array of string | Yes | Array of actor names, labels, or paths to group (minimum 2) |
| group_name | string | No | Optional name for the group |

## Returns

**On success:** Confirmation message with the number of actors grouped and the group name.

**On error:** Error message describing why the grouping failed (e.g. actors not found, fewer than 2 actors provided).

## Example

```json
{
  "actor_identifiers": ["StaticMeshActor_0", "StaticMeshActor_1", "PointLight_0"],
  "group_name": "Building_A"
}
```

## Notes

- A minimum of 2 actors is required to form a group.
- If `group_name` is omitted, a default name is assigned automatically.
- Grouped actors can be manipulated together in the editor.
