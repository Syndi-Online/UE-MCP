# import_asset

Import a file (FBX, texture, audio, etc.) as an asset.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| file_path | string | Yes | Absolute file path on disk to import |
| destination_path | string | Yes | Content path destination (e.g. '/Game/Meshes') |

## Returns

On success, returns the number of imported assets and a list of their content paths.

On error, returns an error message describing why the import failed.

## Example

```json
{
  "file_path": "C:/Models/character.fbx",
  "destination_path": "/Game/Meshes/Characters"
}
```

## Notes

- The `file_path` must be an absolute path to a file on disk.
- Supported file types include FBX, OBJ, PNG, TGA, WAV, and other formats supported by Unreal Engine's import pipeline.
- A single file import may produce multiple assets (e.g., an FBX file with meshes, materials, and textures).
