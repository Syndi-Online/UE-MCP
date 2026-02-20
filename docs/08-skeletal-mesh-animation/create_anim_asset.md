# create_anim_asset

Create a new animation asset (AnimSequence, BlendSpace, BlendSpace1D, or AnimMontage).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| skeleton_path | string | Yes | Asset path of the skeleton to use |
| asset_name | string | Yes | Name for the new animation asset |
| package_path | string | Yes | Package path where the asset will be created, e.g. /Game/Animations |
| asset_type | string | Yes | Type of animation asset: AnimSequence, BlendSpace, BlendSpace1D, AnimMontage |

## Returns

On success, returns a confirmation message with the created asset name and its full asset path.

On error, returns an error message describing the failure reason (e.g., skeleton not found, invalid asset type, path conflict).

## Example

```json
{
  "skeleton_path": "/Game/Characters/SK_Mannequin_Skeleton",
  "asset_name": "Idle_Anim",
  "package_path": "/Game/Animations",
  "asset_type": "AnimSequence"
}
```

## Notes

- Supported asset types:
  - **AnimSequence** -- a standard animation sequence
  - **BlendSpace** -- a 2D blend space for blending between animations
  - **BlendSpace1D** -- a 1D blend space for blending between animations
  - **AnimMontage** -- an animation montage for compositing animations
- The skeleton at `skeleton_path` must already exist in the project.
- The created asset will be empty and needs to be populated with animation data separately.
