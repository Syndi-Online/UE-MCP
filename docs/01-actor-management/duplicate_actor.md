# duplicate_actor

Duplicate an actor in the editor world. The duplicate can be offset from the original position.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path to duplicate |
| offset | object | No | Translation offset {x, y, z} from the original. Defaults to (0, 0, 0). |

### offset object

| Field | Type | Description |
|-------|------|-------------|
| x | number | X offset |
| y | number | Y offset |
| z | number | Z offset |

## Returns

**On success:** Text containing the duplicated actor's name, label, class, and location (x, y, z).

**On error:** Error message describing why the duplication failed (e.g. source actor not found).

## Example

```json
{
  "actor_identifier": "PointLight_0",
  "offset": { "x": 100.0, "y": 0.0, "z": 0.0 }
}
```

## Notes

- If `offset` is omitted, the duplicate is placed at the same position as the original.
- The duplicated actor receives a new unique name and label.
