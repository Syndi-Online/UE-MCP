# create_anim_blueprint

Create a new Animation Blueprint for a skeleton.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| skeleton_path | string | Yes | Asset path of the skeleton to use |
| asset_name | string | Yes | Name for the new Anim Blueprint |
| package_path | string | Yes | Package path where the asset will be created, e.g. /Game/Blueprints |

## Returns

On success, returns a confirmation message with the created Anim Blueprint name and its full asset path.

On error, returns an error message describing the failure reason (e.g., skeleton not found, path conflict).

## Example

```json
{
  "skeleton_path": "/Game/Characters/SK_Mannequin_Skeleton",
  "asset_name": "ABP_Mannequin",
  "package_path": "/Game/Blueprints"
}
```

## Notes

- The skeleton at `skeleton_path` must already exist in the project.
- The created Animation Blueprint will be empty and needs its animation graph to be set up separately.
- Animation Blueprints control how animations are blended and played on a skeletal mesh at runtime.
