# apply_anim_compression

Apply bone compression settings to an animation sequence.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| anim_sequence_path | string | Yes | Asset path of the animation sequence |
| compression_settings_path | string | No | Asset path of the bone compression settings (empty to use default) |

## Returns

On success, returns a confirmation message indicating compression was applied to the animation sequence.

On error, returns an error message describing the failure reason (e.g., animation not found, invalid compression settings).

## Example

```json
{
  "anim_sequence_path": "/Game/Animations/Idle_Anim",
  "compression_settings_path": "/Game/Settings/CS_HighQuality"
}
```

Using default compression:

```json
{
  "anim_sequence_path": "/Game/Animations/Idle_Anim"
}
```

## Notes

- If `compression_settings_path` is not provided or is an empty string, the engine's default bone compression settings are used.
- Animation compression reduces the memory footprint of animation data at the cost of some precision.
- The compression settings asset must already exist in the project when specifying a custom path.
