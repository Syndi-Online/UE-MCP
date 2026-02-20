# set_actor_folder

Set the folder path for an actor in the World Outliner hierarchy.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_identifier | string | Yes | Actor name, label, or path |
| folder_path | string | Yes | Folder path in the World Outliner (e.g. "Lighting/PointLights"). Use empty string to move to root. |

## Returns

**On success:** Confirmation message that the actor was moved to the specified folder.

**On error:** Error message describing why the folder assignment failed (e.g. actor not found).

## Example

```json
{
  "actor_identifier": "PointLight_0",
  "folder_path": "Lighting/PointLights"
}
```

## Notes

- Folder paths use forward slashes to define hierarchy (e.g. "Lighting/PointLights").
- Setting `folder_path` to an empty string `""` moves the actor to the root of the World Outliner.
- If the specified folder does not exist, it will be created automatically.
- This only affects the organizational structure in the World Outliner; it does not change the actor's world-space position.
