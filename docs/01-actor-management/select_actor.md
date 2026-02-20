# select_actor

Select one or more actors in the editor. Can replace or add to the current selection.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifiers | array of string | Yes | Array of actor names, labels, or paths to select |
| add_to_selection | boolean | No | If true, add to existing selection instead of replacing it. Default: false. |

## Returns

**On success:** Confirmation message with the number of actors selected.

**On error:** Error message describing why the selection failed.

## Example

```json
{
  "actor_identifiers": ["PointLight_0", "StaticMeshActor_1"],
  "add_to_selection": false
}
```

## Notes

- By default, the current selection is replaced. Set `add_to_selection` to `true` to add to the existing selection.
- Each identifier can be an actor name, display label, or path.
