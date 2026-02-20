# compile_blueprint

Compile a Blueprint asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint to compile |

## Returns

On success, returns `NumErrors` and `NumWarnings` from the compilation result.

On error, returns a message describing the compilation failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor"
}
```

## Notes

- The Blueprint must exist at the specified asset path.
- Compilation is required after making structural changes to a Blueprint (adding variables, functions, etc.) to apply those changes at runtime.
- Even on success, there may be warnings reported via `NumWarnings`.
