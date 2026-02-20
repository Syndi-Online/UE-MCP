# focus_on_actors

Focus the viewport camera on one or more actors.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifiers | array of string | Yes | List of actor identifiers (names or labels) to focus on |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the focus operation failed.

## Example

```json
{
  "actor_identifiers": ["StaticMeshActor_1", "PointLight_0"]
}
```

## Response

### Success
```
Focused on 2 actor(s)
```

### Error
```
Failed to focus on actors: No actors found to focus on
```

## Notes

- The viewport camera will reposition to frame all specified actors in view.
- Actor identifiers can be actor names or labels as they appear in the World Outliner.
- Multiple actors can be specified to frame them all at once.
- This is equivalent to selecting actors and pressing the `F` key in the editor viewport.
