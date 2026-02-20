# get_actors_in_level

Get a list of all actors in the current level. Optionally filter by class.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| class_filter | string | No | Optional class path to filter actors (e.g. "/Script/Engine.StaticMeshActor") |

## Returns

**On success:** A count of found actors followed by a list. Each entry includes the actor's label, name, class, location (x, y, z), and folder path.

**On error:** Error message describing why the query failed.

## Example

```json
{
  "class_filter": "/Script/Engine.StaticMeshActor"
}
```

## Notes

- If `class_filter` is omitted or empty, all actors in the level are returned.
- The class filter must be a full class path (e.g. "/Script/Engine.PointLight").
- Results include each actor's label, internal name, class, world location, and World Outliner folder path.
