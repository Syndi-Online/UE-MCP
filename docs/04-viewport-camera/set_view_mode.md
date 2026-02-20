# set_view_mode

Set the viewport rendering mode (Lit, Unlit, Wireframe, etc.).

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mode | string | Yes | View mode: Lit, Unlit, Wireframe, BrushWireframe, LightingOnly, LightComplexity, ShaderComplexity, DetailLighting, CollisionPawn, CollisionVisibility |

## Returns

On success, returns a confirmation message.

On error, returns an error message describing why the view mode could not be set.

## Example

```json
{
  "mode": "Wireframe"
}
```

## Notes

- The `mode` value must be one of the supported view modes listed above.
- Available modes:
  - **Lit** -- Full lighting and materials (default).
  - **Unlit** -- No lighting, only base colors.
  - **Wireframe** -- Wireframe rendering of geometry.
  - **BrushWireframe** -- Wireframe for BSP brushes.
  - **LightingOnly** -- Shows only lighting without textures.
  - **LightComplexity** -- Visualizes light overlap complexity.
  - **ShaderComplexity** -- Visualizes shader instruction count.
  - **DetailLighting** -- Detailed lighting view.
  - **CollisionPawn** -- Shows pawn collision geometry.
  - **CollisionVisibility** -- Shows visibility collision geometry.
